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
	long long 					max_client_body_size; // limit on body sent from client;
	bool 						directory_listing;
	std::string 				limit_client_body_size; //limit_client_body_size - authorization header;
	std::string 				cgi_path;
	std::vector<std::pair<std::string, std::string> >	redirect;
	std::vector<std::pair<std::string, std::string> >	scripts;
};

class Config {

private:
	std::string					_name;
	std::string 				_ip;
	std::vector<long> 			_ports;
	std::vector<std::string>	_ip_port_comb;
	std::string 				_root;
	std::vector<std::string>	_methods;
	std::string 				_index;
	long long 					_max_client_body_size;
	std::string 				_error_pages_dir;
	std::vector<location>		_locations;
	std::vector<std::pair<std::string, std::string> >	_redirect;

public:
	friend class ConfigParser;

	//BASIC CLASS SETUP
	Config();
	Config(const Config &src);
	Config &operator=(const Config &src);
	friend std::ostream	&operator<<(std::ostream &o, Config *a);
	~Config();

	//GETTERS
	std::string					get_name();
	std::string					get_ip();
	std::vector<long>			get_ports();
	std::string					get_root();
	std::vector<std::string>	get_methods();
	std::string 				get_index();
	long long 					get_max_client_body_size();
	std::string 				get_error_pages_dir();
	std::vector<location>		get_locations();
	std::vector<std::pair<std::string, std::string> >	get_redirect();
};


