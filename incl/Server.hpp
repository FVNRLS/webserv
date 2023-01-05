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

struct	basic {
	int			port;
	std::string	server_name;
	std::string ip;
	std::string root;
	std::string error_page;
	bool 		autoindex;
};

struct	location {
	std::string	directory;
	bool		GET;
	bool		POST;
	bool		DELETE;

	std::string	root;	//if root empty -> uses the server_root
	size_t		index;
	size_t 		lim_cl_body_size; // limit on body sent from client;
	std::string auth_clients; //limit_client_body_size - authorization header;
	std::string cgi_path;
	std::string	redirect;
};

class Server {

private:
	std::string	_name;
	location	_loc_data;

public:
	//BASIC CLASS SETUP
	Server();
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

};
