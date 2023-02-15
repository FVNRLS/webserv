#pragma once

#include "Env.hpp"
#include "CGI.hpp"
#include "Session.hpp"

class PUTRequest {

private:
    CGI					_cgi;
    request_handler&	_request;
    Env					_environment;

public:
    PUTRequest(request_handler &request);
    ~PUTRequest();

    int		create_response(std::string &response);
};
