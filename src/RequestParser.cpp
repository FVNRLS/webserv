/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:24:56 by doreshev          #+#    #+#             */
/*   Updated: 2023/02/17 12:39:45 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser(request_handler &request) : _request(request) {}

RequestParser::~RequestParser() {}

/*
 * Is responsible for parsing a request.
 * Sequentially calls a series of functions to extract different parts of the request and set them in the _request struct.
 * Extracts the request line, which contains the HTTP method, path, and protocol version. If _request.status is set - returns.
 * Next, calls split_query() to separate the path from the query parameters, if there are any.
 * Then, calls set_cookies() to extract and set any cookies found in the request headers.
 * After that, the function calls set_url_type() to determine the type of URL.
 * The function then calls set_location_config() to set the _location_config object in the _request object,
 * and check_method() to ensure that the request method is allowed for the specified location.
 * If the request is for a directory listing and the method is GET, the function sets the _request.body_received flag and returns.
 * Otherwise, the function sets the _request.dir_list flag to false, calls translate_path() to convert the URL to a local file path, a
 * nd set_interpreter() to determine the interpreter for CGI scripts.
 * It then calls check_file_path() to ensure that the file path exists and is allowed by the server configuration.
 * The function then calls set_cgi_path() to set the path to the CGI executable, and get_header_value() to extract the User-Agent and Content-Type headers. It then calls check_chunked() to determine if the request is using chunked encoding. If _request.chunked is set, the function returns.
 * Finally, the function calls set_body_length() to determine the length of the request body, which is used for non-chunked requests.
 * If _request.status (mostly indicates an error code) is set at any point, the function returns without proceeding.
 * */
void    RequestParser::parse() {
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

/*
 * Is responsible for parsing the first line of a client's HTTP request, which contains the HTTP method, file path, and HTTP version.
 * The function uses the tokenize_first_line() method to split the first line into a vector of strings.
 * If the vector does not contain at least three tokens (the HTTP method, file path, and HTTP version),
 * then the _request object's status is set to indicate that there was a bad request.
 * If the vector contains all the required three tokens, sets the method attribute and file path to respective tokens.
 * */
void    RequestParser::parse_request_line() {
    std::vector<std::string> tokens = tokenize_first_line();

    if (tokens.size() < 3) {
        _request.status = BAD_REQUEST;
        return;
    }
    _request.method = tokens[0];
    _request.file_path = tokens[1];
}

/*
 * Responsible for splitting the first line of the client's HTTP request into a vector of strings.
 * */
std::vector<std::string> RequestParser::tokenize_first_line() {
	std::string					first_line;

	size_t	new_line_position = _request.buf.find(NEWLINE);
	if (new_line_position != std::string::npos) {
		first_line =  _request.buf.substr(0, new_line_position);
		return split(first_line, SPACE);
	}
	return std::vector<std::string>();
}

/*
 * Checks whether the "Transfer-Encoding" header field exists in the request
 * and whether its value is equal to "chunked".
 *
 * If the header field or value is missing, it returns without doing anything.
 * If the value is not equal to "chunked", it sets the response status code to "501 Not Implemented" and returns.
 * If the value is "chunked", sets the _request.chunked flag to true and checks if the "Expect" header field has the value "100-continue".
 * If it does, it sets the response status code to "100 Continue" and sends the response to the client.
 *
 * If the "Expect" header field does not have the value "100-continue", calculates the length of the request body.
 * The function then updates the request buffer and head length, opens a file to save the chunked data,
 * and sets the _request.chunkfile object to the newly opened file.
 * If opening the file fails or the file is not opened, sets the response status code to "500 Internal Server Error".
 * */
void    RequestParser::check_chunked() {
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

/*
 * Parse the body length of a chunked request and set it in the _request object.
 * Extracts the body from the _request buffer by taking a substring starting from the end of the request headers.
 * The chunked body length is expressed in hexadecimal in the first line of the body.
 * This line is then converted to an integer with strtol, which provides the length of the chunked message.
 * If the body length is zero, it indicates that the message is complete, so the body_received flag is set to true.
 * The body length is set in the _request object's body_length attribute.
*/
void    RequestParser::get_body_length_chunked() {
    std::string body = _request.buf.substr(_request.head_length);
    _request.body_length = std::strtol(body.c_str(), NULL, 16);
    if (_request.body_length == 0)
        _request.body_received = true;
}

/*
 * This function attempts to extract a cookie from the request buffer and set it in the _request struct.
 * It searches for the substring "Cookie: key=" in the request buffer using the find() function.
 *
 * If it is not found, the function returns without doing anything.
 *
 * If it is found, it advances the search position by the length of the "Cookie: key=" string
 * and uses the atoi() function to convert the substring after the search position to an integer value,
 * which is then assigned to the _request.cookies member variable.
 * */
void	RequestParser::set_cookies() {
	size_t position = _request.buf.find("Cookie: key=");

	if (position == std::string::npos)
		return;
	position += std::strlen("Cookie: key=");
    _request.cookies = std::atoi(_request.buf.substr(position).c_str());
}

/*
 * Is used to split the query string and the file path from the first line of the HTTP request.
 * Is important for processing the query string in the HTTP request and allows the server to correctly route the
 * request to the appropriate handler.
 *
 * First it searches for the position of the ? character in the _request.file_path variable.
 * If the ? character is found, the function extracts the query string from the file path and sets it as the _request.query variable.
 * It also sets the length of the query string as the _request.body_length.
 *
 * If '?' is not found, the _request.query variable is left empty and  only _request.file_path variable is set.

 * */
void	RequestParser::split_query() {
	size_t position = _request.file_path.find('?');

	if (position != std::string::npos) {
        _request.query = _request.file_path.substr(position + 1);
        _request.body_length = _request.query.length();
	}
    _request.file_path = _request.file_path.substr(0, position);
}

/*
 * Is used to determine the type of URL being requested by the client.
 * It does this by splitting the _request.file_path into different locations (using the "/" character as a delimiter)
 * and then analyzing the resulting vector of strings.
 * Helps the server to understand the type of resource being requested and respond accordingly.
 *
 * It first sets _locations to the result of splitting _request.file_path by "/", and then uses a switch statement
 * to determine the _url_type based on the size of the _locations vector.
 *
 * If the size of the _locations vector is 0, then the _url_type is set to SERVER_INDEX,
 * indicating that the server's default index page is being requested.
 *
 * If the size of the _locations vector is 1, then the function checks whether the single location contains a "." character.
 * If it does, then the _url_type is set to SERVER, indicating that a file is being requested.
 * If not, then the _url_type is set to LOCATION_INDEX, indicating that the index page of a location is being requested.
 *
 * If the size of the _locations vector is greater than 1, then the _url_type is set to LOCATION,
 * indicating that a resource located within a specific location is being requested.
 * */
void    RequestParser::set_url_type() {
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

/*
 * This function check_redirection checks whether a location index should be redirected to another URL
 * and updates the request's status and buffer accordingly.
 * */
void    RequestParser::check_redirection() {
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

/*
 * Sets the location configuration for the current request.
 * It first checks if the URL type is a location or a location index, and if not, it returns without doing anything.
 * It then iterates over the location configurations in the server configuration and finds the configuration that matches
 * the prefix of the location in the request.
 *
 * Once a match is found, it sets the location configuration for the request, including the directory listing flag,
 * and updates the file path if necessary.
 *
 * If no match is found, it sets the status of the request to PAGE_NOT_FOUND.
 * */
void    RequestParser::set_location_config() {
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
    _request.status = PAGE_NOT_FOUND;
}

/*
 * Checks whether the requested HTTP method is allowed for the given URL type,
 * and sets the response status to "405 Method Not Allowed" if it is not allowed.
 * */
void	RequestParser::check_method() {
    std::vector<std::string> allowed_methods;

    if (_url_type == LOCATION || _url_type == LOCATION_INDEX)
        allowed_methods = _location_config.methods;
    else
        allowed_methods = _request.socket.get_config().get_methods();
    for (size_t	i = 0; i < allowed_methods.size(); i++) {
        if	(allowed_methods[i] == _request.method)
            return;
    }
    _request.status = METHOD_NOT_ALLOWED;
}

/*
 * Translates the URL path of the incoming HTTP request to the corresponding file path on the server's file system.
 * It does this by checking the _url_type field of the Request object, which specifies whether the request is for
 * the server, a specific file, a location, or a nested file path within a location.
 *
 * Depending on the url type of request, it sets the file_path field  to the appropriate value.
 * If the request is for a nested file path, it concatenates the path components using the / separator.
 * */
void	RequestParser::translate_path() {
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

/*
 * Determines which interpreter to use based on the extension of the requested file.
 * It first checks if the requested URL is a server-level URL, in which case there is no interpreter to set.
 * If it is not a server-level URL, it searches for the last occurrence of the '.' character in the file path, and if found,
 * extracts the extension from that position to the end of the string.
 *
 * It then iterates through the list of scripts defined in the location configuration, looking for a match with the extension.
 * If a match is found, it sets the interpreter for the request to the corresponding value in the configuration.
 * */
void	RequestParser::set_interpreter() {
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

/*
 * Check the validity and accessibility of the requested file path.
 * The access() function is used to check if the file exists or not, and the is_regular_file() function is used to ensure
 * that the file is a regular file and not a directory.
 *
 * If the file does not exist or is not a regular file, sets the status of the request to "PAGE_NOT_FOUND" or "PAYMENT_REQUIRED", respectively.
 * Otherwise, checks whether the file is readable using access() and sets the status to "FORBIDDEN" if it is not.
 * */
void	RequestParser::check_file_path() {
    if (access(_request.file_path.data(), F_OK) == -1)
        _request.status = PAGE_NOT_FOUND;
    else if (!is_regular_file(_request.file_path.data())) {
        _request.status = PAYMENT_REQUIRED;
    }
    else if (access(_request.file_path.data(), R_OK) == -1)
        _request.status = FORBIDDEN;
}

/*
 * This function sets the body length of the request by looking for the Content-Length header in the request buffer.
 * If the HTTP method is GET, it returns early, as GET requests don't have a body.
 * If the Content-Length header is found, extracts the body length from the header and sets it in the _request object.
 * If the body length is zero, it sets the status of the request to NO_CONTENT.
 * */
void RequestParser::set_body_length() {
    if (_request.method == "GET")
        return;

    size_t pos = _request.buf.find("Content-Length: ");
    if (pos != std::string::npos)
        _request.body_length = static_cast<size_t> (std::atoll(_request.buf.data() + pos + std::strlen("Content-Length: ")));
    if (_request.body_length == 0)
        _request.status = NO_CONTENT;
}

/*
 * If the request URL type is a server or server index, the function simply returns.
 * Otherwise, it sets the request's cgi_path member to the cgi_path value defined in the location configuration.
 * The CGI path is then used to find and execute CGI scripts.
 * */
void RequestParser::set_cgi_path() {
    if (_url_type == SERVER || _url_type == SERVER_INDEX)
        return ;
    _request.cgi_path = _location_config.cgi_path;
}