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
	return (*this);
}

ResponseGenerator::~ResponseGenerator() {}


//MEMBER FUNCTIONS

std::string ResponseGenerator::generate_response() {
	std::string 		response;
	long long 			max_client_body_size;
	const char 			*file_path;
	std::ifstream 		file;
	std::string 		body;
	std::string 		requested_path;
	std::stringstream 	body_len;

//	requested_path = get_requested_path(request);

	max_client_body_size = _socket.get_config().get_max_client_body_size();
	if (static_cast<long long>(_request.length()) > max_client_body_size)
		return (generate_error_code_response(BAD_REQUEST));

	file_path = DEFAULT_INDEX_PAGE.c_str();
	//file_path = ("/Users/rmazurit/Documents/42_Projects/webserv/config/error_pages/400.html"); //just for quick testing of pages
	//	std::cout << file_path << std::endl;

	if (access(file_path, F_OK) < 0) {
		system_call_error(PAGE_NOT_FOUND, _socket);
		return (""); //todo: add error.html
	}
	file.open(file_path);
	if (!file.is_open() || file.fail()) {
		system_call_error(ACCESS_DENIED, _socket);
		return (""); //todo: add error.html
	}
	body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	body_len << body.length();
	response = RESPONSE_HEADER + body_len.str() + "\n\n" + body;

	file.close();

	return ("response");
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
	if (access(error_page_path.c_str(), F_OK) < 0) {
		system_call_error(ACCESS_DENIED, _socket);
		return ("");
	}
	file.open(error_page_path);
	if (!file.is_open() || file.fail()) {
		system_call_error(ACCESS_DENIED, _socket);
		return ("");
	}

	body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	body_len << body.length();
	response = RESPONSE_HEADER + body_len.str() + "\n\n" + body;
	file.close();
	return (response);
}

