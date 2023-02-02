
#include "POSTRequest.hpp"

//BASIC CLASS SETUP
POSTRequest::POSTRequest() {}

POSTRequest::~POSTRequest() {}


//MEMBER FUNCTIONS
int POSTRequest::create_response(const request_handler &request) {
	std::ofstream file;
	_body = request.buf.substr(request.head_length, request.buf.length());

	if (_body == "delete_all") {
		file.open(DATABASE_PATH, std::ios::out | std::ios::trunc);
		return EXIT_SUCCESS;
	}
		//remove file & return;
	file.open(DATABASE_PATH, std::ios::out | std::ios::app);
	if (!file.is_open() || file.fail())
		return INTERNAL_SERVER_ERROR;
	file << _body << '\n';
	return EXIT_SUCCESS;
}