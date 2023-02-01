//
// Created by Roman Mazuritskiy on 1/31/23.
//

#pragma once

#include "requestHandler.hpp"

class POSTRequest {

private:
	request_handler &_request;
	std::string 	_body;

public:
	POSTRequest(request_handler &request);
	~POSTRequest();

	int 			create_response();
};
