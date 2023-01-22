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
#include "CLI.hpp"
#include "Config.hpp"

class Server {

private:
	std::vector<Socket>			*_sockets;
	std::vector<pollfd>			_poll_fds;
	size_t 						_num_fds;
	CLI							_cli;

	//CORE FUNCTIONS
	int 						process_incoming_request(const int &socket_fd, size_t socket_nbr);
	std::string 				generate_response(const std::string &request, size_t socket_nbr);
	std::string					parse_request(const std::string &request);

	//TERMINAL INTERACTION
	int							process_cli_input();
	void						exit_server();
	int 						terminate_with_error(int);
	void 						show_connections();
	void 						show_manual();

public:
	Server(std::vector<Socket> &sockets);
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

	int 						run();
};




