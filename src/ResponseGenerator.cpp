/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseGenerator.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoomen <hoomen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:20:38 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/07 15:31:26 by hoomen           ###   ########.fr       */
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
std::string ResponseGenerator::generate_response() {
  if (_request.status == EXIT_SUCCESS) {
    if (_request.method == "GET") {
      GETRequest get(_request);
      _request.status = get.create_response(_response_body);
    }
    else if (_request.method == "POST") {
      POSTRequest post(_request);
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

// ERROR MANAGEMENT
std::string ResponseGenerator::create_error_code_response(int status_code) {
  std::ifstream file;
  std::string error_page_path;

  error_page_path = _request.socket.get_config().get_error_pages_dir() +
                    toString(status_code) + ".html";
  if (access(error_page_path.c_str(), F_OK) < 0 ||
      open_file(error_page_path, file) == EXIT_FAILURE) {
    _response_body = DEFAULT_PAGE_ERROR;
    return generate_response_header(status_code) + _response_body;
  }
  _response_body.append((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
  return generate_response_header(status_code) + _response_body;
}

std::string ResponseGenerator::generate_response_header(int status_code) {
  std::string status_code_str = toString(status_code);
  std::string body_length = toString(_response_body.length());
  std::cerr << _reasonPhrases.find(status_code)->second << '\n';
  return "HTTP/1.1 " + status_code_str + " " + _reasonPhrases.find(status_code)->second +
         "\nContent-Type: text/html\nContent-Length: " + body_length +
         END_OF_REQUEST;
}

const std::map<int, std::string> ResponseGenerator::_reasonPhrases = make_pairs();

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
  pairs[301] = "301 Moved Permanently";
  pairs[400] = "400 Bad Request";
  pairs[401] = "401 Unauthorized";
  pairs[403] = "403 Forbidden";
  pairs[404] = "404 Not Found";
  pairs[405] = "405 Method Not Allowed";
  pairs[411] = "411 Length Required";
  pairs[413] = "413 Payload Too Large";
  pairs[414] = "414 URI Too Long";
  pairs[415] = "415 Unsupported Media Type";
  pairs[500] = "500 Internal Server Error";
  pairs[505] = "505 HTTP Version Not Supported";
  return pairs;
};
// TODO: FOR LATER > encode html with picture nd find right type!