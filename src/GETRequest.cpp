/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETRequest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 16:47:22 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/30 16:47:22 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GETRequest.hpp"

//BASIC CLASS SETUP
GETRequest::GETRequest() {}

GETRequest::~GETRequest() {}


//MEMBER FUNCTIONS
int GETRequest::create_response(const std::string &file_path, std::string &_response) {
	int exit_code = create_response_body(file_path);
	if (exit_code == EXIT_SUCCESS) {
		_response = RESPONSE_HEADER + toString<size_t>(_body.length()) + "\n\n" + _body;
	}
	return exit_code;
}


int GETRequest::create_response_body(const std::string &file_path) {
	std::ifstream	file;

	if (access(file_path.c_str(), F_OK) < 0)
		return PAGE_NOT_FOUND;
	if (open_file(file_path, file) == EXIT_FAILURE)
		return FORBIDDEN;
	_body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return EXIT_SUCCESS;
}