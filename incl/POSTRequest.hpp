//
// Created by Roman Mazuritskiy on 1/31/23.
//

#pragma once

#include "requestHandler.hpp"

class POSTRequest {

private:
	std::string 	_body;

public:
	POSTRequest();
	~POSTRequest();

	int 			create_response(const request_handler &request);
};
