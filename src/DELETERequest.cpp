#include "DELETERequest.hpp"

//BASIC CLASS SETUP
DELETERequest::DELETERequest() {}

DELETERequest::~DELETERequest() {}


//MEMBER FUNCTIONS
int DELETERequest::create_response(const request_handler &request) {
	std::string	filepath;

	_filename = request.buf.substr(request.head_length, request.buf.length());
	filepath = DEFAULT_UPLOAD_DIR + _filename;
	if (access(filepath.c_str(), F_OK) < 0 )
		return NOT_FOUND;
	if (remove(filepath.c_str()) != 0) {
		return FORBIDDEN;
	}
	return EXIT_SUCCESS;
}