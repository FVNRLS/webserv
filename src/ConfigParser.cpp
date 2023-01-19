/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:27:52 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 13:27:52 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Config.hpp"

//BASIC CLASS SETUP
ConfigParser::ConfigParser() : _config_file(NULL), _serv_mode(false), _line_num(1), _pos(0), _conf_pos(0), _serv_cnt(0),
							   _serv_def_start(0), _serv_def_end(0), _i_serv(-1), _i_loc(-1) {}

ConfigParser::ConfigParser(std::vector<Config> &servers, char *path) : _config_file(path), _serv(&servers),
_serv_mode(false), _line_num(1), _pos(0), _conf_pos(0), _serv_cnt(0), _serv_def_start(0), _serv_def_end(0),
_i_serv(-1), _i_loc(-1) {

	//VECTOR OF VALID SPECIAL CHARACTERS
	_spec_chars.push_back(OPEN_CURLY_BRACE);
	_spec_chars.push_back(CLOSED_CURLY_BRACE);
	_spec_chars.push_back(NEWLINE);
	_spec_chars.push_back(SPACE);
	_spec_chars.push_back(SEMICOLON);
	_spec_chars.push_back(HASH);
	_spec_chars.push_back(NULL_TERM);

	//PAIRS OF VALID CONFIGURATION IDENTIFIERS AND FUNCTION POINTERS
	_valid_identifiers.push_back("server_name"); 			_func_tab.push_back(&ConfigParser::set_server_name);
	_valid_identifiers.push_back("ip_address");				_func_tab.push_back(&ConfigParser::set_ip_address);
	_valid_identifiers.push_back("port");					_func_tab.push_back(&ConfigParser::set_port);
	_valid_identifiers.push_back("root");					_func_tab.push_back(&ConfigParser::set_root);
	_valid_identifiers.push_back("allowed_methods");		_func_tab.push_back(&ConfigParser::set_allowed_methods);
	_valid_identifiers.push_back("index");					_func_tab.push_back(&ConfigParser::set_index);
	_valid_identifiers.push_back("max_client_body_size");	_func_tab.push_back(&ConfigParser::set_max_client_body_size);
	_valid_identifiers.push_back("error_pages");			_func_tab.push_back(&ConfigParser::set_error_pages_dir);
	_valid_identifiers.push_back("redirection");			_func_tab.push_back(&ConfigParser::set_redirection);
	_valid_identifiers.push_back("allowed_scripts");		_func_tab.push_back(&ConfigParser::set_allowed_scripts);
	_valid_identifiers.push_back("directory_listing");		_func_tab.push_back(&ConfigParser::set_directory_listing);
	_valid_identifiers.push_back("cgi_path");				_func_tab.push_back(&ConfigParser::set_cgi_path);

	_spec_valid_identifiers.push_back("server");
	_spec_valid_identifiers.push_back("location");
	_spec_valid_identifiers.push_back(STR_CLOSED_CURLY_BRACE);
	_spec_valid_identifiers.push_back(STR_SEMICOLON);

	_valid_methods.push_back("GET");
	_valid_methods.push_back("POST");
	_valid_methods.push_back("PUT");
	_valid_methods.push_back("DELETE");
}

ConfigParser::ConfigParser(const ConfigParser &src) { *this = src; }

ConfigParser &ConfigParser::operator=(const ConfigParser &src) {
	if (this == &src)
		return (*this);
	_config_file = src._config_file;
	_content = src._content;
	_buf = src._buf;
	_serv_mode = src._serv_mode;
	_line_num = src._line_num;
	_pos = src._pos;
	_conf_pos = src._conf_pos;
	_serv_cnt = src._serv_cnt;
	_serv_def_start = src._serv_def_start;
	_serv_def_end = src._serv_def_end;
	_i_serv = src._i_serv;
	_i_loc = src._i_loc;
	_spec_chars = src._spec_chars;
	_valid_identifiers = src._valid_identifiers;
	_spec_valid_identifiers = src._spec_valid_identifiers;
	_valid_methods = src._valid_methods;
	*_serv = *src._serv; //todo: is it right?
	_serv_blocks = src._serv_blocks;
	_tokens = src._tokens;
	_func_tab = src._func_tab;
	return (*this);
}

ConfigParser::~ConfigParser() {}


//MEMBER FUNCTIONS
int	ConfigParser::parse() {
	if (check_extension() == EXIT_FAILURE)
		return (print_error(INVALID_EXTENSION, _config_file));
	if (read_conf_file() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (split_in_server_blocks() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (_serv_cnt == 0)
		return (print_error(NO_SERVER, _config_file));
	create_servers();
	if (extract_servers() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (check_required_param_def() == EXIT_FAILURE)
		return (EXIT_FAILURE);

	for (int i = 0; i < (*_serv).size(); i++)
		std::cout << &(*_serv)[i] << std::endl;

	return (EXIT_SUCCESS);
}

int	ConfigParser::check_extension() {
	std::size_t dot_pos;

	dot_pos = _config_file.find_last_of('.');
	if (_config_file.empty() || dot_pos == std::string::npos)
		return (EXIT_FAILURE);
	if (_config_file.substr(dot_pos) == ".conf")
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	ConfigParser::read_conf_file() {
	std::ifstream	file;
	std::string		line;

	if (access(_config_file.c_str(), F_OK) < 0)
		return (print_error(NO_FILE, _config_file));
	file.open(_config_file.c_str());
	if (!file.is_open() || file.fail())
		return (print_error(BAD_PERMISSIONS, _config_file));
	while (std::getline(file, line)) {
		line = trim(line);
		_content += line + '\n';
	}
	file.close();
	return (EXIT_SUCCESS);
}

size_t ConfigParser::get_line_num(std::string &str) {
	size_t 	pos;

	if (static_cast<int>(_conf_pos) - static_cast<int>(_buf.length()) - NEGATIVE_OFFSET < 0)
		pos = 0;
	else
		pos = _conf_pos - _buf.length() - NEGATIVE_OFFSET;
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

int	ConfigParser::split_in_server_blocks() {
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

void	ConfigParser::create_servers() {
	for (int i = 0; i < _serv_cnt; i++) {
		Config	serv;
		_serv->push_back(serv);
	}
}

void	ConfigParser::ignore_comments(size_t len) {
	if (_content[_conf_pos] == HASH) {
		while (_content[_conf_pos] != NEWLINE && _conf_pos < len)
			_conf_pos++;
	}
}

int ConfigParser::find_in_spec_chars(char c) const {
	for (int i = 0; i < _spec_chars.size(); i++) {
		if (c == _spec_chars[i])
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int ConfigParser::find_in_valid_identifiers(std::string &s) const {
	size_t	len;

	len = _valid_identifiers.size();
	for (int i = 0; i < len; i++) {
		if (_valid_identifiers[i] == s)
			return (EXIT_SUCCESS);
	}
	len = _spec_valid_identifiers.size();
	for (int i = 0; i < len; i++) {
		if (_spec_valid_identifiers[i] == s)
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int ConfigParser::search_for_server() {
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

int ConfigParser::find_open_brace() {
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

int	ConfigParser::check_closed_braces() {
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

int	ConfigParser::extract_servers() {
	_conf_pos = 0;
	replace_open_braces(_serv_blocks);
	for (_i_serv = 0;  _i_serv < _serv_blocks.size(); _i_serv++) {
		if (extract_server_block() == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	ConfigParser::replace_open_braces(std::vector<std::string> &v) {
	size_t	pos;

	for (int i = 0; i < v.size(); i++) {
		while ((pos = v[i].find(OPEN_CURLY_BRACE)) != _serv_blocks[0].npos) {
			v[i].replace(pos, 1, STR_SEMICOLON);
		}
	}
}

int ConfigParser::extract_server_block() {
	size_t len;
	size_t j;

	_serv_mode = true;
	_buf.clear();
	_i_loc = -1;
	len = _serv_blocks[_i_serv].length();
	j = 0;
	while (j < len) {
		if (_serv_blocks[_i_serv][j] == SEMICOLON) {
			if (_buf.empty()) {
				j++;
				_conf_pos++;
				continue;
			}
			_tokens = split(_buf, SPACE);
			if (_tokens.empty()) {
				_buf.clear();
				continue;
			}
			if (set_mode() == EXIT_FAILURE)
				return (EXIT_FAILURE);
			if (!_tokens.empty() && find_in_valid_identifiers(_tokens[0]) == EXIT_FAILURE)
				return (print_line_error(INVALID_IDENTIFIER, _config_file, get_line_num(_tokens[0])));
			else if (set_server_parameter() == EXIT_FAILURE)
				return (EXIT_FAILURE);
			_buf.clear();
			_tokens.clear();
		}
		else if (_serv_blocks[_i_serv][j] != NEWLINE)
			_buf += _serv_blocks[_i_serv][j];
		j++;
		_conf_pos++;
	}
	return (EXIT_SUCCESS);
}

int	ConfigParser::set_mode() {
	if (_tokens[0] == "location") {
		if (!_serv_mode)
			return (print_line_error(NESTED_LOCATION_DEFINITION, _config_file, get_line_num(_tokens[0])));
		_serv_mode = false;
	}
	else if (_tokens[0] == "}") {
		_serv_mode = true;
		if (!_tokens.empty())
			_tokens.erase(_tokens.begin()); //remove the '}' from the tokens list, so only the parameters are left
	}
	return (EXIT_SUCCESS);
}


//***** CONFIG FILE SETTERS *****
int	ConfigParser::set_server_parameter() {
	int i;

	i = get_func_index();
	if (i == SPEC_IDENTIFIER) {
		if (_tokens[0] == "location") {
			if (add_location() == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		return (EXIT_SUCCESS);
	}
	return ((this->*_func_tab[i])());
}

int	ConfigParser::get_func_index() {
	std::string	member;

	member = _tokens[0];
	for (int i = 0; i < _valid_identifiers.size(); i++) {
		if (member == _valid_identifiers[i])
			return (i);
	}
	return (SPEC_IDENTIFIER);
}

int ConfigParser::add_location() {
	location loc;

	if (set_loc_prefix(loc) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	loc.max_client_body_size = UINT32_MAX;
	loc.directory_listing = false;
	(*_serv)[_i_serv]._locations.push_back(loc);
	_i_loc++;
	return (EXIT_SUCCESS);
}


//SERVER SPECIFIC SETTERS
int ConfigParser::set_loc_prefix(location &loc) {
	if (_tokens.size() != 2)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	loc.prefix = _tokens[1];
	return (EXIT_SUCCESS);
}

int ConfigParser::set_server_name() {
	size_t	num_tokens;

	num_tokens = _tokens.size();
	if ((*_serv)[_i_serv]._name.empty()) {
		if (_serv_mode) {
			if (num_tokens < 2)
				return (EXIT_SUCCESS);
			else if (num_tokens > 2)
				return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
			(*_serv)[_i_serv]._name = _tokens[1];
			return (EXIT_SUCCESS);
		}
		else
			return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
	}
	return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
}

int ConfigParser::set_ip_address() {
	char 						*endptr;
	std::vector<std::string>	_octets;
	std::vector<long>			_octet_val;

	if ((*_serv)[_i_serv]._ip.empty()) {
		if (_serv_mode) {
			if (_tokens.size() == 2) {
				(*_serv)[_i_serv]._ip = _tokens[1];

				_octets = split(_tokens[1], DOT);
				if (_octets.size() == 4)
					for (int i = 0; i < 4; i++) {
						_octet_val.push_back(strtoll(_octets[i].c_str(), &endptr, 10));
						if ((endptr == _octets[i]) || (*endptr != '\0') || ((_octet_val[i] > 255 || _octet_val[i] < 0)))
							return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
						if (_octet_val[0] == 0)
							return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
					}
				else
					return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
				return (EXIT_SUCCESS);
			}
			return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
		}
		else
			return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
	}
	return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
}

int ConfigParser::set_port() {
	char *endptr;
	long port;

	if (_serv_mode) {
		if (_tokens.size() == 2) {
			port = strtoll(_tokens[1].c_str(), &endptr, 10);
			if (endptr == _tokens[1] || *endptr != '\0' || port > MAX_PORT_NUM || port < 1)
				return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
			(*_serv)[_i_serv]._ports.push_back(port);
			return (EXIT_SUCCESS);
		}
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	}
	else
		return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
}

int ConfigParser::set_root() {
	std::string *param;

	if (_serv_mode)
		param = &(*_serv)[_i_serv]._root;
	else
		param = &(*_serv)[_i_serv]._locations[_i_loc].root;
	if (!param->empty())
		return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
	else if (_tokens.size() != 2)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	*param = _tokens[1];
	return (EXIT_SUCCESS);
}

int ConfigParser::set_allowed_methods() {
	size_t 						num_methods;
	size_t 						num_valid_methods;
	size_t 						j;
	std::vector<std::string> 	*param;

	num_methods = _tokens.size() - 1;
	num_valid_methods = _valid_methods.size();
	if (_serv_mode)
		param = &(*_serv)[_i_serv]._methods;
	else
		param = &(*_serv)[_i_serv]._locations[_i_loc].methods;

	if (!param->empty())
		return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
	else if (num_methods < 1 || num_methods > num_valid_methods)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	for (int i = 1; i <= num_methods; i++) {
		j = 0;
		while (j < num_valid_methods) {
			if (_tokens[i] == _valid_methods[j]) {
				param->push_back(_tokens[i]);
				break;
			}
			j++;
		}
		if (j == num_valid_methods)
			return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
	}
	return (EXIT_SUCCESS);
}

int ConfigParser::set_index() {
	std::string *param;
	size_t 		num_tokens;

	num_tokens = _tokens.size();
	if (_serv_mode)
		param = &(*_serv)[_i_serv]._index;
	else
		param = &(*_serv)[_i_serv]._locations[_i_loc].index;
	if (!param->empty())
		return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
	else if (num_tokens > 2)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	else if (num_tokens == 1)
		*param = DEFAULT_INDEX;
	else if (num_tokens == 2)
		*param = _tokens[1];
	return (EXIT_SUCCESS);
}

int ConfigParser::set_max_client_body_size() {
	long long 	*param;
	size_t 		num_tokens;
	char 		*endptr;

	num_tokens = _tokens.size();
	if (_serv_mode)
		param = &(*_serv)[_i_serv]._max_client_body_size;
	else
		param = &(*_serv)[_i_serv]._locations[_i_loc].max_client_body_size;
	if (*param != UINT32_MAX)
		return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
	else if (num_tokens > 2)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	else if (num_tokens == 2) {
		*param = std::strtoll(_tokens[1].c_str(), &endptr, 10);
			if (*param < MIN_CLIENT_BODY_SIZE || *param > UINT32_MAX || endptr == _tokens[1] || *endptr != '\0')
				return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
	}

	return (EXIT_SUCCESS);
}

int ConfigParser::set_error_pages_dir() {
	std::string *param;
	size_t 		num_tokens;

	num_tokens = _tokens.size();
	if (_serv_mode)
		param = &(*_serv)[_i_serv]._error_pages_dir;
	else
		return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
	if (!param->empty())
		return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
	else if (num_tokens > 2)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	else if (num_tokens == 1)
		*param = DEFAULT_ERROR_PAGES_DIR;
	else if (num_tokens == 2)
		*param = _tokens[1];
	return (EXIT_SUCCESS);
}

int ConfigParser::set_redirection() {
	std::vector<std::pair<std::string, std::string> > *param;
	size_t 		num_tokens;
	std::pair<std::string, std::string>	pair;

	num_tokens = _tokens.size();
	if (_serv_mode)
		param = &(*_serv)[_i_serv]._redirect;
	else
		param = &(*_serv)[_i_serv]._locations[_i_loc].redirect;
	if (!param->empty() && !_serv_mode)
		return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));

	if (num_tokens == 1) {
		pair.first = DEFAULT_REDIR_FIRST;
		pair.second = DEFAULT_REDIR_SECOND;
	}
	else if (num_tokens != 4)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	else {
		pair.first = _tokens[1];
		if (_tokens[2] != EQUAL_SIGN)
			return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
		if (_tokens[3] == EQUAL_SIGN)
				return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
		pair.second = _tokens[3];
	}
	if (!pair.first.empty() && !pair.second.empty())
		param->push_back(pair);
	return (EXIT_SUCCESS);
}


//LOCATION SPECIFIC SETTERS
int ConfigParser::set_allowed_scripts() {
	std::vector<std::pair<std::string, std::string> >	*param;
	size_t	num_tokens;
	std::pair<std::string, std::string>	pair;

	num_tokens = _tokens.size();
	if (_serv_mode)
		return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
	else
		param = &(*_serv)[_i_serv]._locations[_i_loc].scripts;

	if (num_tokens == 1) {
		pair.first = DEFAULT_REDIR_FIRST;
		pair.second = DEFAULT_REDIR_SECOND;
	}
	else if (num_tokens != 4)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	else {
		pair.first = _tokens[1];
		if (_tokens[2] != EQUAL_SIGN)
			return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
		if (_tokens[3] == EQUAL_SIGN)
			return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
		pair.second = _tokens[3];
	}
	if (!pair.first.empty() && !pair.second.empty())
		param->push_back(pair);
	return (EXIT_SUCCESS);
}

int ConfigParser::set_directory_listing() {
	size_t 		num_tokens;

	num_tokens = _tokens.size();
	if (_serv_mode)
		return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
	else if ((*_serv)[_i_serv]._locations[_i_loc].directory_listing == true)
		return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
	else if (num_tokens > 2)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	else if (num_tokens == 2) {
		if (_tokens[1] == "false")
			return (EXIT_SUCCESS);
		else if (_tokens[1] == "true")
			(*_serv)[_i_serv]._locations[_i_loc].directory_listing = true;
		else
			return (print_line_error(INVALID_PARAMETER, _config_file, get_line_num(_tokens[0])));
	}
	return (EXIT_SUCCESS);
}

int ConfigParser::set_cgi_path() {
	size_t 		num_tokens;

	num_tokens = _tokens.size();
	if (_serv_mode)
		return (print_line_error(INVALID_SCOPE, _config_file, get_line_num(_tokens[0])));
	else if (!(*_serv)[_i_serv]._locations[_i_loc].cgi_path.empty())
		return (print_line_error(REDEFINITION_OF_SERVER_PARAMETER, _config_file, get_line_num(_tokens[0])));
	else if (num_tokens > 2)
		return (print_line_error(INVALID_NUM_OF_PARAMETERS, _config_file, get_line_num(_tokens[0])));
	(*_serv)[_i_serv]._locations[_i_loc].cgi_path = _tokens[1];
	return (EXIT_SUCCESS);
}


//POST PARSING CHECKERS
int ConfigParser::check_required_param_def() {
	for (_i_serv = 0; _i_serv < _serv_cnt; _i_serv++) {
		if (check_serv_config() == EXIT_FAILURE || check_loc_config() == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	create_ip_port_combinations();
	if (check_ip_port_combinations() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int ConfigParser::check_serv_config() {
	if (check_server_name() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (check_ip_address() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (check_ports() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (check_root() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int ConfigParser::check_server_name() {
	std::stringstream 	ss;
	std::string			*name;

	name = &(*_serv)[_i_serv]._name;
	ss << "Default_Server_" << _i_serv;
	if (name->empty())
		*name = ss.str();
	for (int i = 0; i < _serv->size(); i++) {
		if (*name == (*_serv)[i]._name && i != _i_serv)
			return (print_param_error(SERVER_NAME_NOT_UNIQUE, _config_file, *name));
	}
	return (EXIT_SUCCESS);
}

int ConfigParser::check_ip_address() {
	std::string	*ip;

	ip = &(*_serv)[_i_serv]._ip;
	if (ip->empty())
		return (print_param_error(NO_IP_ADDRESS, _config_file, (*_serv)[_i_serv]._name));
	return (EXIT_SUCCESS);
}

int ConfigParser::check_ports() {
	std::vector <long>	*ports;

	ports = &(*_serv)[_i_serv]._ports;
	if (ports->empty())
		return (print_param_error(NO_PORTS, _config_file, (*_serv)[_i_serv]._name));
	return (EXIT_SUCCESS);
}

int ConfigParser::check_root() {
	std::string	*root;

	root = &(*_serv)[_i_serv]._root;
	if (root->empty())
		return (print_param_error(NO_ROOT, _config_file, (*_serv)[_i_serv]._name));
	return (EXIT_SUCCESS);
}

int ConfigParser::check_loc_config() {
	size_t		num_locs;

	num_locs = (*_serv)[_i_serv]._locations.size();
	for (_i_loc = 0; _i_loc < num_locs; _i_loc++) {
		if (check_loc_index() == EXIT_FAILURE)
			return (EXIT_FAILURE);
		check_loc_root();
		check_cgi_path();

	}
	return (EXIT_SUCCESS);
}

int ConfigParser::check_loc_index() {
	std::string	*index;

	index = &(*_serv)[_i_serv]._locations[_i_loc].index;
	if (index->empty())
		return (print_param_error(NO_INDEX, _config_file, (*_serv)[_i_serv]._locations[_i_loc].prefix));
	return (EXIT_SUCCESS);
}

void 	ConfigParser::check_loc_root() {
	std::string	*root;

	root = &(*_serv)[_i_serv]._locations[_i_loc].root;
	if (root->empty())
		*root = (*_serv)[_i_serv]._locations[_i_loc].prefix;
}

void 	ConfigParser::check_cgi_path() {
	std::string	*cgi;

	cgi = &(*_serv)[_i_serv]._locations[_i_loc].cgi_path;
	if (cgi->empty())
		*cgi = DEFAULT_CGI_PATH;
}

void	ConfigParser::create_ip_port_combinations() {
	std::stringstream 	ss;
	std::string 		comb;

	for (int i = 0; i < _serv_cnt; i++) {
		for (int j = 0; j < (*_serv)[i]._ports.size(); j++) {
			ss << (*_serv)[i]._ports[j];
			comb = (*_serv)[i]._ip + "::" + ss.str();
			(*_serv)[i]._ip_port_comb.push_back(comb);
			comb.clear();
			ss.clear();
			ss.str("");
		}
	}
}

int	ConfigParser::check_ip_port_combinations() {
	std::vector <std::string>			combs;
	std::vector<std::string>::iterator 	it;

	for (int i = 0; i < _serv_cnt; i++)
		combs.insert(combs.end(), (*_serv)[i]._ip_port_comb.begin(), (*_serv)[i]._ip_port_comb.end());
	std::sort(combs.begin(), combs.end());
	it = std::adjacent_find(combs.begin(), combs.end());
	if (it != combs.end())
		return (print_param_error(DUPLICATE_IP_PORT_COMB, _config_file, *it));
	return (EXIT_SUCCESS);
}
