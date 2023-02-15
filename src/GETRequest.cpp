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

#include "sys/stat.h"

//BASIC CLASS SETUP
GETRequest::GETRequest(request_handler& request) : _request(request), _environment(request) {}

GETRequest::~GETRequest() {}


//MEMBER FUNCTIONS
int GETRequest::create_response(std::string &response) {
	if (_request.interpreter.empty()) {
		return create_html_response(response);
	}
	_environment.create();
	return _cgi.create_response(_request, response);
}

int MyisDir(const char* fileName)
{
    struct stat path;

    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}

int GETRequest::create_html_response(std::string &response) {
	std::ifstream	file;
	std::string		body;

	if (access(_request.file_path.c_str(), F_OK) < 0)
		return PAGE_NOT_FOUND;
//    if (MyisDir(_request.file_path.c_str()))
//        return PAGE_NOT_FOUND;
    if (open_file(_request.file_path, file) == EXIT_FAILURE)
		return FORBIDDEN;
	body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	response = body;
	return EXIT_SUCCESS;
}
