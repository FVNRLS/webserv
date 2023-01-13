/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
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
	std::string					prefix;
	std::string					root;	//if root empty -> uses the server_root
	std::string					index;
	std::vector<std::string>	methods;
	std::vector<std::string>	scripts;
	long long 					max_client_body_size; // limit on body sent from client;
	std::string					redirect;
	bool 						directory_listing;
	std::string 				limit_client_body_size; //limit_client_body_size - authorization header;
	std::string 				cgi_path; //todo: bonus stuff
};

class Config {

private:
	std::string					_name;
	std::string 				_ip;
	std::vector<long> 			_port;
	std::string 				_root;
	std::vector<std::string>	_methods;
	std::string 				_index;
	long long 					_max_client_body_size;
	std::string 				_error_pages_dir;
	bool 						_autoindex;
	std::string					_redirect;
	std::vector<location>		_locations;

public:
	friend class ConfigParser;

	//BASIC CLASS SETUP
	Config();
	Config(const Config &src);
	Config &operator=(const Config &src);
	friend std::ostream	&operator<<(std::ostream &o, Config *a);
	~Config();
};


