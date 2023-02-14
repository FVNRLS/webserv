#pragma once

#include "request_handler.hpp"

class CGI {
private:
	int			_response_fd;

	int		error(const char* message);
	void	child_process(const request_handler &request);
	int		parent_process();
	int		write_response(std::string &response);
	int		dup_request_to_stdin(const request_handler& request);

public:
	CGI();
	~CGI();

	int			create_response(const request_handler &request, std::string &response);
};