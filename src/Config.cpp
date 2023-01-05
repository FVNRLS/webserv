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
Config::Config() : _path(NULL), _is_parsed(false) {}

Config::Config(char *path) : _path(path), _is_parsed(false) {}

Config::Config(const Config &src) {
	*this = src;
}

Config &Config::operator=(const Config &src) {
	if (this == &src)
		return (*this);
	_path = src._path;
	return (*this);
}

Config::~Config() {}

//GETTERS / SETTERS
bool	Config::get_is_parsed() const {
	return (_is_parsed);
}

//MEMBER FUNCTIONS
bool	Config::check_extension() {
	std::size_t dot_pos;

	dot_pos = _path.find_last_of('.');
	if (_path.empty() || dot_pos == std::string::npos)
		return (false);
	if (_path.substr(dot_pos) == ".conf")
		return (true);
	return (false);
}

int	Config::count_servers() {
	std::string		line;
	std::ifstream	config_file(_path);
	int 			i = 0;

	if (!config_file.is_open() || config_file.fail())
		return (-1);
	while (std::getline(config_file, line)) {
		if (line.find("server") == 0)
			i++;
	}
	return (i);
}

//todo: refactor to scan the file and to find open curly brackets
void	find_closed_brakets() {
	std::ifstream config_file("config.conf");
	std::stringstream config_stream;
	config_stream << config_file.rdbuf();
	std::string config = config_stream.str();

	// Search for the pattern "server {" followed by some text and a closing curly bracket
	size_t server_pos = config.find("server {");
	if (server_pos != std::string::npos) {
		size_t closing_bracket_pos = config.find('}', server_pos);
		if (closing_bracket_pos != std::string::npos) {
			std::string text = config.substr(server_pos + 8, closing_bracket_pos - server_pos - 8);
			std::cout << "Text between 'server {' and '}': " << text << std::endl;
		}
	}
}

void	Config::parse(Server &servers) {

	_is_parsed = true; //only if the parsing was successful!

}




