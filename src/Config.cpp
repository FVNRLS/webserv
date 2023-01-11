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
Config::Config() : _config_file(NULL), _serv_mode(false), _line_num(1), _pos(0), _conf_pos(0), _serv_cnt(0),
				   _serv_def_start(0), _serv_def_end(0), _i_serv(-1), _i_loc(-1) {}

Config::Config(char *path) : _config_file(path), _serv_mode(false), _line_num(1), _pos(0), _conf_pos(0),
							 _serv_cnt(0), _serv_def_start(0), _serv_def_end(0), _i_serv(-1), _i_loc(-1) {

	//VECTOR OF VALID SPECIAL CHARACTERS
	_spec_chars.push_back(OPEN_CURLY_BRACE);
	_spec_chars.push_back(CLOSED_CURLY_BRACE);
	_spec_chars.push_back(NEWLINE);
	_spec_chars.push_back(SPACE);
	_spec_chars.push_back(SEMICOLON);
	_spec_chars.push_back(HASH);
	_spec_chars.push_back(NULL_TERM);

	//VECTOR OF VALID CONFIGURATION FILE MEMBERS
	_valid_members.push_back("server_name");
	_valid_members.push_back("ip_address");
	_valid_members.push_back("port");
	_valid_members.push_back("root");
	_valid_members.push_back("index");
	_valid_members.push_back("max_client_body_size");
	_valid_members.push_back("error_pages");
	_valid_members.push_back("redirection");
	_valid_members.push_back("allowed_methods");
	_valid_members.push_back("allowed_scripts");
	_valid_members.push_back("directory_listing");
	_valid_members.push_back("autoindex");

	_spec_valid_members.push_back("server");
	_spec_valid_members.push_back("location");
	_spec_valid_members.push_back(STR_CLOSED_CURLY_BRACE);
	_spec_valid_members.push_back(STR_SEMICOLON);

	//VECTOR OF FUNCTION POINTERS
	_func_tab.push_back(&Config::set_server_name);
	_func_tab.push_back(&Config::set_ip_address);
	_func_tab.push_back(&Config::set_port);
	_func_tab.push_back(&Config::set_root);
	_func_tab.push_back(&Config::set_index);
	_func_tab.push_back(&Config::set_max_client_body_size);
	_func_tab.push_back(&Config::set_error_pages);
	_func_tab.push_back(&Config::set_redirection);
	_func_tab.push_back(&Config::set_allowed_methods);
	_func_tab.push_back(&Config::set_allowed_scripts);
	_func_tab.push_back(&Config::set_directory_listing);
	_func_tab.push_back(&Config::set_autoindex);
}

Config::Config(const Config &src) { *this = src; }

Config &Config::operator=(const Config &src) { //todo: complete in the end!
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
	if (split_in_server_blocks() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (_serv_cnt == 0)
		return (print_error(NO_SERVER, _config_file));

	create_servers();

	if (extract_servers() == EXIT_FAILURE)
		return (EXIT_FAILURE);

	std::cout << &servers[0]; //display all server characteristics
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

size_t Config::get_line_num(std::string &str) {
	size_t 	pos;

	if (static_cast<int>(_conf_pos) - static_cast<int>(_buf.length()) - 20 < 0)
		pos = 0;
	else
		pos = _conf_pos - _buf.length() - 20;
	pos = _content.find(str, pos);

	_line_num = 1;
	if (pos != std::string::npos) {
		for (int i = 0; i < pos; i++) {
			if (_content[i] == NEWLINE)
				_line_num++;
		}
	}
	return (_line_num);
}

int	Config::split_in_server_blocks() {
	size_t	len;

	len = _content.length();
	_conf_pos = 0;
	while (_conf_pos < len) {
		ignore_comments(len);
		if (_content[_conf_pos] != NEWLINE)
			_buf += _content[_conf_pos];
		else {
			_buf += NEWLINE;
			if (_conf_pos < len)
				_buf += SPACE;
			if (search_for_server() == EXIT_FAILURE)
				return (EXIT_FAILURE);
			if (_serv_mode)
				_serv_blocks[_i_serv] += _buf;
			_buf.clear();
			_line_num++;
		}
		if (_conf_pos == _serv_def_end)
			_serv_mode = false;
		_conf_pos++;
	}
	return (EXIT_SUCCESS);
}

void	Config::create_servers() {
	for (int i = 0; i < _serv_cnt; i++) {
		Server	serv;
		_serv->push_back(serv);
	}
}

void	Config::ignore_comments(size_t len) {
	if (_content[_conf_pos] == HASH) {
		while (_content[_conf_pos] != NEWLINE && _conf_pos < len)
			_conf_pos++;
	}
}

int Config::find_in_spec_chars(char c) const {
	for (int i = 0; i < _spec_chars.size(); i++) {
		if (c == _spec_chars[i])
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int Config::find_in_valid_members(std::string &s) const {
	size_t	len;

	len = _valid_members.size();
	for (int i = 0; i < len; i++) {
		if (_valid_members[i] == s)
			return (EXIT_SUCCESS);
	}
	len = _spec_valid_members.size();
	for (int i = 0; i < len; i++) {
		if (_spec_valid_members[i] == s)
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int Config::search_for_server() {
	_pos = _buf.find("server");
	if (_pos == 0) {
		if (_serv_mode && find_open_brace() == EXIT_SUCCESS)
			return (print_line_error(REDEFINITION_OF_SERVER, _config_file, _line_num));
		else if (_serv_mode && find_open_brace() == EXIT_FAILURE)
			return (NOT_FOUND);
		else if (!_serv_mode && find_open_brace() == EXIT_FAILURE)
			return (print_line_error(INVALID_SERVER_DEFINITION, _config_file, _line_num));
		if (check_closed_braces() == EXIT_FAILURE)
			return (print_line_error(BRACES_NOT_CLOSED, _config_file, _line_num));
		_serv_mode = true;
		_serv_blocks.push_back("");
		_i_serv++;
		_serv_cnt++;
		return (EXIT_SUCCESS);
	}
	else if (!_serv_mode && find_in_spec_chars(_buf[0]) == EXIT_FAILURE)
		return (print_line_error(INVALID_CHARACTERS_FOUND, _config_file, _line_num));
	return (NOT_FOUND);
}

int Config::find_open_brace() {
	size_t	j;
	size_t	len;

	j = _conf_pos - _buf.length() + 8; //offset from the start of the word 'server'
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

	open_braces = 1;
	closed_braces = 0;
	j = _serv_def_start + 1;
	while (j < _content.length()) {
		if (_content[j] == OPEN_CURLY_BRACE)
			open_braces++;
		else if (_content[j] == CLOSED_CURLY_BRACE)
			closed_braces++;
		if (open_braces == closed_braces) {
			_serv_def_end = j + 1;
			return (EXIT_SUCCESS);
		}
		j++;
	}
	return (EXIT_FAILURE);
}

int	Config::extract_servers() {
	_conf_pos = 0;
	replace_open_braces(_serv_blocks);
	for (_i_serv = 0;  _i_serv < _serv_blocks.size(); _i_serv++) {
		if (extract_server_block(_i_serv) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	Config::replace_open_braces(std::vector<std::string> &v) {
	size_t	pos;

	for (int i = 0; i < v.size(); i++) {
		while ((pos = v[i].find(OPEN_CURLY_BRACE)) != _serv_blocks[0].npos) {
			v[i].replace(pos, 1, STR_SEMICOLON);
		}
	}
}

int Config::extract_server_block(int i) {
	_serv_mode = true;
	_buf.clear();

	while (_conf_pos < _serv_blocks[i].length()) {
		if (_serv_blocks[i][_conf_pos] == SEMICOLON) {
			_tokens = split(_buf, SPACE);
			set_mode();
//			print_vector(_tokens, _tokens.size());
			if (!_tokens.empty() && find_in_valid_members(_tokens[0]) == EXIT_FAILURE)
				return (print_line_error(INVALID_MEMBER, _config_file, get_line_num(_tokens[0])));
			else if (set_server_parameter() == EXIT_FAILURE)
				return (EXIT_FAILURE);
			_buf.clear();
			_tokens.clear();
		}
		else if (_serv_blocks[i][_conf_pos] == NEWLINE)
			_line_num++;
		else
			_buf += _serv_blocks[0][_conf_pos];
		_conf_pos++;
	}
	return (EXIT_SUCCESS);
}

void	Config::set_mode() {
	if (_tokens[0] == "location") {
		_serv_mode = false;
		_i_loc++;
	}
	else if (_tokens[0] == "}") {
		_serv_mode = true;
		_tokens.erase(_tokens.begin()); //remove the '}' from the tokens list, so only the parameters are left
	}
}

int	Config::set_server_parameter() {
	int i;

	i = get_func_index();
	if (i == SPEC_MEMBER)
		return (EXIT_SUCCESS);
	return ((this->*_func_tab[i])());
}

int	Config::get_func_index() {
	std::string	member;

	member = _tokens[0];
	for (int i = 0; i < _valid_members.size(); i++) {
		if (member == _valid_members[i])
			return (i);
	}
	return (SPEC_MEMBER);
}

int Config::set_server_name() {
	if ((*_serv)[_i_serv]._name.empty()) {
		if (_serv_mode) {
			if (_tokens.size() == 2) {
				(*_serv)[_i_serv]._name = _tokens[1];
				return (EXIT_SUCCESS);
			}
			return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
		}
		else
			return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
	}
	return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
}

int Config::set_ip_address() {
	if ((*_serv)[_i_serv]._ip.empty()) {
		if (_serv_mode) {
			if (_tokens.size() == 2) {
				(*_serv)[_i_serv]._ip = _tokens[1];
				return (EXIT_SUCCESS);
			}
			return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
		}
		else
			return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
	}
	return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
}

int Config::set_port() {
	char *endptr;

	if ((*_serv)[_i_serv]._port == 0) {
		if (_serv_mode) {
			if (_tokens.size() == 2) {
				(*_serv)[_i_serv]._port = strtoll(_tokens[1].c_str(), &endptr, 10);

				if (endptr == _tokens[1])
					std::cout << "Error: Not a valid number" << std::endl;
				else if ((*_serv)[_i_serv]._port > MAX_PORT_NUM || (*_serv)[_i_serv]._port < 1)
					std::cout << "Error: Number out of range" << std::endl;
				else if (*endptr != '\0')
					std::cout << "Error: Trailing non-numeric characters" << std::endl;

				return (EXIT_SUCCESS);
			}
			return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
		} else
			return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
	}
	return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
}

int Config::set_root() {
	return (EXIT_SUCCESS);
}

int Config::set_index() {
	return (EXIT_SUCCESS);
}

int Config::set_max_client_body_size() {
	return (EXIT_SUCCESS);
}

int Config::set_error_pages() {
	return (EXIT_SUCCESS);
}

int Config::set_redirection() {
	return (EXIT_SUCCESS);
}

int Config::set_allowed_methods() {
	return (EXIT_SUCCESS);
}

int Config::set_allowed_scripts() {
	return (EXIT_SUCCESS);
}

int Config::set_directory_listing() {
	return (EXIT_SUCCESS);
}

int Config::set_autoindex() {
	return (EXIT_SUCCESS);
}
