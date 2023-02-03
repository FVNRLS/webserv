//
// Created by Roman Mazuritskiy on 1/31/23.
//

#pragma once

#include "requestHandler.hpp"

class POSTRequest {

private:
	request_handler&	_request;
	std::string 		_body;


	void				create_env();
	void				set_script_path();
	void				set_interpreter_path();
	std::string			http_user_agent();
	std::string 		remote_addr();


public:
	POSTRequest(request_handler &request);
	~POSTRequest();

	int 			create_response();
};
