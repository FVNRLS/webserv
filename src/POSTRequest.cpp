
#include "POSTRequest.hpp"

//BASIC CLASS SETUP
POSTRequest::POSTRequest(request_handler &request, Session &cookies)
        : _request(request), _environment(_request), _cookie(cookies) {}

POSTRequest::~POSTRequest() {}

/*
 * Handles all POST requests through CGI and creates respective response, setting up the environment and using cookies.
 * */
int POSTRequest::create_response(std::string &response) {
	if (_request.query.empty())
        _request.query = _request.buf.substr(_request.head_length, _request.buf.length());
	check_for_cookies();
	_environment.create();
	return _cgi.create_response(_request, response);
}

/*
 * Checks if the POST request should be handled with cookies and applies them if needed.
 *
 * Checks the _request object's query string for a username and password parameter.
 * If they are found, it creates a new cookie object and sets it in the _request object's cookies attribute.
 *
 * First searches for the username parameter in the request.
 * If the parameter is not found, the function returns EXIT_FAILURE.
 * If the parameter is found, advances the search position by the length of the username= string to find the end of the parameter value.
 * Then searches for the password parameter in a similar way, and if it is not found, the function returns EXIT_FAILURE.
 *
 * If both parameters are found, creates a new cookie object  with the username and password substrings as parameters.
 * Then sets this cookie in the _request object's cookies attribute.
 *
 * Returns EXIT_SUCCESS if the cookie creation was successful, indicating that the cookies were found and set correctly.
 * */
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