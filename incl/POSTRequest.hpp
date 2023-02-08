//
// Created by Roman Mazuritskiy on 1/31/23.
//

#pragma once

#include "Env.hpp"
#include "CGI.hpp"
#include "Session.hpp"

class POSTRequest {

private:
	CGI					_cgi;
	request_handler&	_request;
	Env					_environment;
	Session				_cookie;

	int		check_for_cookies();
	int		set_interpreter_path();
public:
	POSTRequest(request_handler &request);
	~POSTRequest();

	int		create_response(std::string &response);
};
