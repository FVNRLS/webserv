#include "DELETERequest.hpp"

//BASIC CLASS SETUP
DELETERequest::DELETERequest() {}

DELETERequest::~DELETERequest() {}


//MEMBER FUNCTIONS
int DELETERequest::create_response(const request_handler &request) {
	std::string	filepath;

	size_t begin = request.buf.find("name=\"");
	if (begin == std::string::npos)
		return BAD_REQUEST;
	begin += std::strlen("name=\"");
	size_t end = request.buf.find("\"", begin);
	if (end == std::string::npos)
		return BAD_REQUEST;
	filepath = "uploads" + request.buf.substr(begin, end - begin);

	if (access(filepath.c_str(), F_OK) < 0 )
		return PAGE_NOT_FOUND;
	if (unlink(filepath.c_str()) == -1)
		return FORBIDDEN;

	return OK;
}