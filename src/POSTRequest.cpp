
#include "POSTRequest.hpp"

//BASIC CLASS SETUP
POSTRequest::POSTRequest() {}

POSTRequest::~POSTRequest() {}

//MEMBER FUNCTIONS
int POSTRequest::create_response(const request_handler &request) {
	std::ofstream file;

	_body = request.buf.substr(request.head_length, request.buf.length());
	if (_body.find("Content-Disposition: form-data") != std::string::npos)
		return (upload_file());
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

int POSTRequest::upload_file() {
	std::ofstream file;
	size_t begin = _body.find("filename=\"") + std::strlen("filename=\"");
	if (begin == std::string::npos)
		return BAD_REQUEST;
	begin += std::strlen("filename=\"");
	size_t end = _body.find("\"", begin);
	if (end == std::string::npos)
		return BAD_REQUEST;
	_filepath = DEFAULT_UPLOAD_DIR + _body.substr(begin, end - begin);
	file.open(_filepath, std::ios::out | std::ios::trunc);
	if (!file.is_open() || file.fail())
		return INTERNAL_SERVER_ERROR;
	begin = _body.find("\r\n\r\n", begin) + std::strlen("\r\n\r\n");
	end = _body.find("\r\n", begin);
	if (begin == std::string::npos || end == std::string::npos)
		return BAD_REQUEST;
	file << _body.substr(begin, end - begin);
	return  EXIT_SUCCESS;
}
