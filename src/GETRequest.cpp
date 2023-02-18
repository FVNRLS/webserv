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

int GETRequest::create_html_response(std::string &response) {
	std::ifstream	file;
	std::string		body;

    if (_request.dir_list)
        return dir_list_response(response);
	if (access(_request.file_path.c_str(), F_OK) < 0)
		return PAGE_NOT_FOUND;
    if (open_file(_request.file_path, file) == EXIT_FAILURE)
		return FORBIDDEN;
	body.append((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	response = body;
	return EXIT_SUCCESS;
}
int GETRequest::dir_list_response(std::string &response) {
    DIR* dir = opendir(_request.file_path.c_str());

    if (dir == NULL)
        return PAGE_NOT_FOUND;

    std::stringstream html;
    html << "<html><head><title>Directory listing for requested location! </title></head><body>";
    html << "<h1>Directory listing for requested location!" << "</h1>";

    dirent* entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_name[0] != '.')
            html << "<p>" << entry->d_name << "</p>";
        entry = readdir(dir);
    }
    html << "</body></html>";
    closedir(dir);
    response = html.str();
    return EXIT_SUCCESS;
}