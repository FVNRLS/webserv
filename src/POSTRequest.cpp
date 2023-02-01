
#include "POSTRequest.hpp"

//BASIC CLASS SETUP
POSTRequest::POSTRequest(request_handler &request) : _request(request) {}

POSTRequest::~POSTRequest() {}


//MEMBER FUNCTIONS
int POSTRequest::create_response() {
	std::ofstream file;

	file.open(DATABASE_PATH, std::ios::out | std::ios::app);
	if (!file.is_open() || file.fail())
		return INTERNAL_SERVER_ERROR;
	_body = _request.buf.substr(_request.head_length, _request.buf.length());
	file << _body;
	return OK;
}

