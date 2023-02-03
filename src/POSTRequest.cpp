
#include "POSTRequest.hpp"

//BASIC CLASS SETUP
POSTRequest::POSTRequest(request_handler &request) : _request(request) {}

POSTRequest::~POSTRequest() {}


int POSTRequest::create_response() {

	_body = _request.buf.substr(_request.head_length, _request.buf.length());
	create_env();
	if (set_interpreter_path() == EXIT_FAILURE)
		return	INTERNAL_SERVER_ERROR;
	if (set_script_path() == EXIT_FAILURE)
		return	INTERNAL_SERVER_ERROR;
	
}

void	POSTRequest::create_env() {
	_request.env.push_back("REQUEST_METHOD=POST");
	_request.env.push_back("QUERY=" + _body);
	_request.env.push_back("SCRIPT_NAME=" + _request.file_path);
	_request.env.push_back("CONTENT_LENGTH=" + toString<size_t>(_request.body_length));
	_request.env.push_back("REMOTE_ADDR=" + remote_addr());
	_request.env.push_back("HTTP_USER_AGENT=" + http_user_agent());
	_request.env.push_back("RESPONSE_HEADER" + RESPONSE_HEADER);
}

std::string POSTRequest::http_user_agent() {
	size_t	begin;
	size_t	end;

	begin = _request.buf.find("User-Agent:");
	if (begin == std::string::npos)
		return EMPTY_STRING;
	begin += std::strlen("User-Agent:");
	while (_request.buf[begin] == SPACE)
		begin++;
	end = _request.buf.find(NEWLINE, begin);
	if (end == std::string::npos)
		return EMPTY_STRING;
	return	_request.buf.substr(begin, end - begin);
}

std::string POSTRequest::remote_addr() {
	struct sockaddr_in	socket_in_address = _request.socket.get_serv_addr();
	struct sockaddr *socket_address = reinterpret_cast<struct sockaddr *>(&socket_in_address);
	socklen_t addrlen = sizeof(*socket_address);
	int remote_addr = getpeername(_request.socket.get_pollfd().fd, socket_address, &addrlen);
	if (remote_addr == -1)
		return "";

	char str[INET_ADDRSTRLEN];
	return inet_ntop(AF_INET, &socket_in_address, str, INET_ADDRSTRLEN);
}

int	POSTRequest::set_interpreter_path() {
	std::size_t dot_pos;
	std::string extensions[3] = {".py", ".php", ".perl"}
	std::string interpreters[3] = {"/usr/bin/python", "/usr/bin/php","/usr/bin/perl"}

	dot_pos = _request.filepath.find_last_of('.');
	if (_request.filepath.empty() || dot_pos == std::string::npos)
		return EXIT_FAILURE;
	std::string extension = _request.file_path.substr(dot_pos, _request.file_path.size() - dot_pos);
	for (size_t i = 0; i < 3; i++) {
		if (extension = extensions[i]) {
			_request.interpreter = interpreters[i];
			return EXIT_SUCCESS;
		}
	}	
	return EXIT_FAILURE;	
}

int	POSTRequest::set_script_path() {
	_request.file_path
	return EXIT_FAILURE;
}

// //MEMBER FUNCTIONS
// int POSTRequest::create_response(const request_handler &request) {
// 	std::ofstream file;

// 	_body = request.buf.substr(request.head_length, request.buf.length());
// 	if (_body.find("Content-Disposition: form-data") != std::string::npos)
// 		return (upload_file());
// 	if (_body == "delete_all") {
// 		file.open(DATABASE_PATH, std::ios::out | std::ios::trunc);
// 		return EXIT_SUCCESS;
// 	}
// 		//remove file & return;
// 	file.open(DATABASE_PATH, std::ios::out | std::ios::app);
// 	if (!file.is_open() || file.fail())
// 		return INTERNAL_SERVER_ERROR;
// 	file << _body << '\n';
// 	return EXIT_SUCCESS;
// }

// int POSTRequest::upload_file() {
// 	std::ofstream file;
// 	size_t begin = _body.find("filename=\"") + std::strlen("filename=\"");
// 	if (begin == std::string::npos)
// 		return BAD_REQUEST;
// 	begin += std::strlen("filename=\"");
// 	size_t end = _body.find("\"", begin);
// 	if (end == std::string::npos)
// 		return BAD_REQUEST;
// 	_filepath = DEFAULT_UPLOAD_DIR + _body.substr(begin, end - begin);
// 	file.open(_filepath, std::ios::out | std::ios::trunc);
// 	if (!file.is_open() || file.fail())
// 		return INTERNAL_SERVER_ERROR;
// 	begin = _body.find("\r\n\r\n", begin) + std::strlen("\r\n\r\n");
// 	end = _body.find("\r\n", begin);
// 	if (begin == std::string::npos || end == std::string::npos)
// 		return BAD_REQUEST;
// 	file << _body.substr(begin, end - begin);
// 	return  EXIT_SUCCESS;
// }
