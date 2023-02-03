/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseGenerator.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:20:38 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/03 17:14:14 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

//BASIC CLASS SETUP
ResponseGenerator::ResponseGenerator(request_handler &request) : _request(request) {}

ResponseGenerator::ResponseGenerator(ResponseGenerator &src) : _request(src._request) {}

ResponseGenerator::~ResponseGenerator() {}


//MEMBER FUNCTIONS
std::string ResponseGenerator::generate_response() {
	if (_request.status == EXIT_SUCCESS) {
        if (_request.method == "GET") {
            GETRequest get;
            _request.status = get.create_response(_request.file_path, _response);
        }
        else if (_request.method == "POST") {
            POSTRequest post(_request);
            _request.status = post.create_response(_response);
        }
        else if (_request.method == "DELETE") {
            DELETERequest delete_method;
            _request.status = delete_method.create_response(_request);
        }
        else
            _request.status = METHOD_NOT_ALLOWED;
    }
	if (_request.status)
		return (create_error_code_response(_request.status));
	return (_response);
}

//ERROR MANAGEMENT
std::string ResponseGenerator::create_error_code_response(int error) {
	std::ifstream 			file;
	std::string 			error_page_path;
	std::string 			body;

	error_page_path = _request.socket.get_config().get_error_pages_dir() + toString<int>(error) + ".html";
	if (access(error_page_path.c_str(), F_OK) < 0 || open_file(error_page_path, file) == EXIT_FAILURE) {
		_response = RESPONSE_HEADER + toString<size_t>(DEFAULT_PAGE_ERROR.length()) + "\n\n" + DEFAULT_PAGE_ERROR;
		return (_response);
	}
	body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	_response = RESPONSE_HEADER + toString<size_t>(body.length()) + "\n\n" + body;
	return (_response);
}


//TODO: FOR LATER > encode html with picture nd find right type!