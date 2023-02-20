/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseGenerator.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoomen <hoomen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:20:38 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/14 16:56:24 by hoomen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

// BASIC CLASS SETUP
ResponseGenerator::ResponseGenerator(request_handler &request)
    : _request(request) {}

ResponseGenerator::ResponseGenerator(ResponseGenerator &src)
    : _request(src._request) {}

ResponseGenerator::~ResponseGenerator() {}

// MEMBER FUNCTIONS

/*
 * Generates a response for an HTTP request.
 *
 * It first checks the method of the request and creates the appropriate request object (e.g. GETRequest, POSTRequest, or DELETERequest).
 * The create_response() method of the request object is then called to generate the response body,
 * and the response status is set based on the result.
 * If there was an error, it creates an error code response, otherwise, it generates the response header and returns the concatenated response.
 * */
std::string ResponseGenerator::generate_response(Session &cookie) {
    if (_request.status == EXIT_SUCCESS) {
        if (_request.method == "GET") {
            GETRequest get(_request);
            _request.status = get.create_response(_response_body);
        }
        else if (_request.method == "POST") {
            POSTRequest post(_request, cookie);
            _request.status = post.create_response(_response_body);
        }
        else if (_request.method == "DELETE") {
            DELETERequest delete_method;
            _request.status = delete_method.create_response(_request);
        }
        else
          _request.status = METHOD_NOT_ALLOWED;
    }
    if (_request.status)
        return create_error_code_response(_request.status);
  return generate_response_header(_request.status) + _response_body;
}

/*
 * Creates a response body for HTTP error codes.
 *
 * It first checks if there is a custom error page for the specific error code, and if there is,
 * it reads the content of the file and adds it to the response body.
 * If there is no custom error page, it sets a default error page to the response body.
 *
 * If the HTTP method is HEAD, it only generates the response header and returns it.
 * Finally, it returns the response header and the response body concatenated for all other HTTP methods.
 * */
std::string ResponseGenerator::create_error_code_response(int status_code) {
  std::ifstream file;
  std::string error_page_path;

  if (_request.status == MOVED_PERMANENTLY)
      return redirection_response(status_code);
  error_page_path = _request.socket.get_config().get_error_pages_dir() +
                    toString(status_code) + ".html";
  if (access(error_page_path.c_str(), R_OK) < 0 ||
      open_file(error_page_path, file) == EXIT_FAILURE) {
      _response_body = DEFAULT_PAGE_ERROR;
      return generate_response_header(status_code) + _response_body;
  }
  _response_body.append((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
  if (_request.method == "HEAD")
      return generate_response_header(status_code);
  return generate_response_header(status_code) + _response_body;
}

/*
 * Generates a redirection response using the specified status code, reason phrase and location.
 * Returns a string that includes the HTTP version, status code, phrase, "Location" header and the end of request marker.
 * */
std::string ResponseGenerator::redirection_response(int status_code) {
    return  "HTTP/1.1 " + toString(status_code) + " " +
            _reasonPhrases.find(status_code)->second +
            "\r\nLocation: " + _request.buf + END_OF_REQUEST;
}

/*
 * Generates the response header for the HTTP response.
 *
 * It takes an integer status code as an argument and uses it to determine the HTTP status code for the response.
 * If the status code is EXIT_SUCCESS, it is converted to the OK status code.
 *
 * Then checks if there are any cookies to be set in the response, and if so,
 * it includes them in the header using the Set-Cookie field.
 *
 * Finally, returns the HTTP response header as a string, which includes the HTTP version,
 * the status code, the reason phrase for the status code, the content type (always text/html),
 * the length of the response body, and any cookies that were set.
 * The header ends with the special sequence "\r\n\r\n", which indicates the end of the header and the start of the response body.
 * */
std::string ResponseGenerator::generate_response_header(int status_code) {
	if (status_code == EXIT_SUCCESS)
		status_code = OK;
	std::string cookies;
	if (_request.cookies)
		cookies = "\r\nSet-Cookie: key=" + toString<int>(_request.cookies);
	
    return  "HTTP/1.1 " + toString(status_code) + " " +
            _reasonPhrases.find(status_code)->second +
            "\r\nContent-Type: text/html\r\nContent-Length: " +
            toString(_response_body.length()) +
            cookies + END_OF_REQUEST;
}



const std::map<int, std::string> ResponseGenerator::_reasonPhrases = make_pairs();

/*
 * Pairs of status codes and respective phrases to form a response header.
 * */
const std::map<int, std::string> ResponseGenerator::make_pairs() {
    std::map<int, std::string> pairs;
    pairs[100] = "Continue";
    pairs[101] = "Switching Protocols";
    pairs[200] = "OK";
    pairs[201] = "Created";
    pairs[202] = "Accepted";
    pairs[203] = "Non-Authoritative Information";
    pairs[204] = "No Content";
    pairs[205] = "Reset Content";
    pairs[206] = "Partial Content";
    pairs[300] = "Multiple Choices";
    pairs[301] = "Moved Permanently";
    pairs[302] = "Found";
    pairs[303] = "See Other";
    pairs[304] = "Not Modified";
    pairs[305] = "Use Proxy";
    pairs[307] = "Temporary Redirect";
    pairs[400] = "Bad Request";
    pairs[401] = "Unauthorized";
    pairs[402] = "Payment Required";
    pairs[403] = "Forbidden";
    pairs[404] = "Not Found";
    pairs[405] = "Method Not Allowed";
    pairs[406] = "Not Acceptable";
    pairs[407] = "Proxy Authentication Required";
    pairs[408] = "Request Time-out";
    pairs[409] = "Conflict";
    pairs[410] = "Gone";
    pairs[411] = "Length Required";
    pairs[412] = "Precondition Failed";
    pairs[413] = "Request Entity Too Large";
    pairs[414] = "Request-URI Too Large";
    pairs[415] = "Unsupported Media Type";
    pairs[416] = "Requested range not satisfiable";
    pairs[417] = "Expectation Failed";
    pairs[500] = "Internal Server Error";
    pairs[501] = "Not Implemented";
    pairs[502] = "Bad Gateway";
    pairs[503] = "Service Unavailable";
    pairs[504] = "Gateway Time-out";
    pairs[505] = "HTTP Version not supported";
    return pairs;
};
