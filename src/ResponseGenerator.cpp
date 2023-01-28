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
ResponseGenerator::ResponseGenerator(pollfd &pfd, const Socket &socket , std::string &request)
	: _pfd(pfd), _socket(socket), _request(request) {}

ResponseGenerator::ResponseGenerator(const ResponseGenerator &src) :
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
	const char 			*file_path;
	std::ifstream 		file;
	std::string 		body;
	std::string 		requested_path;
	std::stringstream 	body_len;

//	requested_path = get_requested_path(request);

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
