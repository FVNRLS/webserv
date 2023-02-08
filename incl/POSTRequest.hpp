//
// Created by Roman Mazuritskiy on 1/31/23.
//

#pragma once

#include "Env.hpp"
#include "CGI.hpp"

class POSTRequest {

private:
	CGI					_cgi;
	request_handler&	_request;
	Env					_environment;

	int 	set_interpreter_path();
public:
	POSTRequest(request_handler &request);
	~POSTRequest();

	int		create_response(std::string &response);
};
