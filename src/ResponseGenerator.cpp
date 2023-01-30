/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseGenerator.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:20:38 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/30 11:35:32 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

//BASIC CLASS SETUP
ResponseGenerator::ResponseGenerator(const Socket &socket, std::string &request) :
		_socket(socket), _request(request) {

	//VECTOR OF VALID LOCATIONS FOR PATH REQUEST
	_valid_locs.push_back(socket.get_config().get_index());
	_valid_locs.push_back(socket.get_config().get_root());
	for (size_t i = 0; i < socket.get_config().get_locations().size(); i++) {
		_valid_locs.push_back(socket.get_config().get_locations()[i].root);
		_valid_locs.push_back(socket.get_config().get_locations()[i].cgi_path);
	}
}

ResponseGenerator::ResponseGenerator(const ResponseGenerator &src) :
		_socket(src._socket), _request(src._request) { *this = src; }

ResponseGenerator	&ResponseGenerator::operator=(const ResponseGenerator &src) {
	if (this == &src)
		return (*this);
	_response = src._response;
	_method = src._method;
	return (*this);
}

ResponseGenerator::~ResponseGenerator() {}


//MEMBER FUNCTIONS
std::string ResponseGenerator::generate_response() {
	std::string 				file_path;
	std::vector<std::string>	tokens;
	int 						exit_code;

	if (check_max_client_body_size() == EXIT_FAILURE)
		return (create_error_code_response(BAD_REQUEST));

	tokenize_first_line(tokens);
	if (tokens.size() < 3)
		return (create_error_code_response(BAD_REQUEST));
	exit_code = select_method(tokens);
	if (exit_code == EXIT_SUCCESS)
		return (_response);
	return (create_error_code_response(exit_code));
}

int	ResponseGenerator::check_max_client_body_size() {
	long long 			max_client_body_size;

	max_client_body_size = _socket.get_config().get_max_client_body_size();
	if (static_cast<long long>(_request.length()) > max_client_body_size)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

std::vector<std::string>	ResponseGenerator::tokenize_first_line(std::vector<std::string> &tokens) {
	std::string 				first_request_line;
	size_t 						nl_pos;

	nl_pos = _request.find(NEWLINE);
	if (nl_pos != std::string::npos) {
		first_request_line =  _request.substr(0, nl_pos);
	}
	tokens = split(first_request_line, SPACE);
	return (tokens);
}

int ResponseGenerator::select_method(const std::vector<std::string> &tokens) {
	std::string 								requested_path;
	const std::vector<std::string>				&allowed_methods = _socket.get_config().get_methods();
	std::vector<std::string>::const_iterator	it;
	std::string 								full_path;

	_method = tokens[0];
	requested_path = tokens[1];
	full_path = get_full_location_path(requested_path);

	it = std::find(allowed_methods.begin(), allowed_methods.end(), _method);
	if (it == allowed_methods.end())
		return (EXIT_FAILURE);
	if (*it == "GET") {
		GETRequest	parser(_response);
		return (parser.create_response(full_path));
	}
	//todo: complete with post and delete!
	return (EXIT_FAILURE);
}

std::string ResponseGenerator::get_full_location_path(std::string &file_path) {
	if (file_path == "/") {
		file_path = _socket.get_config().get_index();
		return (file_path);
	}
	for (size_t i = 0; i < _valid_locs.size(); i++) {
		if (std::equal(file_path.rbegin(), file_path.rend(), _valid_locs[i].rbegin())) {
			file_path = _valid_locs[i];
			break;
		}
	}
	return (file_path);
}


//ERROR MANAGEMENT
std::string ResponseGenerator::create_error_code_response(int error) {
	std::ifstream 			file;
	std::stringstream 		error_code;
	std::string 			error_file;
	std::string 			error_page_path;
	std::string 			body;
	std::stringstream 		body_len;
	std::string 			response;

	error_code << error;
	error_file = error_code.str() + ".html";
	error_page_path = _socket.get_config().get_error_pages_dir() + error_file;

	if (access(error_page_path.c_str(), F_OK) < 0 || open_file(error_page_path, file) == EXIT_FAILURE) {
		_response = DEFAULT_PAGE_ERROR;
		file.close();
		return (_response);
	}
	body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	body_len << body.length();
	_response = RESPONSE_HEADER + body_len.str() + "\n\n" + body;
	file.close();
	return (_response);
}


//TODO: FOR LATER > encode html with picture nd find right type!