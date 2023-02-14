#pragma once

#include "Socket.hpp"

struct request_handler { //todo: delete item from map
	Socket							socket;
	std::string						buf;
	std::string						method;
	std::string						file_path;
	bool							body_received;
	bool							head_received;
	int								status;
	size_t							body_length;
	size_t							head_length;
	std::vector<std::string>		env;
	std::string						interpreter;
	std::string						query;
	int								cookies;

    request_handler ();
    request_handler(const request_handler &src);
    request_handler& operator= (const request_handler &rhs);
};
