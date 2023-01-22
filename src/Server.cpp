/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:36:37 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/12 13:36:37 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Config.hpp"
std::string 				trim(std::string &s);
std::vector<std::string>	split(std::string &s, char sep);

//BASIC CLASS SETUP
Server::Server() {}

Server::Server(const Server &src) {
	*this = src;
}

Server &Server::operator=(const Server &src) { //todo: check after finishing the class
	if (this == &src)
		return (*this);
	_sockets = src._sockets;
	_socket_fds = src._socket_fds;
	return (*this);
}

Server::~Server() {}


//MEMBER FUNCTIONS
void 	Server::run(std::vector<Socket> &sockets) {

}


/*
 * 1. The function runs in an infinite loop.
 * 2. Within the loop, it calls the poll() function on the _sockets vector, with a timeout of -1
 * 		(meaning it will wait indefinitely) to wait for incoming connections.
 * 3. It checks the return value of poll() for errors.
 * 		If an error occurs, it checks if the error is EINTR -> indicates that the system call was interrupted by a signal.
 * 		If the error is EINTR, the function continues to the next iteration of the loop.
 * 		If the error is not EINTR, the function calls server_error() and returns an error code.
 * 4. It iterates over the _sockets vector, checking for the POLLIN event on each file descriptor.
 * 		If the POLLIN event is set, it calls the process_incoming_request() function on the file descriptor.
 * 5. After handling all the _sockets in the vector, it checks if there is any input from the command-line interface (CLI).
 * 		If there is input, it calls the process_cli() function. If the process_cli() function returns an error,
 * 		the function returns an error code.
 * 6. The function exits the infinite loop and returns EXIT_SUCCESS to indicate that it completed successfully.
 * */
int Server::resolve_requests() {
	size_t 	i;

	while (true) {
		if (poll(&_sockets[0], _sockets.size(), -1) < 0) {
			if (errno == EINTR)
				continue;
			return (server_error(POLL_ERROR, _config[0], 0));
		}
		for (i = 0; i < _num_sockets; i++) {
			if (_sockets[i].revents & POLLIN) {
				if (process_incoming_request(_sockets[i].fd, i) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
		}
		if (_sockets[i].revents & POLLIN) { //todo: write terminal input handler!
			if (process_cli() == CLOSE_SERVER_CMD)
				exit_server();
		}
	}
	return (EXIT_SUCCESS);
}

int Server::process_incoming_request(const int &socket_fd, size_t socket_nbr) {
		struct sockaddr_in 	cli_addr;
		socklen_t 			client_len;
		int 				client_socket;
		char 				client_buf[1024];
		std::string 		request;
		std::string 		content_length;
		std::string 		response;


		// Accept an incoming connection
		client_len = sizeof(cli_addr);
		client_socket = accept(socket_fd, (struct sockaddr *) &cli_addr, &client_len);
		if (client_socket < 0)
			return (server_error(ACCEPT_ERROR, *_config, socket_nbr));

		// Read a message from the client
		recv(client_socket, client_buf, sizeof(client_buf), 0);
		request = client_buf;
		std::cout << request << std::endl;

		// Echo the message back to the client

		response = generate_response(request);
		if (response.empty()) {
			close(client_socket);
			return (EXIT_FAILURE);
		}

		send(client_socket, response.c_str(), response.length(), 0);

		// Clear buffer and close the socket
		close(client_socket);

	return (EXIT_SUCCESS);
}

std::string Server::generate_response(const std::string &request) {
	std::string 		response;
	const char 			*file_path;
	std::ifstream 		file;
	std::string 		body;
	std::string 		requested_path;
	std::stringstream 	body_len;

	requested_path = parse_request(request); //todo: cont!

	file_path = _config->get_index().c_str();
	std::cout << file_path << std::endl;

	if (access(file_path, F_OK) < 0) {
		parsing_error_basic(NO_FILE, file_path);
		return ("");
	}
	file.open(file_path);
	if (!file.is_open() || file.fail()) {
		parsing_error_basic(BAD_PERMISSIONS, file_path);
		return ("");
	}
	body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	body_len << body.length();
	response = RESPONSE_HEADER + body_len.str() + "\n\n" + body;
	file.close();

	return (response);
}

std::string	Server::parse_request(const std::string &request) {
	return (request);
}


//todo: complete with client sockets ???
void	Server::exit_server() {
	for (size_t i = 0; i < _num_sockets; i++)
		close(_sockets[i].fd);
	exit(EXIT_SUCCESS);
}