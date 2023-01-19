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
//	std::vector<Config>	*_conf;
	Config				*_config; //todo: just for test of 1 config --> delete afterwards!
	struct sockaddr_in	_serv_addr;
	int 				_socket;


	void	set_serv_addr();
	int		create_socket();
	int 	bind_socket();
	int 	connect_to_server();
	int		listen_to_connections();
	int 	accept_requests();



public:
	Server();
	Server(std::vector<Config> &server_configs);
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

	int run();
};


