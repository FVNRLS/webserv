#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <csignal>
#include <netdb.h>

#include "Connection.hpp"
#include "Configuration.hpp"
#include "IOException.hpp"
#include "CGIResponseError.hpp"

static void stopper(int);

Connection::Connection()
		: _timeout(-1), _nfds(0), _fds(), connections(0) {
	bzero(_fds, sizeof(_fds));
	struct sockaddr_in address = {};
	bzero(&address, sizeof(address));
	const std::vector<int> & ports = Configuration::getInstance().get_server_ports();
	for (unsigned long i = 0; i < ports.size(); ++i) {
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(ports[i]);

		int server_fd;
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			throw IOException("Could not create socket!");
		}
		int on = 1;
		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		fcntl(server_fd, F_SETFL, O_NONBLOCK);
		if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0) {
			throw IOException("Could not bind file descriptor to the address!");
		}
		if (listen(server_fd, 128) < 0) {
			throw IOException("Cannot listen on the socket descriptor!");
		}
		_fds[i].fd = server_fd;
		_fds[i].events = POLLIN;
		_server_fds[server_fd] = ports[i];
	}
	signal(SIGINT, stopper);
	signal(SIGTERM, stopper);
	signal(SIGKILL, stopper);
	currentInstance = this;
}

Connection::~Connection() {
	currentInstance = NULL;
}

Connection * Connection::currentInstance = NULL;

volatile bool end_server = false;

static void stopper(int) {
	end_server = true;
}

void Connection::_clear_poll_array() {
	unsigned long i, j;
	for (i = 0, j = 0; i < _nfds; ++i) {
		if (_fds[i].fd != -1) {
			_fds[j++] = _fds[i];
		}
	}
	_nfds = j;
}

bool Connection::_is_serving_fd(int fd) {
	return _server_fds.find(fd) != _server_fds.end();
}

void Connection::establishConnection() {
	int rc;

	_nfds = _server_fds.size();
	bool clean = false;
	while (!end_server && (rc = poll(_fds, _nfds, _timeout)) > 0) {
		debug("total Connection count: " << connections);
		printPollArray();
		nfds_t currentSize = _nfds;
		for (nfds_t i = 0; i < currentSize; ++i) {
			if (_fds[i].revents == 0) {
				// Ignore...
			} else if (_is_serving_fd(_fds[i].fd)) {
				accept(i);
			} else {
				handle(i);
			}
		}
		_clear_poll_array();
		if (clean) {
			_clean_readers();
			clean = false;
		} else {
			clean = true;
		}
	}
	if (end_server) {
		std::cout << "Interrupted, exiting..." << std::endl;
	} else if (rc == 0) {
		std::cout << "Poll timed out, exiting..." << std::endl;
	} else {
		std::cerr << "Polling error! Exiting..." << std::endl;
	}
}

void Connection::accept(nfds_t i) {
	++connections;
	int socketDescriptor;

	while ((socketDescriptor = ::accept(_fds[i].fd, NULL, NULL)) >= 0) {
		HTTPReader current(socketDescriptor);
		std::list<HTTPReader>::iterator it = _readers.insert(_readers.end(), current);
		current.getSocket().invalidate();
		if (!add_fd(socketDescriptor, &(*it))) {
			denyConnection(socketDescriptor);
			continue;
		}
		_connection_pairs[socketDescriptor] = _fds[i].fd;
		struct sockaddr_in address = {};
		bzero(&address, sizeof(address));
		socklen_t addrlen;
		getpeername(socketDescriptor, reinterpret_cast<struct sockaddr *>(&address), &addrlen);
		it->setPeerAddress(ntohl(address.sin_addr.s_addr));
		char host[50] = {0};
		getnameinfo(reinterpret_cast<struct sockaddr *>(&address), addrlen, host, static_cast<socklen_t>(50), NULL, 0, 0);
		it->setPeerName(host);
		const int our_fd = _server_fds[_connection_pairs[socketDescriptor]];
		bzero(&address, sizeof(address));
		getpeername(_fds[i].fd, reinterpret_cast<struct sockaddr *>(&address), &addrlen);
		it->setUsedPort(our_fd);
	}
}


void Connection::denyConnection(const int fd, const int errorCode) _NOEXCEPT {
	try {
		remove_fd(fd);
		Socket socket(fd);
		HTTPHeader h;
		h.setStatusCode(errorCode);
		h.setStatusMessage(get_message(errorCode));
		h.set_content_type("text/plain");
		std::string body = "Fatal error! Try again later!";
		h.set_content_length(static_cast<int>(body.size()));
		socket.write(h.tostring() + "\r\n\r\n" + body);
	} catch (std::exception & ex) {
		std::cerr << "Could not send error " << errorCode << "!" << std::endl
				  << "Exception: " << ex.what()                  << std::endl;
	}
}

void Connection::handle(nfds_t i) {
	int fd = _fds[i].fd;
	try {
		if (_fd_mapping.find(fd)->second->runForFD(fd, _fds[i].revents & POLLHUP || _fds[i].revents & POLLERR || _fds[i].revents & POLLNVAL)) {
			remove_fd(fd);
		}
	}
	catch (std::bad_alloc &) {
		denyConnection(fd, 507);
	}
	catch (std::exception & ex) {
		std::cerr << ">>>>>>> " << ex.what() << " <<<<<<<" << std::endl;
		denyConnection(fd, 500);
	}
}

bool Connection::add_fd(int fd, Runnable * reader, bool read) {
	nfds_t i;
	for (i = 0; i < _nfds && _fds[i].fd != fd; ++i);
	if (i >= NUM_FDS) {
		return false;
	}
	_fd_mapping[fd] = reader;
	_fds[i].fd = fd;
	_fds[i].events = read ? POLLIN : POLLOUT;
	debug("Added " << fd << " (" << i << ")" << reinterpret_cast<unsigned long>(reader));
	if (i == _nfds) {
		++_nfds;
	}
	return true;
}

bool Connection::remove_fd(int fd) {
	nfds_t i;

	for (i = 0; i < _nfds && _fds[i].fd != fd; ++i);
	if (i != _nfds) {
		_connection_pairs.erase(fd);
		debug("Removed " << fd << " (" << i << ")");
		_fds[i].fd = -1;
		std::map<int, Runnable *>::const_iterator it = _fd_mapping.find(fd);
		if (it != _fd_mapping.end()) {
			_fd_mapping.erase(it);
		}
	}
	return i != _nfds;
}

Connection & Connection::getInstance() {
	return *currentInstance;
}

void Connection::printPollArray() _NOEXCEPT {
#ifdef DEBUG
	#if DEBUG == 2
    std::cout << std::endl << __FILE__ << ":" << __LINE__ << " Poll array" << std::endl;
    for (unsigned long i = 0; i < _nfds; ++i) {
        std::cout << "fd:      " << _fds[i].fd     << std::endl
                  << "events:  " << _fds[i].events << std::endl
                  << "revents: ";
        switch (_fds[i].revents) {
            case POLLIN:     std::cout << "POLLIN";     break;
            case POLLERR:    std::cout << "POLLERR";    break;
            case POLLHUP:    std::cout << "POLLHUP";    break;
            case POLLOUT:    std::cout << "POLLOUT";    break;
            case POLLPRI:    std::cout << "POLLPRI";    break;
            case POLLNVAL:   std::cout << "POLLNVAL";   break;
            case POLLRDBAND: std::cout << "POLLRDBAND"; break;
            case POLLRDNORM: std::cout << "POLLRDNORM"; break;
            case POLLWRBAND: std::cout << "POLLWRBAND"; break;
            default: std::cout << _fds[i].revents;
        }

        std::cout << std::endl << std::endl;
    }
    std::cout << __FILE__ << ":" << __LINE__ << " ---------" << std::endl << std::endl;
 #endif
#endif
}

static bool notMarked(const HTTPReader & reader) {
	return !reader.isMarked();
}

void Connection::_clean_readers() {
	for (std::list<HTTPReader>::iterator it = _readers.begin(); it != _readers.end(); ++it) {
		it->setMarked(false);
	}
	for (std::map<int, Runnable *>::iterator it = _fd_mapping.begin(); it != _fd_mapping.end(); ++it) {
		it->second->setMarked(true);
	}
	_readers.remove_if(notMarked);
}
