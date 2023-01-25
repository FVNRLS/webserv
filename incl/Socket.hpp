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


class Socket {

private:
	Config						*_config;
	sockaddr_in					_serv_addr;
	pollfd						_socket;
	size_t 						_port;
	bool 						_is_unique;

	//CORE FUNCTIONS
	void						set_serv_addr();
	int							init_unblock_sockets();
	int 						bind_socket();
	int							listen_to_connections();

	//ERROR MANAGEMENT
	int 						socket_error(int error) const;

public:
	Socket();
	Socket(Config &server_config);
	Socket(const Socket &src);
	Socket &operator=(const Socket &src);
	~Socket();

	int 						activate();

	//GETTERS
	Config						&get_config()	const;
	const sockaddr_in			&get_serv_addr() const;
	pollfd						get_pollfd()	const;
	size_t 						get_port()		const;
	bool 						get_is_unique() const;
};
