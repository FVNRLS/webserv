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
Server::Server(Config &server_config) {
	_config = &server_config;
	_num_sockets = server_config.get_ports().size();
}

Server::Server(const Server &src) {
	*this = src;
}

Server &Server::operator=(const Server &src) { //todo: check after finishing the class
	if (this == &src)
		return (*this);
	_config = src._config;
	_num_sockets = src._num_sockets;
	_serv_addr = src._serv_addr;
	_sockets = src._sockets;
	return (*this);
}

Server::~Server() {}


//MEMBER FUNCTIONS
int Server::run() {
	set_serv_addr();
	if (init_unblock_sockets() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (bind_socket() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (listen_to_connections() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (resolve_requests() == EXIT_FAILURE)
		return (EXIT_FAILURE);

	for (std::vector<pollfd>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
		close(it->fd);
	return (EXIT_SUCCESS);
}

/*
 * setting up the server address for a network communication:
 * 1. clearing the memory of the _serv_addr variable
 * 2. set the address family to AF_INET, which stands for Internet Protocol version 4.
 * 3. set the port number for the server to the first element of the ports array from the _config object,
 * and it uses the htons function to convert the port number to the correct byte order for network transmission.
 * 4. set ip address - server will listen for connections on the available network interface
 * */
void Server::set_serv_addr() {
	u_int32_t	ip_addr;
	size_t 		num_addr;

	num_addr = _config->get_ports().size();
	_serv_addr.resize(num_addr);
	ip_addr = inet_addr(_config->get_ip().c_str());
	for (size_t i = 0; i < num_addr; i++) {
		memset(&_serv_addr[i], 0, sizeof(_serv_addr[i]));
		_serv_addr[i].sin_family = AF_INET;
		_serv_addr[i].sin_port = htons(_config->get_ports()[i]); // server port
		_serv_addr[i].sin_addr.s_addr = ip_addr; // server ip_address
	}
}

/*
 * This function initializes a set of sockets and makes them non-blocking.
 * 1. creates a new variable new_socket of type pollfd and sets the events field to POLLIN.
 * 2. creates a new socket with the socket() function, and assigns the file descriptor to the fd field of the new_socket variable.
 * 3. assigns the new_socket variable to the corresponding element of the _sockets vector.
 * 4. It sets the file descriptor of the last element of _sockets vector to be the standard input file descriptor (STDIN_FILENO)
 * 5. It uses the fcntl() function to set the last element of the _sockets vector (standard input) to non-blocking mode.

 * */
int Server::init_unblock_sockets() {
	size_t 	i;
	pollfd	new_socket = {};

	_sockets.resize(_num_sockets + 1);
	new_socket.events = POLLIN;
	for (i = 0; i != _num_sockets; i++) {
		new_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
		if (new_socket.fd < 0)
			return  (server_error(SOCKET_OPEN_ERROR, *_config, i));
		if (fcntl(new_socket.fd, F_SETFL, fcntl(new_socket.fd, F_GETFL) | O_NONBLOCK) < 0)
			return  (server_error(SOCKET_OPEN_ERROR, *_config, i));
		_sockets[i] = new_socket;
	}
	new_socket.fd = STDIN_FILENO;
	_sockets[i] = new_socket;
	if (fcntl(_sockets[i].fd, F_SETFL, fcntl(_sockets[i].fd, F_GETFL) | O_NONBLOCK) < 0)
		return  (server_error(SOCKET_OPEN_ERROR, *_config, i));
	return (EXIT_SUCCESS);
}

/*
 * 1. _socket: The socket descriptor that was returned by the socket() function.
 * 2. (struct sockaddr *)&_serv_addr: A pointer to a sockaddr_in structure that contains the address and port information.
 * 3. sizeof(_serv_addr): The size of the sockaddr_in structure.
 * specifies the address and port that the socket should use. Once a socket is bound,
 * it can be used to send or receive data.
 * */
int Server::bind_socket() {
	for (size_t i = 0; i < _num_sockets; i++) {
		if (bind(_sockets[i].fd, (struct sockaddr *)&_serv_addr[i], sizeof(_serv_addr[i])) < 0)
			return (server_error(BIND_ERROR, *_config, i));
		std::cout << "bind success on " << inet_ntoa(_serv_addr[i].sin_addr) << ":" << ntohs(_serv_addr[i].sin_port)
		<< std::endl; //todo: DEL
	}
	return (EXIT_SUCCESS);
}

/*
 * Once the server is listening for connections, it can use the accept() function to accept a connection from a client.
 * */
int	Server::listen_to_connections() {
	for (size_t i = 0; i < _num_sockets; i++) {
		if (listen(_sockets[i].fd, MAX_CONNECTIONS) < 0)
			return (server_error(LISTEN_ERROR, *_config, i));
	}
	return (EXIT_SUCCESS);
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
	std::string 	response;
	const char 		*file_path;
	std::ifstream 	file;
	std::string 	body;
	std::string 	requested_path;

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
	response = RESPONSE_HEADER + std::to_string(body.length()) + "\n\n" + body;
	file.close();



	return (response);
}

std::string	Server::parse_request(const std::string &request) {
	return (request);
}

int Server::process_cli() {
	std::ifstream	std_in;
	std::string 	input;

	std_in.open("/dev/stdin");
	input.append((std::istreambuf_iterator<char>(std_in)), std::istreambuf_iterator<char>());

	std::cout << input << std::endl;
	if (input.empty())
		return (EXIT_SUCCESS);
	else if (input == "exit\n")
		return (CLOSE_SERVER_CMD);
	std_in.close();
	return (EXIT_SUCCESS);
}

//todo: complete with client sockets ???
void	Server::exit_server() {
	for (size_t i = 0; i < _num_sockets; i++)
		close(_sockets[i].fd);
	exit(EXIT_SUCCESS);
}