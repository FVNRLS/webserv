#include "RequestParser.hpp"

RequestParser::RequestParser(request_handler& request) : _request(request) {}

RequestParser::RequestParser(RequestParser const& src) {
  *this = src;
}

RequestParser& RequestParser::operator=(RequestParser const& rhs) {
  if (this == &rhs) return *this;
  _request = rhs._request;
  return *this;
}

RequestParser::~RequestParser() {}

std::vector<std::string> RequestParser::tokenize_request_line() {
  size_t new_line_position = _request.raw.find(NEWLINE);
  if (new_line_position != std::string::npos)
    return std::vector<std::string>();
  std::string first_line = _request.raw.substr(0, new_line_position);
  return split(first_line, SPACE);
}

void RequestParser::parse_request_line() {
  std::vector<std::string> tokens_request_line = tokenize_request_line();
  if (tokens_request_line.size() < 3) {
    _request.status = BAD_REQUEST;
    return;
  }
  _request.method = tokens_request_line[0];
  split_url_and_query(tokens_request_line[1]);
}

void RequestParser::split_url_and_query(std::string& url_and_query_token) {
  size_t question_mark_position = url_and_query_token.find('?');
  if (question_mark_position != std::string::npos)
    _request.query = url_and_query_token.substr(question_mark_position + 1);
  _request.url = url_and_query_token.substr(0, question_mark_position);
}

void RequestParser::parse() {
  parse_request_line();
  parse_body_length();
  parse_cookies();
  get_location_id();
  get_allowed_methods();
  check_method();
}

void RequestParser::get_location_id() {
  std::vector<std::string> locations = split(_request.url, '/');
	switch (locations.size()) {
		case 0:
      break;
		case 1:
      if (locations[1].find(".html") != std::string::npos)
        break;
		default:
			_location_id = _request.socket.get_config().get_index();
  }
}

location& RequestParser::get_location() {
  std::vector<std::string> locations = split(_request.url, '/');
  switch (locations.size()) {
		case 0:
      return empty_location();
		case 1:
      if (locations[0].find(".html") != std::string::npos);
      return empty_location();
		default:
			return get_location_config(locations[0]);
}


// void RequestParser::get_allowed_methods() {
//   if (_location_id == "")
//     _allowed_methods = _request.socket.get_config().get_methods();
//   else {
//     location loc = get_location();
//     _allowed_methods = loc.methods;
//     if (_request.status) return ;
//     check_method();
//   }
// }

// void RequestParser::check_method() {
//   for(std::vector<std::string>::iterator it = _allowed_methods.begin(); it != _allowed_methods.end(); ++it) {
//     if (*it == _request.method)
//       return ;
//   }
//   _request.status = METHOD_NOT_ALLOWED;
// }

// location RequestParser::get_location() {
//   std::vector<location> locations = _request.socket.get_config().get_locations();
//   for (std::vector<location>::iterator it = locations.begin(); it != locations.end(); ++it) {
//     if (it->index == _location_id)
//       return *it;
//   }
//   _request.status = PAGE_NOT_FOUND;
//   location empty;
//   memset(&empty, 0, sizeof(empty));
//   return empty;
//   }

void RequestParser::parse_body_length() {
  std::string body_length = get_header_value("Content-Length:");
  if (body_length.c_str() == NULL) return ;
  _request.body_length = strtoll(get_header_value("Content-Length:").c_str(), NULL, 10);
}

void RequestParser::parse_cookies() {
  std::string cookies = get_header_value("Cookie:");
  _request.cookies = split(cookies, ';');
  for (std::vector<std::string>::iterator it = _request.cookies.begin(); it != _request.cookies.end(); ++it) {
    size_t first_non_whitespace = it->find_first_not_of(" \t");
    *it = it->substr(first_non_whitespace);
  }
}

std::string RequestParser::get_header_value(std::string const& key) {
	size_t begin;
	size_t end;

	begin = _request.raw.find(key);
	if (begin == std::string::npos)
		return EMPTY_STRING;
	begin += key.length();
	while (_request.raw[begin] == SPACE)
		begin++;
	end = _request.raw.find(NEWLINE, begin);
	if (end == std::string::npos)
		return EMPTY_STRING;
	return _request.raw.substr(begin, end - begin);
}