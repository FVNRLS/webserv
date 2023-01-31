//
// Created by Roman Mazuritskiy on 1/31/23.
//

#pragma once

#include "Socket.hpp"

class POSTRequest {

private:
	std::string		&_response;
	std::string 	_body;

	int 			create_response_body(std::string &file_path);

public:
	POSTRequest(std::string &response);
	~POSTRequest();

	int 			create_response(std::string &file_path);
};
