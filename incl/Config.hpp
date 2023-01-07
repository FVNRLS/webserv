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
	std::string					_config_file;
	std::string 				_content;
	std::string					_buf;
	bool 						_server_mode;
	size_t 						_line_num;
	size_t 						_pos;
	size_t 						_i;
	size_t 						_serv_cnt;
	std::vector<Server> 		*_serv;
	std::vector<std::string>	_blocks;  // Vector to store blocks of code


	//PRIVATE MEMBER FUNCTIONS
	int							check_extension();
	int							read_conf_file();
	int							split_blocks();
	int							search_for_server();
	int 						find_open_brace();

public:
	//BASIC CLASS SETUP
	Config();
	Config(char *path);
	Config(const Config &src);
	Config &operator=(const Config &src);
	~Config();

	//PUBLIC MEMBER FUNCTIONS
	int	parse(std::vector<Server> &servers, const char *config);
};
