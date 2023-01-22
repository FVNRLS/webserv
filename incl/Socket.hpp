/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 10:35:23 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/22 10:35:23 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include <poll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>

class Socket {

private:
	Config						*_config;
	sockaddr_in					_serv_addr;
	pollfd						_socket;
	size_t 						_port;

	//CORE FUNCTIONS
	void						set_serv_addr();
	int							init_unblock_sockets();
	int 						bind_socket();
	int							listen_to_connections();

	//ERROR MANAGEMENT
	int 						socket_error(int error) const;

public:
	Socket(Config &server_config, size_t port);
	Socket(const Socket &src);
	Socket &operator=(const Socket &src);
	~Socket();

	int 						activate();

	//GETTERS
	pollfd						get_pollfd()	const;
	Config						&get_config()	const;
	size_t 						get_port()		const;
};
