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

//BASIC CLASS SETUP
Server::Server() {} //todo: what to do?

Server::Server(std::vector<Config> &server_configs) {
	_config = &server_configs[0]; //todo: just for test of 1 config --> delete afterwards!
}

Server::Server(const Server &src) {
	*this = src;
}

Server &Server::operator=(const Server &src) { //todo: check after finishing the class
	if (this == &src)
		return (*this);
	return (*this);
}

Server::~Server() {}


//MEMBER FUNCTIONS
int Server::run() {
	set_serv_addr();
	if (create_socket() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (bind_socket() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (listen_to_connections() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (accept_requests() == EXIT_FAILURE)
		return (EXIT_FAILURE);

	close(_socket);
	return (EXIT_SUCCESS);
}

/*
 * creates a socket using the Internet Protocol (IP) version 4 address family (AF_INET) and the TCP
 * socket type (SOCK_STREAM). The third argument, 0, is used to specify the protocol and is typically set to 0 to let
 * the system automatically choose the appropriate protocol.
 * */
int Server::create_socket() {
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		return (server_error(SOCKET_OPEN_ERROR, *_config));
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

	ip_addr = inet_addr(_config->get_ip().c_str());
	memset(&_serv_addr, 0, sizeof(_serv_addr));
	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_port = htons(_config->get_ports()[0]); // server port
	_serv_addr.sin_addr.s_addr = ip_addr; // server ip_address
}

/*
 * 1. _socket: The socket descriptor that was returned by the socket() function.
 * 2. (struct sockaddr *)&_serv_addr: A pointer to a sockaddr_in structure that contains the address and port information.
 * 3. sizeof(_serv_addr): The size of the sockaddr_in structure.
 * specifies the address and port that the socket should use. Once a socket is bound,
 * it can be used to send or receive data.
 * */
int Server::bind_socket() {
	if (bind(_socket, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0)
		return (server_error(BIND_ERROR, *_config));
	else
		std::cout << "bind success on " << inet_ntoa(_serv_addr.sin_addr) << ":" << ntohs(_serv_addr.sin_port)
			<< std::endl; //todo: DEL
	return (EXIT_SUCCESS);
}

int Server::connect_to_server() {
	if (connect(_socket, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0)
		return (server_error(CONNECT_ERROR, *_config));
	char buffer[1024] = "Hello, server!";
	send(_socket, buffer, sizeof(buffer), 0);
	return (EXIT_SUCCESS);
}
/*
 * Once the server is listening for connections, it can use the accept() function to accept a connection from a client.
 * */
int	Server::listen_to_connections() {
	if (listen(_socket, MAX_CONNECTIONS) < 0)
		return (server_error(LISTEN_ERROR, *_config));
	return (EXIT_SUCCESS);
}

int Server::accept_requests() {
	while (true) {
		std::string	msg;
		msg = "Message received from client:	";

		// Accept an incoming connection
		struct sockaddr_in cli_addr;
		socklen_t clilen = sizeof(cli_addr);
		int client_socket = accept(_socket, (struct sockaddr *) &cli_addr, &clilen);
		if (client_socket < 0)
			return (server_error(ACCEPT_ERROR, *_config));

		// Read a message from the client
		char buffer[1024];
//		memset(buffer, '\0', sizeof(buffer));
		recv(client_socket, buffer, sizeof(buffer), 0);
		std::cout << buffer << std::endl;

		msg += buffer;
		// Echo the message back to the client
		send(client_socket, msg.c_str(), sizeof(buffer), 0);

		// Clear buffer and close the socket
		close(client_socket);
	}

	return (EXIT_SUCCESS);
}