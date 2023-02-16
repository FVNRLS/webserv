
#include "POSTRequest.hpp"

//BASIC CLASS SETUP
POSTRequest::POSTRequest(request_handler &request, Session &cookies)
        : _request(request), _environment(_request), _cookie(cookies) {}

POSTRequest::~POSTRequest() {}

// TODO check max client body size!!

int POSTRequest::create_response(std::string &response) {
	_request.query = _request.buf.substr(_request.head_length, _request.buf.length());
	check_for_cookies();
	_environment.create();
	return _cgi.create_response(_request, response);
}

int POSTRequest::check_for_cookies() {
	size_t name = _request.query.find("username=");
	if (name == std::string::npos)
		return EXIT_FAILURE;
	name += std::strlen("username=");

	size_t end = _request.query.find('&', name);
	if (end == std::string::npos)
		return EXIT_FAILURE;

	size_t pass = _request.query.find("password=");
	if (pass == std::string::npos)
		return EXIT_FAILURE;
	pass += std::strlen("password=");

	_request.cookies = _cookie.create(_request.query.substr(name, end - name), _request.query.substr(pass));
	return EXIT_SUCCESS;
}