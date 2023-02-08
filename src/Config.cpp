/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoomen <hoomen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 12:56:00 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/08 10:32:30 by hoomen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

//BASIC CLASS SETUP
Config::Config() : _ports(0), _max_client_body_size(UINT32_MAX), _is_unique(true) {}

Config::Config(const Config &src) {
	*this = src;
}

Config &Config::operator=(const Config &src) {
	if (this == &src)
		return (*this);
	_name = src._name;
	_alias = src._alias;
	_ip = src._ip;
	_ports = src._ports;
	_port = src._port;
	_root = src._root;
	_methods = src._methods;
	_index = src._index;
	_max_client_body_size = src._max_client_body_size;
	_error_pages_dir = src._error_pages_dir;
	_redirect = src._redirect;
	_locations = src._locations;
	_is_unique = src._is_unique;
	return (*this);
}

std::ostream	&operator<<(std::ostream &o, Config *s) {

	std::string	s_upper_name;
	std::string	loc_upper_name;

	//PRINTING OF SERVER CONFIGURATIONS
	s_upper_name = s->get_name();
	std::transform(s_upper_name.begin(), s_upper_name.end(), s_upper_name.begin(), ::toupper);

	o << std::endl << "\033[31m****** SERVER " << s_upper_name << " *******\033[0m" << std::endl << std::endl;
	o << "\033[33m****** SERVER CONFIGURATION *******\033[0m" << std::endl;
	o << std::left << std::setw(28) << "NAME:" << s->get_name() << std::endl;
	o << std::left << std::setw(28) << "ALIAS:" << s->get_alias() << std::endl;
	o << std::left << std::setw(28) << "IP ADDRESS:" << s->get_ip() << std::endl;
	o << std::left << std::setw(28) << "PORT:" << s->get_port() << std::endl;
	o << std::left << std::setw(28) << "ROOT:" << s->get_root() << std::endl;

	if (!s->get_methods().empty()) {
		o << std::left << std::setw(28) << "METHODS:";
		for (size_t j = 0; j < s->get_methods().size(); j++) {
			if (j < s->get_methods().size() - 1)
				o << s->get_methods()[j] << ", ";
			else
				o << s->get_methods()[j] << std::endl;
		}
	}

	if (!s->get_index().empty())
		o << std::left << std::setw(28) << "INDEX:" << s->get_index() << std::endl;
	o << std::left << std::setw(28) << "MAX. CLIENT BODY SIZE:" << s->get_max_client_body_size() << std::endl;
	if (!s->get_error_pages_dir().empty())
		o << std::left << std::setw(28) << "ERROR PAGE:" << s->get_error_pages_dir() << std::endl;

	if (!s->get_redirect().empty()) {
		o << std::left << std::setw(28) << "REDIRECTS:";
		for (size_t j = 0; j < s->get_redirect().size(); j++) {
			if (j == 0)
				o << s->get_redirect()[j].first << " = " << s->get_redirect()[j].second << std::endl;
			else
				o << "							" << s->get_redirect()[j].first << " = " << s->get_redirect()[j].second
				  << std::endl;
		}
	}

	std::cout << std::endl;

	//PRINTING OF ALL LOCATIONS
	for (size_t i = 0; i < s->get_locations().size(); i++) {
		s_upper_name = s->get_locations()[i].prefix;
		std::transform(s_upper_name.begin(), s_upper_name.end(), s_upper_name.begin(), ::toupper);

		o << "\033[33m****** LOCATION " << s->get_locations()[i].prefix << " CONFIGURATION *******\033[0m" << std::endl;
		o << std::left << std::setw(28) << "PREFIX:" << s->get_locations()[i].prefix << std::endl;
		o << std::left << std::setw(28) << "ROOT:" << s->get_locations()[i].root << std::endl;

		if (!s->get_locations()[i].methods.empty()) {
			o << std::left << std::setw(28) << "METHODS:";
			for (size_t j = 0; j < s->get_locations()[i].methods.size(); j++) {
				if (j < s->get_locations()[i].methods.size() - 1)
					o << s->get_locations()[i].methods[j] << ", ";
				else
					o << s->get_locations()[i].methods[j] << std::endl;
			}
		}

		if (!s->get_locations()[i].scripts.empty()) {
			o << std::left << std::setw(28) << "SCRIPTS:";
			for (size_t j = 0; j < s->get_locations()[i].scripts.size(); j++) {
				if (j == 0)
					o << s->get_locations()[i].scripts[j].first << " = "
						<< s->get_locations()[i].scripts[j].second << std::endl;
				else
					o <<  "							" << s->get_locations()[i].scripts[j].first <<
					" = " << s->get_locations()[i].scripts[j].second << std::endl;
			}
		}

		o << std::left << std::setw(28) << "INDEX:" << s->get_locations()[i].index << std::endl;
		o << std::left << std::setw(28) << "MAX. CLIENT BODY SIZE:"
			<< s->get_locations()[i].max_client_body_size << std::endl;
		if (!s->get_locations()[i].redirect.empty())
			o << std::left << std::setw(28) << "REDIRECT:" << s->get_locations()[i].redirect[0].first
				<< " = " << s->get_locations()[i].redirect[0].second << std::endl;
		o << std::left << std::setw(28) << "DIRECTORY LISTING:" << s->get_locations()[i].directory_listing << std::endl;
		o << std::left << std::setw(28) << "CGI PATH:" << s->get_locations()[i].cgi_path << std::endl << std::endl;
	}
	return (o);
}

Config::~Config() {}



//GETTERS
std::string Config::get_name() const {
	return (_name);
}

std::string	Config::get_alias() const {
	return (_alias);
}

std::string	Config::get_ip() const {
	return (_ip);
}

std::vector<long>	Config::get_ports() const {
	return (_ports);
}

long	Config::get_port() const {
	return (_port);
}

std::string	Config::get_root() const {
	return (_root);
}

std::vector<std::string>	Config::get_methods() const {
	return (_methods);
}

std::string	Config::get_index() const {
	return (_index);
}

long long	Config::get_max_client_body_size() const {
	return (_max_client_body_size);
}

std::string	Config::get_error_pages_dir() const {
	return (_error_pages_dir);
}

std::vector<location>	Config::get_locations() const {
	return (_locations);
}

bool Config::get_is_unique() const {
	return (_is_unique);
}

std::vector<std::pair<std::string, std::string> >	Config::get_redirect() const {
	return (_redirect);
}
