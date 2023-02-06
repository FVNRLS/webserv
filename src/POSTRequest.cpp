
#include "POSTRequest.hpp"

//BASIC CLASS SETUP
POSTRequest::POSTRequest(request_handler &request) : _request(request) {}

POSTRequest::~POSTRequest() {}


int POSTRequest::create_response(std::string &response) {
	_request.query = _request.buf.substr(_request.head_length, _request.buf.length());
	
	_environment.create();
	return (_cgi.create_response(_request, response));
}