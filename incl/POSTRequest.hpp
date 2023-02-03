//
// Created by Roman Mazuritskiy on 1/31/23.
//

#pragma once

#include "requestHandler.hpp"
#include "CGI.hpp"

class POSTRequest {

private:
	CGI					_cgi;
	request_handler&	_request;
	std::string 		_body;


	void				create_env();
	void				set_script_path();
	int 				set_interpreter_path();
	std::string			http_user_agent();
	std::string 		remote_addr();


public:
	POSTRequest(request_handler &request);
	~POSTRequest();

	int 			create_response(std::string &response);
};
