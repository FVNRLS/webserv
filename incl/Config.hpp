/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 12:56:14 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/08 12:52:17 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tools.hpp"

struct	location {
	std::string											prefix;
	std::string											root;	//if root empty -> uses the server_root
	std::string											index;
	std::vector<std::string>							methods;
	long long											max_client_body_size; // limit on body sent from client;
	bool												directory_listing;
	std::string											limit_client_body_size; //limit_client_body_size - authorization header;
	std::string											cgi_path;
	std::vector<std::pair<std::string, std::string> >	redirect;
	std::vector<std::pair<std::string, std::string> >	scripts;
};

class Config {

private:
	friend class										ConfigParser;
	std::string											_name;
	std::string											_alias;
	std::string											_ip;
	std::vector<long>									_ports;
	long												_port;
	std::string											_root;
	std::vector<std::string>							_methods;
	std::string											_index;
	long long											_max_client_body_size;
	std::string											_error_pages_dir;
	std::vector<location>								_locations;
	std::vector<std::pair<std::string, std::string> >	_redirect;

public:
	//BASIC CLASS SETUP
	Config();
	Config(const Config &src);
	Config &operator=(const Config &src);
	~Config();



	//GETTERS
	std::string											get_name() const;
	std::string											get_alias() const;
	std::string											get_ip() const;
	std::vector<long>									get_ports() const;
	long												get_port() const;
	std::string											get_root() const;
	std::vector<std::string>							get_methods() const;
	std::string											get_index() const;
	long long											get_max_client_body_size() const;
	std::string											get_error_pages_dir() const;
	std::vector<location>								get_locations() const;
	std::vector<std::pair<std::string, std::string> >	get_redirect() const;

};

std::ostream	&operator<<(std::ostream &o, Config *s);
