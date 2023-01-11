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
	std::string						_config_file;
	std::string 					_content;
	std::string						_buf;
	bool 							_serv_mode;
	size_t 							_line_num;
	size_t 							_pos;
	size_t 							_conf_pos;
	size_t 							_serv_cnt;
	size_t 							_serv_def_start;
	size_t 							_serv_def_end;
	int 							_i_serv;
	int 							_i_loc;
	std::vector<char>				_spec_chars;
	std::vector<std::string>		_valid_members;
	std::vector<std::string>		_spec_valid_members;
	std::vector<Server> 			*_serv;
	std::vector<std::string>		_serv_blocks;
	std::vector<std::string>		_tokens;
	std::vector<int (Config::*)()> 	_func_tab;

	//PRIVATE MEMBER FUNCTIONS
	int								check_extension();
	int								read_conf_file();
	size_t 							get_line_num(std::string &str);
	int								split_in_server_blocks();
	int								search_for_server();
	void							ignore_comments(size_t len);
	int 							find_in_spec_chars(char c) const;
	int 							find_in_valid_members(std::string &s) const;
	int 							find_open_brace();
	int 							check_closed_braces();

	int								extract_servers();
	void							replace_open_braces(std::vector<std::string> &v);
	void 							create_servers();
	int 							extract_server_block(int i);
	void							set_mode();

	int								set_server_parameter();
	int								get_func_index();
	int 							set_server_name();
	int 							set_ip_address();
	int 							set_port();
	int 							set_root();
	int 							set_index();
	int 							set_max_client_body_size();
	int 							set_error_pages();
	int 							set_redirection();
	int 							set_allowed_methods();
	int 							set_allowed_scripts();
	int 							set_directory_listing();
	int 							set_autoindex();


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
