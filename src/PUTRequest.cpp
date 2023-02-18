
#include "PUTRequest.hpp"

//BASIC CLASS SETUP
PUTRequest::PUTRequest(request_handler &request)
        : _request(request), _environment(_request) {}

PUTRequest::~PUTRequest() {}


int PUTRequest::create_response(std::string &response) {
    _request.query = _request.buf.substr(_request.head_length, _request.buf.length());
    _request.cookies = 1;
    _request.file_path = "html/cgi/upload.py";
    _request.interpreter = "/usr/bin/python";
    _environment.create();
    return _cgi.create_response(_request, response);
}