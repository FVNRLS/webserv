#include "Env.hpp"

Env::Env(request_handler& request) : _request(request) {
}
Env::~Env() {}


void	Env::create() {
	_request.env.push_back("REQUEST_METHOD=" + _request.method);
	_request.env.push_back("QUERY_STRING=" + _request.query);
	_request.env.push_back("SCRIPT_NAME=" + _request.file_path);
	_request.env.push_back("CONTENT_LENGTH=" + toString<size_t>(_request.body_length));
	_request.env.push_back("REMOTE_ADDR=" + remote_addr());
	_request.env.push_back("HTTP_USER_AGENT=" + get_header_value("User-Agent:"));
	_request.env.push_back("RESPONSE_HEADER=" + RESPONSE_HEADER);
	_request.env.push_back("CONTENT_TYPE=" + get_header_value("Content-Type:"));
	if (_request.cookies == true)
		_request.env.push_back("HTTP_COOKIE=" + toString<int>(_request.cookies));
	else
		_request.env.push_back("HTTP_COOKIE=");
}

std::string Env::get_header_value(std::string key) {
	size_t begin;
	size_t end;

	begin = _request.buf.find(key);
	if (begin == std::string::npos)
		return EMPTY_STRING;
	begin += key.length();
	while (_request.buf[begin] == SPACE)
		begin++;
	end = _request.buf.find(NEWLINE, begin);
	if (end == std::string::npos)
		return EMPTY_STRING;
	return _request.buf.substr(begin, end - begin);
}

std::string Env::remote_addr() {
	struct sockaddr_in	socket_in_address = _request.socket.get_serv_addr();
	struct sockaddr *socket_address = reinterpret_cast<struct sockaddr *>(&socket_in_address);
	socklen_t address_len = sizeof(*socket_address);
	int remote_addr = getpeername(_request.socket.get_pollfd().fd, socket_address, &address_len);

	if (remote_addr == -1)
		return "";
	char str[INET_ADDRSTRLEN];
	return inet_ntop(AF_INET, &socket_in_address, str, INET_ADDRSTRLEN);
}
