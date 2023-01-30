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

GETRequest::GETRequest(std::string &response) :  _response(response) {}

GETRequest::~GETRequest() {}


int GETRequest::create_response(std::string &file_path) {
	std::stringstream 	body_len;
	int 				exit_code;

	exit_code = create_response_body(file_path);
	if (exit_code == EXIT_SUCCESS) {
		body_len << _body.length();
		_response = RESPONSE_HEADER + body_len.str() + "\n\n" + _body;
	}
	return exit_code;
}


int GETRequest::create_response_body(std::string &file_path) {
	std::ifstream 		file;

	if (access(file_path.c_str(), F_OK) < 0)
		return PAGE_NOT_FOUND;
	if (open_file(file_path, file) == EXIT_FAILURE)
		return (FORBIDDEN);
	_body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return (EXIT_SUCCESS);
}