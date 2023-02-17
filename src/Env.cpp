#include "Env.hpp"

Env::Env(request_handler& request) : _request(request) {
}
Env::~Env() {}


void	Env::create() {
	_request.env.push_back("REQUEST_METHOD=" + _request.method);
	_request.env.push_back("QUERY_STRING=" + _request.query);
	_request.env.push_back("SCRIPT_NAME=" + _request.file_path);
    _request.env.push_back("REQUEST_URI=" + _request.file_path);
    _request.env.push_back("PATH_INFO=" + _request.file_path);
    _request.env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_request.env.push_back("CONTENT_LENGTH=" + toString<size_t>(_request.body_length));
	_request.env.push_back("REMOTE_ADDR=" + remote_addr());
	_request.env.push_back("HTTP_USER_AGENT=" + _request.user_agent);
	_request.env.push_back("RESPONSE_HEADER=" + RESPONSE_HEADER);
	_request.env.push_back("CONTENT_TYPE=" + _request.content_type);
	if (_request.cookies)
		_request.env.push_back("HTTP_COOKIE=key=" + toString<int>(_request.cookies));
	else
		_request.env.push_back("HTTP_COOKIE=");
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
