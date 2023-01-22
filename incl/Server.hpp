/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:36:52 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/12 13:36:52 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
#include "Socket.hpp"
#include "Config.hpp"

class Server {

private:
	std::vector<Socket>			_sockets;
	std::vector<pollfd>			_socket_fds;

	//CORE FUNCTIONS
	int 						resolve_requests();
	int 						process_incoming_request(const int &socket_fd, size_t socket_nbr);
	std::string 				generate_response(const std::string &request);
	std::string					parse_request(const std::string &request);

	//TERMINAL INTERACTION
	void						exit_server();

public:
	Server();
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

	void 						run(std::vector<Socket> &sockets);
};




