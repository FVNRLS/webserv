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
	bool 						_serv_mode;
	size_t 						_line_num;
	size_t 						_pos;
	size_t 						_conf_pos;
	size_t 						_serv_cnt;
	size_t 						_serv_def_start;
	size_t 						_serv_def_end;
	int 						_i_serv;
	int 						_i_loc;
	std::vector<char>			_spec_chars;
	std::vector<std::string>	_valid_members;
	std::vector<Server> 		*_serv;
	std::vector<std::string>	_serv_blocks;
	std::vector<std::string>	_tokens;

	//PRIVATE MEMBER FUNCTIONS
	int							check_extension();
	int							read_conf_file();
	size_t 						get_line_num(std::string &str);
	int							split_in_server_blocks();
	int							search_for_server();
	void						ignore_comments(size_t len);
	int 						find_in_spec_chars(char c) const;
	int 						find_in_valid_members(std::string &s) const;
	int 						find_open_brace();
	int 						check_closed_braces();
	int							extract_servers();
	void						replace_open_braces(std::vector<std::string> &v);
	void 						create_servers();
	int 						extract_server_block(int i);
	void 						replace_newlines_with_spaces();
	void						set_mode();

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
