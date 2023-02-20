#pragma once

#include "Socket.hpp"

/*
 * Stores parameters of every request.
 * Is stored in the _requests map as 'second' value, which is a private attribute of Server class.
 * */
struct request_handler {
	Socket							socket;								//reference to the Socket class with getter access to its unique configuration
	std::string						buf;								//here a request is accumulated, using recv() - head + body
	std::string						method;								//parsed request method - is set in request headers
	std::string						file_path;							//parsed requested file path - is set request in headers
	bool							body_received;						//indicates end of request
	bool							head_received;						//indicates that HEAD is parsed to search for another indicator to parse (or not) the body
	int								status;								//
	size_t							body_length;						//used to separate HEAD from BODY
	size_t							head_length;						//used to separate HEAD from BODY
	std::vector<std::string>		env;								//used to store environmental vars for execve() through CGI
	std::string						interpreter;						//parsed requested script file - should match with allowed scripts identifiers in config file
	std::string						query;								//
	int								cookies;							//indicates usage of cookies
    bool                            response_sent;						//
    std::string                     response;							//stored response from ResponseGenerator
    size_t                          bytes_sent;							//number of response bytes sent to client with send()
    std::string                     cgi_path;							//path to directory to search for CGI script
    bool                            chunked;							//indicates if request be parsed in chunks ("transfer encoding" found in headers).
    bool                            chunk_complete;						//indicates end of received chunk
    size_t                          chunk_length;						//value parsed from headers during receiving of chunked request
    std::string                     user_agent;							//
    std::string                     content_type;						//
    std::ofstream                   chunkfile;							//temporary file to store the chunked information
    bool                            dir_list;							//directory listing parameter

    request_handler ();
    request_handler(const request_handler &src);
    request_handler& operator= (const request_handler &rhs);

    void                            clear();
};
