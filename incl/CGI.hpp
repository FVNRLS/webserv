#pragma once

#include "requestHandler.hpp"

class CGI {
private:

	int			error_catched(const char* message);
	void		child_process(int *fd, const request_handler &request);
	int			parent_process();
	int 		write_response(int fd, std::string &response);
    int         create_tmp_file(const request_handler& request);
	
public:
	CGI();
	~CGI();

	int 		create_response(const request_handler &request, std::string &response);
};