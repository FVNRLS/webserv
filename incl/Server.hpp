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

class Server {

private:
	Config						*_config;
	std::vector<sockaddr_in>	_serv_addr;
	std::vector<pollfd>			_sockets;
	size_t 						_num_sockets;

	void						set_serv_addr();
	int							init_unblock_sockets();
	int 						bind_socket();
	int							listen_to_connections();
	int 						resolve_requests();
	int 						process_incoming_request(const int &socket_fd, size_t socket_nbr);
	int 						process_cli();
	std::string 				generate_response(const std::string &request);
	std::string					parse_request(const std::string &request);


public:
	Server(Config &server_config);
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

	int run();
};


