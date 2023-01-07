/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:27:52 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 13:27:52 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

//BASIC CLASS SETUP
Config::Config() : _config_file(NULL), _server_mode(false), _line_num(1), _pos(0), _i(0), _serv_cnt(0), _serv_def_start(0), _serv_def_end(0) {}

Config::Config(char *path) : _config_file(path), _server_mode(false), _line_num(1), _pos(0), _i(0), _serv_cnt(0), _serv_def_start(0), _serv_def_end(0) {}

Config::Config(const Config &src) { *this = src; }

Config &Config::operator=(const Config &src) {
	if (this == &src)
		return (*this);
	_config_file = src._config_file;
	return (*this);
}

Config::~Config() {}

//MEMBER FUNCTIONS
int	Config::parse(std::vector<Server> &servers, const char *config) {
	_serv = &servers;
	if (check_extension() == EXIT_FAILURE)
		return (print_error(INVALID_EXTENSION, config));
	if (read_conf_file() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (split_blocks() == EXIT_FAILURE)
		return (EXIT_FAILURE);

//	std::cout << _content << std::endl;
	return (EXIT_SUCCESS);
}

int	Config::check_extension() {
	std::size_t dot_pos;

	dot_pos = _config_file.find_last_of('.');
	if (_config_file.empty() || dot_pos == std::string::npos)
		return (EXIT_FAILURE);
	if (_config_file.substr(dot_pos) == ".conf")
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	Config::read_conf_file() {
	std::ifstream	file;
	std::string		line;

	file.open(_config_file);
	if (!file.is_open() || file.fail())
		return (print_error(BAD_PERMISSIONS, _config_file));
	while (std::getline(file, line)) {
		line = trim(line);
		_content += line + '\n';
	}
	file.close();
	return (EXIT_SUCCESS);
}

int	Config::split_blocks() {

	size_t	len;
	int 	serv_index;
	size_t 	serv_pos;

	len = _content.length();
	_i = 0;
	while (_i < len) {

		while (_content[_i] != NEWLINE && _i < len) {
			_buf += _content[_i];
			_i++;
		}
		if (search_for_server() == EXIT_FAILURE)
				return (EXIT_FAILURE);
		_buf.clear();
		_line_num++;
		_i++;
	}
	return (EXIT_SUCCESS);
}

int Config::search_for_server() {
	_pos = _buf.find("server");
	if (_pos == 0) {
		if (_server_mode && find_open_brace() == EXIT_SUCCESS)
			return (print_line_error(REDEFINITION_OF_SERVER, _config_file, _line_num));
		else if (!_server_mode && find_open_brace() == EXIT_FAILURE)
			return (print_line_error(INVALID_SERVER_DEFINITION, _config_file, _line_num));
		if (check_closed_braces() == EXIT_FAILURE)
			return (print_line_error(BRACES_NOT_CLOSED, _config_file, _line_num));
		_server_mode = true;
		_blocks.push_back(_buf);
		return (EXIT_SUCCESS);
	}
	else if (!_server_mode && _buf[0] != '\0')
		return (print_line_error(INVALID_CHARACTERS_FOUND, _config_file, _line_num));
	return (NOT_FOUND);
}

int Config::find_open_brace() {
	size_t	j;
	size_t	len;

	j = _i - _buf.length() + 7; //offset from the start of the word 'server'
	len = _content.length();
	while (j < len) {
		if (_content[j] != SPACE && _content[j] != NEWLINE && _content[j] != OPEN_CURLY_BRACE)
			return (EXIT_FAILURE);
		else if (_content[j] == OPEN_CURLY_BRACE) {
			_serv_def_start = j;
			return (EXIT_SUCCESS);
		}
		j++;
	}
	return (EXIT_FAILURE);
}

int	Config::check_closed_braces() {
	size_t	open_braces;
	size_t	closed_braces;
	size_t	j;

	_serv_def_end = _line_num;
	open_braces = 1;
	closed_braces = 0;
	j = _serv_def_start + 1;
	while (_content[j] < _content.length()) {
		if (_content[j] == NEWLINE)
			_serv_def_end++;

		if (_content[j] == OPEN_CURLY_BRACE)
			open_braces++;
		else if (_content[j] == CLOSED_CURLY_BRACE)
			closed_braces++;

		if (open_braces == closed_braces)
			return (EXIT_SUCCESS);
		j++;
	}
	return (EXIT_FAILURE);
}




