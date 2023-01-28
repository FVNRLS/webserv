/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:20:38 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/26 12:20:38 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

//BASIC CLASS SETUP
ResponseGenerator::ResponseGenerator(pollfd &pfd, const Socket &socket, std::string &request) :
		_pfd(pfd), _socket(socket), _request(request) {}

ResponseGenerator::ResponseGenerator(std::vector<Socket> &sockets, const ResponseGenerator &src) :
		_socket(src._socket), _request(src._request) { *this = src; }

ResponseGenerator	&ResponseGenerator::operator=(const ResponseGenerator &src) {
	if (this == &src)
		return (*this);
	_pfd = src._pfd;
	_response = src._response;
	_body = src._body;
	return (*this);
}

ResponseGenerator::~ResponseGenerator() {}


//MEMBER FUNCTIONS
std::string ResponseGenerator::generate_response() {
	std::string 		file_path;
	std::ifstream 		file;

//	requested_path = get_requested_path(request);

	if (check_max_client_body_size() == EXIT_FAILURE)
		return (generate_error_code_response(BAD_REQUEST));

	file_path = _socket.get_config().get_index();
	if (get_response_body(file_path, file) == EXIT_FAILURE)
		return (EMPTY_STRING);

	_response = RESPONSE_HEADER + _body_len.str() + "\n\n" + _body;
	file.close();

	return (_response);
}

int	ResponseGenerator::check_max_client_body_size() {
	long long 			max_client_body_size;

	max_client_body_size = _socket.get_config().get_max_client_body_size();
	if (static_cast<long long>(_request.length()) > max_client_body_size)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int ResponseGenerator::get_response_body(std::string &file_path, std::ifstream &file) {
	if (open_file(file_path, file) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	_body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	_body_len << _body.length();
	return (EXIT_SUCCESS);
}

int ResponseGenerator::open_file(std::string &file_path, std::ifstream &file) {
	if (access(file_path.c_str(), F_OK) < 0)
		return (system_call_error(ACCESS_DENIED, _socket));
	file.open(file_path);
	if (!file.is_open() || file.fail())
		return (system_call_error(ACCESS_DENIED, _socket));
	return (EXIT_SUCCESS);
}

std::string	ResponseGenerator::get_requested_path() {

	return ("");
}


//ERROR MANAGEMENT
int ResponseGenerator::system_call_error(int error, const Socket &socket) {
	switch(error) {
		case ACCESS_DENIED:
			std::cerr << "Error: Access Denied" << std::endl;
			break;
		default:
			std::cerr << "Server: Unknown Error" << std::endl;
	}
	return (EXIT_FAILURE);
}

std::string	ResponseGenerator::generate_error_code_response(int error) {
	std::ifstream 		file;
	std::stringstream	error_code;
	std::string 		error_file;
	std::string 		error_page_path;
	std::string 		body;
	std::stringstream 	body_len;
	std::string 		response;

	error_code << error;
	error_file = error_code.str() + ".html";
	error_page_path = _socket.get_config().get_error_pages_dir() + error_file;

	if (open_file(error_page_path, file) == EXIT_FAILURE)
		return (EMPTY_STRING);

	body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	body_len << body.length();
	response = RESPONSE_HEADER + body_len.str() + "\n\n" + body;
	file.close();
	return (response);
}

