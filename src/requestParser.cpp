/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:24:56 by doreshev          #+#    #+#             */
/*   Updated: 2023/02/17 12:39:45 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requestParser.hpp"

requestParser::requestParser(request_handler &request) :  _request(request){}

requestParser::~requestParser() {}

void    requestParser::parse() {
    parse_request_line();
    if (_request.status)
        return;
    split_query();
    set_cookies();
    set_url_type();
    check_redirection();
    if (_request.status)
        return;
    set_location_config();
    if (_request.status)
        return;
    check_method();
    if (_request.status)
        return;
    if (_request.dir_list && _request.method == "GET") {
        _request.body_received = true;
        return;
    }
    _request.dir_list = false;
    translate_path();
    set_interpreter();
    check_file_path();
    if (_request.status)
        return;
    set_cgi_path();
    _request.user_agent = get_header_value("User-Agent:", _request.buf);
    _request.content_type = get_header_value("Content-Type:", _request.buf);
    check_chunked();
    if (_request.chunked)
        return;
    set_body_length();
}

void    requestParser::parse_request_line() {
    std::vector<std::string> tokens = tokenize_first_line();

    if (tokens.size() < 3) {
        _request.status = BAD_REQUEST;
        return;
    }
    _request.method = tokens[0];
    _request.file_path = tokens[1];
}

std::vector<std::string> requestParser::tokenize_first_line() {
	std::string					first_line;

	size_t	new_line_position = _request.buf.find(NEWLINE);
	if (new_line_position != std::string::npos) {
		first_line =  _request.buf.substr(0, new_line_position);
		return split(first_line, SPACE);
	}
	return std::vector<std::string>();
}

void    requestParser::check_chunked() {
    if (!header_key_exists("Transfer-Encoding:", _request.buf))
        return;
    if (!header_value_is_equal_to("Transfer-Encoding:", "chunked", _request.buf))
    {
        _request.status = NOT_IMPLEMENTED;
        return;
    }
    _request.chunked = true;
    if (header_value_is_equal_to("Expect:", "100-continue", _request.buf)) {
        _request.status = CONTINUE;
        _request.response = "HTTP/1.1 100 Continue\r\n\r\n";
    }
    else
        get_body_length_chunked();
    _request.buf = _request.buf.substr(_request.head_length);
    _request.head_length = 0;
    _request.chunkfile.open("./uploads/curl_upload.txt", std::ios::out | std::ios::trunc);
    if (!_request.chunkfile.is_open() || _request.chunkfile.fail())
        _request.status = INTERNAL_SERVER_ERROR;
}

void    requestParser::get_body_length_chunked() {
    std::string body = _request.buf.substr(_request.head_length);
    _request.body_length = std::strtol(body.c_str(), NULL, 16);
    if (_request.body_length == 0)
        _request.body_received = true;
}

void	requestParser::set_cookies() {
	size_t position = _request.buf.find("Cookie: key=");

	if (position == std::string::npos)
		return;
	position += std::strlen("Cookie: key=");
    _request.cookies = std::atoi(_request.buf.substr(position).c_str());
}

void	requestParser::split_query() {
	size_t position = _request.file_path.find('?');

	if (position != std::string::npos) {
        _request.query = _request.file_path.substr(position + 1);
        _request.body_length = _request.query.length();
	}
    _request.file_path = _request.file_path.substr(0, position);
}

void    requestParser::set_url_type() {
    _locations = split(_request.file_path, '/');

    switch (_locations.size()) {
        case 0:
            _url_type = SERVER_INDEX;
            break;
        case 1:
            if (_locations[0].find('.') != std::string::npos)
                _url_type = SERVER;
            else
                _url_type = LOCATION_INDEX;
            break;
        default:
            _url_type = LOCATION;
    }
}

void    requestParser::check_redirection() {
    if (_url_type != LOCATION_INDEX)
        return;

    for (size_t	i = 0; i < _request.socket.get_config().get_redirect().size(); i++) {
        if (_request.socket.get_config().get_redirect()[i].first == _locations[0]) {
            _request.status = MOVED_PERMANENTLY;
            _request.buf = _request.socket.get_config().get_redirect()[i].second;
            return;
        }
    }
}

void    requestParser::set_location_config() {
    if (_url_type != LOCATION && _url_type != LOCATION_INDEX)
        return;

    for (size_t	i = 0; i < _request.socket.get_config().get_locations().size(); i++) {
        if (_request.socket.get_config().get_locations()[i].prefix.compare(1, _locations[0].size(), _locations[0]) == 0) {
             _location_config = _request.socket.get_config().get_locations()[i];
            _request.dir_list = _location_config.directory_listing;
            if (_request.dir_list && _url_type == LOCATION_INDEX)
                _request.file_path = _location_config.root;
            else
                _request.dir_list = false;
            return;
        }
    }
    _request.status = FORBIDDEN;
}

void	requestParser::check_method() {
    std::vector<std::string> allowed_methods;

    if (_url_type == LOCATION)
        allowed_methods = _location_config.methods;
    else
        allowed_methods = _request.socket.get_config().get_methods();
    for (size_t	i = 0; i < allowed_methods.size(); i++) {
        if	(allowed_methods[i] == _request.method)
            return;
    }
    _request.status = METHOD_NOT_ALLOWED;
}

void	requestParser::translate_path() {
    switch (_url_type) {
        case SERVER_INDEX:
            _request.file_path = _request.socket.get_config().get_root() + _request.socket.get_config().get_index();
            break;
        case SERVER:
            _request.file_path = _request.socket.get_config().get_root() + _locations.back();
            break;
        case LOCATION_INDEX:
            _request.file_path = _location_config.root + _location_config.index;
            break;
        case LOCATION:
            _request.file_path = _location_config.root + _locations[1];
            for (size_t i = 2; i < _locations.size(); i++) {
                _request.file_path += "/" + _locations[i];
            }
//            if (_locations.back().find('.') == std::string::npos)
//                _request.file_path += "/" + _location_config.index;
    }
}

void	requestParser::set_interpreter() {
    if (_url_type == SERVER_INDEX || _url_type == SERVER)
        return;

    size_t  position = _request.file_path.find_last_of('.');
    if (position == std::string::npos)
        return;

    std::string extension = _request.file_path.substr(position + 1);
    for (size_t i = 0; i < _location_config.scripts.size(); i++) {
        if (_location_config.scripts[i].first == extension) {
            _request.interpreter = _location_config.scripts[i].second;
            return;
        }
    }
}

void	requestParser::check_file_path() {
    if (access(_request.file_path.data(), F_OK) == -1)
        _request.status = PAGE_NOT_FOUND;
    else if (!is_regular_file(_request.file_path.data())) {
        _request.status = PAYMENT_REQUIRED;
    }
    else if (access(_request.file_path.data(), R_OK) == -1)
        _request.status = FORBIDDEN;
}

void requestParser::set_body_length() {
    if (_request.method == "GET")
        return;

    size_t pos = _request.buf.find("Content-Length: ");
    if (pos != std::string::npos)
        _request.body_length = static_cast<size_t> (std::atoll(_request.buf.data() + pos + std::strlen("Content-Length: ")));
    if (_request.body_length == 0)
        _request.status = NO_CONTENT;
}

void requestParser::set_cgi_path() {
    if (_url_type == SERVER || _url_type == SERVER_INDEX)
        return ;
    _request.cgi_path = _location_config.cgi_path;
}