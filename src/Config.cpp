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
Config::Config() : _config_file(NULL) {}

Config::Config(char *path) : _config_file(path) {}

Config::Config(const Config &src) {
	*this = src;
}

Config &Config::operator=(const Config &src) {
	if (this == &src)
		return (*this);
	_config_file = src._config_file;
	return (*this);
}

Config::~Config() {}

//MEMBER FUNCTIONS
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
		return (EXIT_FAILURE);
	while (std::getline(file, line)) {
		if (line.empty())
			continue;
		else {
			line = trim(line);
			_content += line + '\n';
		}
	}
	file.close();
	return (EXIT_SUCCESS);
}

int 	Config::find_open_brace() {
//	while (pos != )

	return (EXIT_FAILURE);
}


int Config::search_for_server() {
	size_t pos;

	pos = _buf.find_first_of("server");
	if (pos == 0) {
		_blocks.push_back(_buf);
		_buf.clear();
		return (EXIT_SUCCESS);
	}
	else if (pos != std::string::npos)
		return (print_line_error(INVALID_SERVER_DEFINITION, _config_file, _line_num));
	else
		return (NOT_FOUND);
}

int	Config::split_blocks() {

	int 						ret;
	int 						i;
	int 						serv_cnt;
	int 						serv_index;
	size_t 						serv_pos;
	size_t 						closed_brace_pos;

	serv_cnt = 0;
	i = 0;
	while (i < _content.length()) {

		if (_content[i] != NEWLINE)
			_buf += _content[i];
		else if (search_for_server() == EXIT_FAILURE)
				return (EXIT_FAILURE);
		_line_num++;
		i++;
	}
	return (EXIT_SUCCESS);
}

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




