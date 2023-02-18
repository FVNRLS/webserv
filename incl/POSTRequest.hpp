#pragma once

#include "Env.hpp"
#include "CGI.hpp"
#include "Session.hpp"

class POSTRequest {

private:
	CGI					_cgi;
	request_handler&	_request;
	Env					_environment;
	Session				&_cookie;

	int		check_for_cookies();

public:
	POSTRequest(request_handler &request, Session &cookies);
	~POSTRequest();

	int		create_response(std::string &response);
};
