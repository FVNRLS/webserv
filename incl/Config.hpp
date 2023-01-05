/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:28:14 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 13:28:14 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
#include "Server.hpp"

class Config {

private:
	std::string	_path;
	bool 		_is_parsed;

public:
	//BASIC CLASS SETUP
	Config();
	Config(char *path);
	Config(const Config &src);
	Config &operator=(const Config &src);
	~Config();

	//GETTERS / SETTERS
	bool		get_is_parsed() const;

	//MEMBER FUNCTIONS

	std::string get_extension(const std::string &file_name);
	bool		check_extension();
	int 		count_servers();
	void		parse(Server &servers);

};
