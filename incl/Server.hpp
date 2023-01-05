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

typedef struct s_conf_data {
	std::string	directory;
	bool		GET;
	bool		POST;
	bool		DELETE;

	bool		dir_listing;
	std::string	def_file;
	std::string	root;	//if root empty -> uses the server_root
	size_t		id;
	size_t		upload_size;
	bool		upload_size_bool;
	std::string	redirect;
} t_conf_data;

class Server {

private:
	std::string	_name;
	t_conf_data	_data;

public:
	//BASIC CLASS SETUP
	Server();
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

};
