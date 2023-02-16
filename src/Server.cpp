/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:36:37 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/14 17:50:29 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//BASIC CLASS SETUP
Server::Server(std::vector<Socket> &sockets) : _sockets(sockets) {
	if (_cli.start() == EXIT_FAILURE)
		exit_server();
	_pfds.push_back(_cli.get_pollfd());
	for (size_t i = 1; i < sockets.size(); i++) {
		_pfds.push_back(sockets[i].get_pollfd());
		_pfds[i].events = POLLIN;
	}
}

Server::~Server() {}

//MEMBER FUNCTIONS
int Server::run() {
	while (true) {
		if (poll(_pfds.data(), _pfds.size(), TIMEOUT) < 0)
			return (terminate_with_error(system_call_error(POLL_ERROR, _sockets.front())));
		if (check_cli() == EXIT_FAILURE)
			return EXIT_FAILURE;
		if (accept_requests() == EXIT_FAILURE)
			return EXIT_FAILURE;
		if (resolve_requests() == EXIT_FAILURE)
			return EXIT_FAILURE;
		delete_invalid_fds();
//		std::cout << "NUM FD'S:		" << _pfds.size() << std::endl;
	}
}

int	Server::check_cli() {
	if (_pfds[0].revents & POLLIN) {
		if (process_cli_input() == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Server::accept_requests() {
	socklen_t 			client_len;
	struct sockaddr 	client_addr;
	pollfd				client_pollfd;

	for (size_t i = 1; i < _sockets.size(); i++) {
		if (_pfds[i].revents & POLLIN) {
//            std::cerr << "New request at " << i << std::endl;
			client_len = sizeof(client_addr);
			client_pollfd.fd = accept(_pfds[i].fd, &client_addr, &client_len);
			if (client_pollfd.fd < 0)
				return (system_call_error(ACCEPT_ERROR, _sockets[i]));
			if (fcntl(client_pollfd.fd, F_SETFL, O_NONBLOCK) < 0)
				return EXIT_FAILURE;
			client_pollfd.events = POLLIN;
			_pfds.push_back(client_pollfd);
			_requests[client_pollfd.fd] = request_handler();
            _requests[client_pollfd.fd].socket = _sockets[i];
		}
	}
	return EXIT_SUCCESS;
}

int Server::resolve_requests() {
	for (size_t i = _sockets.size(); i < _pfds.size(); i++) {
		if (check_connection(_pfds[i]) == EXIT_FAILURE)
			continue;
		if (_pfds[i].revents & POLLIN)
			return (handle_pollin(_pfds[i]));
		else if (_pfds[i].revents & POLLOUT)
			return (handle_pollout(_pfds[i]));
	}
	return EXIT_SUCCESS;
}

int Server::handle_pollin(pollfd &pfd) {
    request_handler* request = &_requests.find(pfd.fd)->second;

	if (accumulate(*request, pfd.fd) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (request->head_received) {
        request->status = handle_request_header(*request);
        if (request->buf.size() >= request->head_length + request->body_length)
            request->body_received = true;
    }

	if (request->status || request->method == "GET" || request->body_received)
		pfd.events = POLLOUT;
	return EXIT_SUCCESS;
}

int Server::handle_pollout(pollfd &pfd) {
	request_handler* request = &_requests.find(pfd.fd)->second;


    if (request->response.empty()) {
        ResponseGenerator resp_gen(*request);
        request->response = resp_gen.generate_response(_cookies);
        std::cerr << "----------------------------------------\n";
        std::cerr << "Request type: " << request->method << '\n';
        std::cerr << "Path: " << request->file_path << '\n';
        std::cerr << "Response of " << request->response.size() << " bytes" <<
                  std::endl;
        std::cerr << "Response: " << request->response << std::endl;
    }
//    std::cerr << "Total bytes sent: " << request->bytes_sent << std::endl;
	if (send_response(pfd.fd, request) || request->response_sent) {
//        std::cerr << "Total bytes sent at completion: " << request->bytes_sent
//        <<
//        std::endl;
////        std::cerr << "----------------------------------------\n" << std::endl;
//        if (request->response.size() < 200)
//            sleep(1);
        request->clear();
        pfd.events = POLLIN;
    }
	return EXIT_SUCCESS;
}


int Server::send_response(int fd, request_handler *request) {

    ssize_t bytes = send(fd, request->response.data(), request->response.size(), 0);
//    std::cerr << "Bytes sent: " << bytes << std::endl;
    if (bytes == -1)
        return EXIT_FAILURE;
    request->bytes_sent += bytes;
//    std::cerr << "Request->bytes_sent after incrementing: " <<
//    request->bytes_sent << std::endl;
    if (bytes == 0 || request->bytes_sent >= request->response.size())
        request->response_sent = true;
    else
        request->response = request->response.substr(bytes);
    return EXIT_SUCCESS;
}

int Server::check_connection(pollfd &pfd) {
	if (pfd.revents & POLLERR || pfd.revents & POLLHUP || pfd.revents & POLLNVAL) {
		std::map<int, request_handler>::iterator it = _requests.find(pfd.fd);

		if (it != _requests.end())
			_requests.erase(it);
		shutdown(pfd.fd, SHUT_RDWR);
		close(pfd.fd);
		pfd.fd = -1;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int	Server::accumulate(request_handler &request, int request_fd) {
	char 										buffer[2000];
	long 										bytes;

	bytes = recv(request_fd, buffer, sizeof(buffer), MSG_DONTWAIT);
	if (bytes < 0)
		return (system_call_error(RECV_ERROR));
	request.buf += std::string(buffer, bytes);
//    std::cout << request.buf << '\n';
	set_request_end_flags(request);
	return EXIT_SUCCESS;
}

void	Server::set_request_end_flags(request_handler &request) {
	long long	max_client_body_size = request.socket.get_config().get_max_client_body_size();

	if ((static_cast<long long>(request.buf.length()) > max_client_body_size)) {
		request.status = BAD_REQUEST;
		return;
	}
	request.head_length = request.buf.find(END_OF_REQUEST);
	if (request.head_length != std::string::npos) {
		request.head_length += std::strlen(END_OF_REQUEST.c_str());
		request.head_received = true;
	}
}

int Server::handle_request_header(request_handler &request) {
    requestParser	request_parser(request);

    if (!request.file_path.empty())
        return request.status;
    request_parser.parse();
    if (request.status)
        return request.status;
	if (!_cookies.exists(request.cookies))
		request.cookies = false;
	request.cookies = check_logout(request.cookies, request.query);
    return request.status;
}

int	Server::check_logout(const int &key, const std::string &query) {
	if (key && query.find("action=logout") != std::string::npos) {
		_cookies.delete_session(key);
		return false;
	}
	return key;
}
void	Server::delete_invalid_fds() {
	std::vector<pollfd>::iterator it = _pfds.begin() + _sockets.size();

	while (it != _pfds.end()) {
		if (it->fd == -1)
			it = _pfds.erase(it);
		else
			it++;
	}
}

//TERMINAL FUNCTIONS
int	Server::process_cli_input() {
	switch (_cli.check_input()) {
		case CLI_EMPTY:
			return EXIT_SUCCESS;
		case CLI_FAIL:
			return terminate_with_error(system_call_error(CLI_ERROR, _sockets.front()));
		case CLI_EXIT:
			exit_server();
		case CLI_LS:
			show_connections();
			break;
		default:
			show_manual();
	}
	return EXIT_SUCCESS;
}

void	Server::exit_server() {
	for (size_t i = 0; i < _sockets.size(); i++)
		close(_pfds[i].fd);
	exit(EXIT_SUCCESS);
}

int		Server::terminate_with_error(int) {
	for (size_t i = 0; i < _sockets.size(); i++)
		close(_pfds[i].fd);
	return EXIT_FAILURE;
}

void 	Server::show_connections() {
	print_configurations();
}

void	Server::print_configurations() {
	for (size_t i = 1; i < _sockets.size(); i++)
		std::cout << &_sockets[i].get_config() << std::endl;
}

void 	Server::show_manual() {
	std::cout << "Let me help you!\nSIGINT was silenced in order to close webserver correctly!\n"
    << "Commands:\n"
    << "ls     prints server configuration\n"
    << "exit   exits webserver properly!\n";
}


//ERROR MANAGEMENT
int Server::system_call_error(int error, const Socket &socket) {
	switch(error) {
		case POLL_ERROR:
			std::cerr << "Error: Failed polling sockets" << std::endl;
			break;
		case ACCEPT_ERROR:
			std::cerr << "Error: Failed to accept connection on port " << socket.get_port() << std::endl;
			break;
		case CLI_ERROR:
			std::cerr << "Error: Command line interface failed" << std::endl;
			break;
		case RECV_ERROR:
			std::cerr << "Error: Failed to receive request" << std::endl;
			break;
		case SEND_ERROR:
			std::cerr << "Error: Failed To Send a Response On Port " << socket.get_port() << std::endl;
			break;
		case ACCESS_DENIED:
			std::cerr << "Error: Access Denied!" << std::endl;
			break;
		default:
			std::cerr << "Server: unknown error" << std::endl;
	}
	return EXIT_FAILURE;
}

