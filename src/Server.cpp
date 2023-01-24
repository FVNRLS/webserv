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
	_num_fds = sockets.size() + 1;
	for (size_t i = 0; i < sockets.size(); i++)
		_poll_fds.push_back(sockets[i].get_pollfd());
}

Server::~Server() {}

//MEMBER FUNCTIONS

/*
 * 1. The function runs in an infinite loop.
 * 2. Within the loop, it calls the poll() function on the _sockets vector, with a timeout of -1
 * 		(meaning it will wait indefinitely) to wait for incoming connections.
 * 3. It iterates over the _sockets vector, checking for the POLLIN event on each file descriptor.
 * 		If the POLLIN event is set, it calls the process_request() function on the file descriptor.
 * 4. After handling all the _sockets in the vector, it checks if there is any input from the command-line interface (CLI).
 * 		If there is input, it calls the process_cli() function. If the process_cli() function returns an error,
 * 		the function returns an error code.
 * 5. The function exits the infinite loop and returns EXIT_SUCCESS to indicate that it completed successfully.
 * */
int Server::run() {
	size_t i;

//	print_configurations();

	if (_cli.start() == EXIT_FAILURE)
		exit_server();
	_poll_fds.push_back(_cli.get_pollfd());

	while (true) {
		if (poll(&_poll_fds[0], _num_fds, TIMEOUT) < 0)
			return (terminate_with_error(server_error(POLL_ERROR, _sockets.front())));
		for (i = 0; i < _sockets.size(); i++) {

			if (_poll_fds[i].revents & POLLIN) {
				if (process_request(_sockets[i], _poll_fds[i]) == EXIT_FAILURE)
					return (terminate_with_error(EXIT_FAILURE));
			}
		}
		if (_poll_fds[i].revents & POLLIN) {
			if (process_cli_input() == EXIT_FAILURE)
				return EXIT_FAILURE;
		}
	}

}

int Server::process_request(const Socket &socket, pollfd &poll_fd) {
	bool	socket_is_unique;

	socket_is_unique = socket.get_is_unique();
	if (socket_is_unique) {
		if (serve_on_port(socket, poll_fd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (serve_on_virtual_host(socket, poll_fd) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int Server::serve_on_port(const Socket &socket, pollfd &poll_fd) {
	std::string 		content_length;
	std::string 		response;
	struct sockaddr 	client_addr = {};
	std::string 		request;
	socklen_t 			client_len;
	int 				client_socket;

	client_len = sizeof(client_addr);
	client_socket = accept(poll_fd.fd, &client_addr, &client_len);
	if (client_socket < 0)
		return (server_error(ACCEPT_ERROR, socket));
	if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
		return (EXIT_FAILURE);

//	request = get_request(client_socket);
//	if (request.empty())
//		return (server_error(RECV_ERROR, socket));

	char 	buffer[1024];
	long 	bytes;
	while ((bytes = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
		request += std::string(buffer, bytes);
	if(bytes < 0) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			return (server_error(RECV_ERROR, socket));
		}
	}
	std::cout << request << std::endl;

	response = generate_response(request);
	send(client_socket, response.c_str(), response.length(), 0);
	close(client_socket);
	return (EXIT_SUCCESS);
}

std::string Server::get_request(int &socket) {
	char		client_buf[1024];
	std::string request;

	if (recv(socket, client_buf, sizeof(client_buf), 0) <= 0)
		return (request);
	request = client_buf;
	return (request);
}

std::string Server::generate_response(const std::string &request) {
	std::string 		response;
	const char 			*file_path;
	std::ifstream 		file;
	std::string 		body;
	std::string 		requested_path;
	std::stringstream 	body_len;
	(void) request;

//	requested_path = get_requested_path(request); //todo: cont!

	file_path = DEFAULT_INDEX_PAGE.c_str();
//	std::cout << file_path << std::endl;

	if (access(file_path, F_OK) < 0) {
		server_error(ERROR_404, _sockets.front());
		return (""); //todo: add error.html
	}
	file.open(file_path);
	if (!file.is_open() || file.fail()) {
		server_error(ACCESS_DENIED, _sockets.front());
		return (""); //todo: add error.html
	}
	body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	body_len << body.length();
	response = RESPONSE_HEADER + body_len.str() + "\n\n" + body;
	file.close();

	return (response);
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

std::string	Server::get_requested_path(const std::string &request) {

	return (request);
}


//TERMINAL FUNCTIONS
int	Server::process_cli_input() {
	switch (_cli.check_input()) {
		case CLI_EMPTY:
			return EXIT_SUCCESS;
		case CLI_FAIL:
			return terminate_with_error(server_error(CLI_ERROR, _sockets.front()));
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

//todo: complete with client sockets ???
void	Server::exit_server() {
	for (size_t i = 0; i < _sockets.size(); i++)
		close(_poll_fds[i].fd);
	exit(EXIT_SUCCESS);
}

int		Server::terminate_with_error(int) {
	for (size_t i = 0; i < _sockets.size(); i++)
		close(_poll_fds[i].fd);
	return (EXIT_FAILURE);
}

void 	Server::show_connections() {
	std::cout << "CALLED LS\n";
}

void	Server::print_configurations() {
	for (size_t i = 0; i < _sockets.size(); i++)
		std::cout << &_sockets[i].get_config() << std::endl;
}

void 	Server::show_manual() {
	std::cout << "CALLED HELP\n";
}


int Server::server_error(int error, const Socket &socket) {
	switch(error) {
		case POLL_ERROR:
			std::cerr << "Error: failed polling sockets" << std::endl;
			break;
		case ACCEPT_ERROR:
			std::cerr << "Error: failed to accept connection on port " << socket.get_port() << std::endl;
			break;
		case CLI_ERROR:
			std::cerr << "Error: command line interface failed" << std::endl;
			break;
		case RECV_ERROR:
			std::cerr << "Error: failed to receive message on port " << socket.get_port() << std::endl;
			break;
		case ERROR_404:
			std::cerr << "Error: Page not found!" << std::endl;
			break;
		case ACCESS_DENIED:
			std::cerr << "Error: Access Denied!" << std::endl;
			break;
		default:
			std::cerr << "Server: unknown error" << std::endl;
	}
	return (EXIT_FAILURE);
}


