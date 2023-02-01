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
ResponseGenerator::ResponseGenerator(request_handler &request) : _request(request) {}

ResponseGenerator::ResponseGenerator(ResponseGenerator &src) : _request(src._request) {}

ResponseGenerator::~ResponseGenerator() {}


//MEMBER FUNCTIONS
std::string ResponseGenerator::generate_response() {
	if (_request.status)
		return (create_error_code_response(_request.status));
	if (_request.method == "GET") {
		GETRequest   get;
		_request.status = get.create_response(_request.file_path, _response);
	}
	else if (_request.method == "POST") {
		std::cout << "POST_METHOD CALLED!\n";
	}
	else {
		std::cout << "DELETE_METHOD CALLED!\n";
	}
	return (_response);
}

std::string ResponseGenerator::get_request_body() {
	return (EMPTY_STRING);
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
	error_page_path = _request.socket.get_config().get_error_pages_dir() + error_file;

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