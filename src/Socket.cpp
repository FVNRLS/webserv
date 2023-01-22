/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 10:35:56 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/22 10:35:56 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

//BASIC CLASS SETUP
Socket::Socket(Config &server_config, size_t port) {
	_config = &server_config;
	_port = port;
}

Socket::Socket(const Socket &src) {
	*this = src;
}

Socket &Socket::operator=(const Socket &src) { //todo: check after finishing the class
	if (this == &src)
		return (*this);
	_config = src._config;
	_serv_addr = src._serv_addr;
	_socket = src._socket;
	_port = src._port;
	return (*this);
}

Socket::~Socket() {}


//MEMBER FUNCTIONS
int	Socket::activate() {
	set_serv_addr();
	if (init_unblock_sockets() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (bind_socket() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (listen_to_connections() == EXIT_FAILURE)
		return (EXIT_FAILURE);

//	for (std::vector<pollfd>::iterator it = _socket_fds.begin(); it != _socket_fds.end(); it++)
//		close(it->fd);
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
void Socket::set_serv_addr() {
	u_int32_t	ip_addr;

	ip_addr = inet_addr(_config->get_ip().c_str());
	memset(&_serv_addr, 0, sizeof(_serv_addr));
	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_port = htons(_config->get_ports()[_port]); // server port
	_serv_addr.sin_addr.s_addr = ip_addr; // server ip_address
}

/*
 * This function initializes a set of sockets and makes them non-blocking.
 * 1. creates a new variable new_socket of type pollfd and sets the events field to POLLIN.
 * 2. creates a new socket with the socket() function, and assigns the file descriptor to the fd field of the new_socket variable.
 * 3. assigns the new_socket variable to the corresponding element of the _socket_fds vector.
 * 4. It sets the file descriptor of the last element of _socket_fds vector to be the standard input file descriptor (STDIN_FILENO)
 * 5. It uses the fcntl() function to set the last element of the _socket_fds vector (standard input) to non-blocking mode.

 * */
int Socket::init_unblock_sockets() {
	size_t 	i;

	_socket.events = POLLIN;
	_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket.fd < 0)
		return  (server_error(SOCKET_OPEN_ERROR, _config, i));
	if (fcntl(_socket.fd, F_SETFL, fcntl(_socket.fd, F_GETFL) | O_NONBLOCK) < 0)
		return  (server_error(SOCKET_OPEN_ERROR, _config, i));
	return (EXIT_SUCCESS);
}

/*
 * 1. _socket: The socket descriptor that was returned by the socket() function.
 * 2. (struct sockaddr *)&_serv_addr: A pointer to a sockaddr_in structure that contains the address and port information.
 * 3. sizeof(_serv_addr): The size of the sockaddr_in structure.
 * specifies the address and port that the socket should use. Once a socket is bound,
 * it can be used to send or receive data.
 * */
int Socket::bind_socket() {
	if (bind(_socket.fd, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0)
		return (server_error(BIND_ERROR, _config, _port));
	std::cout << "bind success on " << inet_ntoa(_serv_addr.sin_addr) << ":" << ntohs(_serv_addr.sin_port)
			  << std::endl; //todo: DEL
	return (EXIT_SUCCESS);
}

/*
 * Once the server is listening for connections, it can use the accept() function to accept a connection from a client.
 * */
int	Socket::listen_to_connections() {
	if (listen(_socket.fd, MAX_CONNECTIONS) < 0)
		return (server_error(LISTEN_ERROR, _config, _port));
	return (EXIT_SUCCESS);
}


//GETTERS
pollfd	Socket::get_socket() {
	return (_socket);
}

Config	Socket::get_config() {
	return (*_config);
}

