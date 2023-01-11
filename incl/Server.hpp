/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 12:56:14 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 12:56:14 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

struct	location {
	std::string					path;
	std::string					root;	//if root empty -> uses the server_root
	std::vector<std::string>	methods;
	std::vector<std::string>	scripts;
	size_t						index;
	size_t 						max_client_body_size; // limit on body sent from client;
	std::string 				auth_clients; //limit_client_body_size - authorization header;
	std::string					redirect;

	std::string 				cgi_path; //todo: bonus stuff
};

class Server {

private:
	std::string					_name;
	std::string 				_ip;
	int							_port;
	std::string 				_root;
	std::vector<std::string>	_methods;
	std::string 				_index;
	int 						_max_client_body_size;
	std::string 				_error_page;
	bool 						_autoindex;
	std::vector<location>		_locations;

public:
	friend class Config;

	//BASIC CLASS SETUP
	Server();
	Server(const Server &src);
	Server &operator=(const Server &src);
	friend std::ostream	&operator<<(std::ostream &o, Server *a);
	~Server();
};


