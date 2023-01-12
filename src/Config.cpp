/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 12:56:00 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 12:56:00 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

//BASIC CLASS SETUP
Config::Config() : _max_client_body_size(10), _autoindex(true), _port(0) {}

Config::Config(const Config &src) {
	*this = src;
}

Config &Config::operator=(const Config &src) { //todo: complete!
	if (this == &src)
		return (*this);
	_name = src._name;
	_ip = src._ip;
	_port = src._port;
	_root = src._root;
	_methods = src._methods;
	_index = src._index;
	_max_client_body_size = src._max_client_body_size;
	_error_page = src._error_page;
	_autoindex = src._autoindex;
	_locations = src._locations;
	return (*this);
}

std::ostream	&operator<<(std::ostream &o, Config *s) {

	std::string	s_upper_name;
	std::string	loc_upper_name;

	//PRINTING OF SERVER CONFIGURATIONS
	s_upper_name = s->_name;
	std::transform(s_upper_name.begin(), s_upper_name.end(), s_upper_name.begin(), ::toupper);

	o << std::endl << "\033[31m****** SERVER " << s_upper_name << " *******\033[0m" << std::endl << std::endl;
	o << "\033[33m****** SERVER CONFIGURATION *******\033[0m" << std::endl;
	o << "NAME:						" << s->_name << std::endl;
	o << "IP ADDRESS:					" << s->_ip << std::endl;

	o << "PORTS:						";
	for (int j = 0; j < s->_port.size(); j++) {
		if (j < s->_port.size() - 1)
			o << s->_port[j] << ", ";
		else
			o << s->_port[j] << std::endl;
	}

	o << "ROOT:						" << s->_root << std::endl;

	o << "METHODS:					";
	for (int j = 0; j < s->_methods.size(); j++) {
		if (j < s->_methods.size() - 1)
			o << s->_methods[j] << ", ";
		else
			o << s->_methods[j] << std::endl;
	}
	std::cout << std::endl;

	o << "INDEX:					" << s->_index << std::endl;
	o << "MAX. CLIENT BODY SIZE:		" << s->_max_client_body_size << std::endl;
	o << "ERROR PAGE:					" << s->_error_page << std::endl;
	o << "AUTOINDEX:					" << s->_autoindex << std::endl;
	std::cout << std::endl;

	//PRINTING OF ALL LOCATIONS
	for (int i = 0; i < s->_locations.size(); i++) {
		s_upper_name = s->_locations[i].prefix;
		std::transform(s_upper_name.begin(), s_upper_name.end(), s_upper_name.begin(), ::toupper);

		o << "\033[33m****** LOCATION " << s->_locations[i].prefix << " CONFIGURATION *******\033[0m" << std::endl;
		o << "PREFIX:						" << s->_locations[i].prefix << std::endl;
		o << "ROOT:						" << s->_locations[i].root << std::endl;

		o << "METHODS:					";
		for (int j = 0; j < s->_locations[i].methods.size(); j++) {
			if (j < s->_locations[i].methods.size() - 1)
			 	o << s->_locations[i].methods[j] << ", ";
			else
				o << s->_locations[i].methods[j] << std::endl;
		}
		std::cout << std::endl;


		o << "SCRIPTS:					";
		for (int j = 0; j < s->_locations[i].scripts.size(); j++) {
			if (j < s->_locations[i].scripts.size() - 1)
				o << s->_locations[i].scripts[j] << ", ";
			else
				o << s->_locations[i].scripts[j] << std::endl;
		}
		std::cout << std::endl;

		o << "INDEX:					" << s->_locations[i].index << std::endl;
		o << "MAX. CLIENT BODY SIZE:		" << s->_locations[i].max_client_body_size << std::endl;
		o << "CLIENTS:					" << s->_locations[i].limit_client_body_size << std::endl;
		o << "REDIRECT:					" << s->_locations[i].redirect << std::endl;
		o << "DIRECTORY LISTING:			" << s->_locations[i].directory_listing << std::endl;
		o << "CGI PATH:					" << s->_locations[i].cgi_path << std::endl;
	}
	return (o);
}

Config::~Config() {}

