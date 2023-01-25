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
Socket::Socket(Config &server_config) {
	_config = &server_config;
	_port = _config->get_port();
	_is_unique = _config->get_is_unique();
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
	_is_unique = src._is_unique;
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
	_serv_addr.sin_port = htons(_config->get_port()); // server port
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
	std::string server_name = _config->get_name();
	std::string server_alias = _config->get_alias();

	_socket.fd = socket(AF_INET, SOCK_STREAM, 0);;
	if (_socket.fd < 0)
		return  (socket_error(SOCKET_OPEN_ERROR));

	int on = 1;
	setsockopt(_socket.fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (fcntl(_socket.fd, F_SETFL, O_NONBLOCK) < 0)
		return (socket_error(SOCKET_OPEN_ERROR));

	//virtual part
	if (!_is_unique) { //todo: find out, what is right: SO_REUSEADDR or SO_REUSEPORT ????
		if (setsockopt(_socket.fd, SOL_SOCKET, SO_REUSEADDR, server_name.c_str(), server_name.length()) < 0)
			return (socket_error(BIND_ERROR));
		if (setsockopt(_socket.fd, SOL_SOCKET, SO_REUSEADDR, server_alias.c_str(), server_alias.length()) < 0)
			return (socket_error(BIND_ERROR));
	}
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
	if (bind(_socket.fd, reinterpret_cast<struct sockaddr *>(&_serv_addr), sizeof(_serv_addr)) < 0)
		return (socket_error(BIND_ERROR));
	std::cout << "bind success on " << inet_ntoa(_serv_addr.sin_addr) << ":" << ntohs(_serv_addr.sin_port) << std::endl;
	return (EXIT_SUCCESS);
}

/*
 * Once the server is listening for connections, it can use the accept() function to accept a connection from a client.
 * */
int	Socket::listen_to_connections() {
	if (listen(_socket.fd, MAX_CONNECTIONS) < 0)
		return (socket_error(LISTEN_ERROR));
	return (EXIT_SUCCESS);
}

//GETTERS
pollfd	Socket::get_pollfd() const {
	return (_socket);
}

Config &Socket::get_config() const {
	return (*_config);
}

const sockaddr_in	&Socket::get_serv_addr() const {
	return (_serv_addr);
}

size_t 	Socket::get_port()	const {
	return _port;
}

bool	Socket::get_is_unique() const {
	return (_is_unique);
}

//ERROR MANAGEMENT
int Socket::socket_error(int error) const {
	switch(error) {
		case SOCKET_OPEN_ERROR:
			std::cerr << "Error: failed to open socket" << std::endl;
			break;
		case BIND_ERROR:
			std::cerr << "Error: failed to bind socket on " << _config->get_ip()  << ":" << _port << std::endl;
			break;
		case LISTEN_ERROR:
			std::cerr << "Error: failed to listen to the socket on port " << get_port() << std::endl;
			break;
		default:
			std::cerr << "Socket: unknown error" << std::endl;
	}
	return (EXIT_FAILURE);
}
