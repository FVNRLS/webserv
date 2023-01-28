/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:36:37 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/22 17:44:52 by doreshev         ###   ########.fr       */
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
			return (EXIT_FAILURE);
		if (accept_requests() == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (resolve_requests() == EXIT_FAILURE)
			return (EXIT_FAILURE);
		delete_invalid_fds();
//		std::cout << "NUM FD'S:		" << _pfds.size() << std::endl; //todo: check on high loaded server!
	}
}

int	Server::check_cli() {
	if (_pfds[0].revents & POLLIN) {
		if (process_cli_input() == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	return (EXIT_SUCCESS);
}

int Server::accept_requests() {
	socklen_t 			client_len;
	struct sockaddr 	client_addr = {};
	pollfd				client_pollfd = {};
	request_handler		request;

	for (size_t i = 1; i < _sockets.size(); i++) {
		if (_pfds[i].revents & POLLIN) {
			client_len = sizeof(client_addr);
			client_pollfd.fd = accept(_pfds[i].fd, &client_addr, &client_len);
			if (client_pollfd.fd < 0)
				return (system_call_error(ACCEPT_ERROR, _sockets[i]));
			if (fcntl(client_pollfd.fd, F_SETFL, O_NONBLOCK) < 0)
				return (EXIT_FAILURE);
			client_pollfd.events = POLLIN;
			_pfds.push_back(client_pollfd);
			request.socket = _sockets[i];
			_requests[client_pollfd.fd] = request;
		}
	}
	return (EXIT_SUCCESS);
}

int Server::resolve_requests() {
	std::string 		content_length;
	std::string 		request;
	std::string 		response;
	std::map<int, request_handler>::iterator	it;


	for (size_t i = _sockets.size(); i < _pfds.size(); i++) {
		if (check_connection(_pfds[i]) == EXIT_FAILURE)
			continue;
		if (_pfds[i].revents & POLLIN) {
			if (accumulate_request(_pfds[i].fd) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			if (check_request(request, _pfds[i]) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		if (_pfds[i].revents == POLLOUT) {
			std::cout << request << std::endl;
			it = _requests.find(_pfds[i].fd);
			ResponseGenerator resp_gen(_pfds[i], (*it).second.socket, (*it).second.buf);
			response = resp_gen.generate_response();
			if (!response.empty()) {
				if (send(_pfds[i].fd, response.c_str(), response.length(), 0) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
			close(_pfds[i].fd);
		}
	}
	return (EXIT_SUCCESS);
}

int	Server::check_request(std::string &request, pollfd &pfd) {
	std::map<int, request_handler>::iterator	it;
	long long 									max_client_body_size;

	request = (_requests.find(pfd.fd))->second.buf;
	it = _requests.find(pfd.fd);
	max_client_body_size = (*it).second.socket.get_config().get_max_client_body_size();
	if (static_cast<long long>((request.length()) > max_client_body_size)
		|| (request.find("\r\n\r\n") != std::string::npos))
		pfd.revents = POLLOUT;
	return (EXIT_SUCCESS);
}

int Server::check_connection(pollfd &pfd) {
	if (pfd.revents & POLLERR || pfd.revents & POLLHUP || pfd.revents & POLLNVAL) {
		shutdown(pfd.fd, SHUT_RDWR);
		close(pfd.fd);
		pfd.fd = -1;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int	Server::accumulate_request(int &client_fd) {
	char 										buffer[100];
	std::map<int, request_handler>::iterator	it;
	long 										bytes;

	bytes = recv(client_fd, buffer, sizeof(buffer), MSG_DONTWAIT);
	if (bytes < 0)
		return (system_call_error(RECV_ERROR));
	it = _requests.find(client_fd);
	(*it).second.buf += std::string(buffer, bytes);
	return (EXIT_SUCCESS);
}

void	Server::delete_invalid_fds() {
	std::vector<pollfd>::iterator it = _pfds.begin();

	while (it != _pfds.end()) {
		if (it->fd == -1)
			it = _pfds.erase(it);
		else
			it++;
	}
}





















//TODO:implement later
//---------------------------------------------------------------------------------------------------------------------

//VIRTUAL HOSTING
int Server::process_request(const Socket &socket, pollfd &poll_fd) {
	(void )poll_fd;
	(void )socket;
	//	bool	socket_is_unique;

//	socket_is_unique = socket.get_is_unique();
//	if (socket_is_unique) {
//		if (resolve_requests(socket, poll_fd) == EXIT_FAILURE)
//			return (EXIT_FAILURE);
//	}
//	else if (serve_on_virtual_host(socket, poll_fd) < 0)
//		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int Server::serve_on_virtual_host(const Socket &socket, pollfd &poll_fd) {
	char				buf[4096];
	int 				client_socket;
	std::string 		domain;
	std::stringstream	ss;
	std::string 		request;
	std::string 		name;
	std::string 		alias;

	name = socket.get_config().get_name();
	alias = socket.get_config().get_alias();

	client_socket = accept(poll_fd.fd, NULL, NULL);
	recv(client_socket, buf, 4096, 0);
	ss << recv(client_socket, buf, 4096, 0);
	request = ss.str();

	//todo: probalby unuseful! but that's how we can get the domain name!
//	struct hostent *host = gethostbyname(alias.c_str());
//	if (host == NULL)
//		std::cout << "Error: Invalid domain name" << std::endl;
//	std::cout << host << std::endl;


	domain = extract_domain(request);
	std::cout << domain << std::endl;
	if (domain == name || domain == alias) {
		char response[] = "Hello from server!";
		send(client_socket, response, sizeof(response), 0);
	}
	close(client_socket);
	return (EXIT_SUCCESS);
}

std::string Server::extract_domain(std::string &request) {
	std::string domain;
	std::stringstream ss(request);
	std::string line;
	while (std::getline(ss, line)) {
		if (line.find("Host:") == 0) {
			domain = line.substr(6);
			break;
		}
	}
	return domain;
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
	return (EXIT_FAILURE);
}

void 	Server::show_connections() {
	print_configurations();
}

void	Server::print_configurations() {
	for (size_t i = 1; i < _sockets.size(); i++)
		std::cout << &_sockets[i].get_config() << std::endl;
}

void 	Server::show_manual() {
	std::cout << "CALLED HELP\n";
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
	return (EXIT_FAILURE);
}

