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
int Server::start() {
	set_serv_addr();
	if (create_socket() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (bind_socket() == EXIT_FAILURE)
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

int Server::bind_socket() {
	if (bind(_socket, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0)
		return (server_error(BIND_ERROR, *_config));
	else
		std::cout << "bind success on port " << ntohs(_serv_addr.sin_port) << std::endl; //todo: DEL
	return (EXIT_SUCCESS);
}