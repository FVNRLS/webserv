#pragma once

#include "requestHandler.hpp"

class CGI {
private:

	int			error_catched(const char* message);
	void		child_process(int *fd, const request_handler &request);
	int			parent_process();
	void 		write_response(int fd, std::string &response);
	
public:
	CGI();
	~CGI();

	int 		create_response(const request_handler &request, std::string &response);
};