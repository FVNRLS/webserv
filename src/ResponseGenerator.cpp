/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseGenerator.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoomen <hoomen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:20:38 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/07 12:14:00 by hoomen           ###   ########.fr       */
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
  return "HTTP/1.1 " + status_code_str +
         "\nContent-Type: text/html\nContent-Length: " + body_length +
         END_OF_REQUEST;
}

// TODO: FOR LATER > encode html with picture nd find right type!