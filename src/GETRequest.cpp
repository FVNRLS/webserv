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

/*
 * Handles a GET request by either generating a simple HTML response or by using a CGI script to create a more complex response.
 * It delegates the responsibility for generating the response to the appropriate method based on whether a CGI script is present or not.
 * */
int GETRequest::create_response(std::string &response) {
	if (_request.interpreter.empty()) {
		return create_html_response(response);
	}
	_environment.create();
	return _cgi.create_response(_request, response);
}

/*
 * Generates an HTML response based on the contents of the requested file path.
 * If directory listing is switched on for the location, uses dir_list_response to generate response.
 * If the file does not exist or cannot be opened, an appropriate error code is returned.
 * If the file is successfully opened, its contents are read into a string and returned as the response.
 * */
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
/*
 * Handles GET requests for directory listing.
 * It opens the specified directory, creates an HTML response that lists the contents, and returns it as a string.
 * If the directory can't be opened, it returns a PAGE_NOT_FOUND error code.
 * */
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