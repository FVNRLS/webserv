/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:36:37 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/06 19:00:55 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//BASIC CLASS SETUP
Server::Server(std::vector<Socket> &sockets) : _sockets(sockets) {
	if (_cli.start() == EXIT_FAILURE)
		exit_server();
	_pfds.push_back(_cli.get_pollfd());
	for (size_t i = 1; i < sockets.size(); i++) {
		_pfds.push_back(sockets[i].get_pollfd());
		_pfds[i].events = POLLIN;
	}
}

Server::~Server() {}

//MEMBER FUNCTIONS
int Server::run() {
	while (true) {
		if (poll(_pfds.data(), _pfds.size(), TIMEOUT) < 0)
			return (terminate_with_error(system_call_error(POLL_ERROR, _sockets.front())));
		if (check_cli() == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (accept_requests() == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (resolve_requests() == EXIT_FAILURE)
			return (EXIT_FAILURE);
		delete_invalid_fds();
//		std::cout << "NUM FD'S:		" << _pfds.size() << std::endl;
	}
}

int	Server::check_cli() {
	if (_pfds[0].revents & POLLIN) {
		if (process_cli_input() == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	return (EXIT_SUCCESS);
}

int Server::accept_requests() {
	socklen_t 			client_len;
	struct sockaddr 	client_addr = {};
	pollfd				client_pollfd = {};
	request_handler		request = {};

	for (size_t i = 1; i < _sockets.size(); i++) {
		if (_pfds[i].revents & POLLIN) {
			client_len = sizeof(client_addr);
			client_pollfd.fd = accept(_pfds[i].fd, &client_addr, &client_len);
			if (client_pollfd.fd < 0)
				return (system_call_error(ACCEPT_ERROR, _sockets[i]));
			if (fcntl(client_pollfd.fd, F_SETFL, O_NONBLOCK) < 0)
				return (EXIT_FAILURE);
			client_pollfd.events = POLLIN;
			_pfds.push_back(client_pollfd);
			request.socket = _sockets[i];
			_requests[client_pollfd.fd] = request;
		}
	}
	return (EXIT_SUCCESS);
}

int Server::resolve_requests() {
	for (size_t i = _sockets.size(); i < _pfds.size(); i++) {
		if (check_connection(_pfds[i]) == EXIT_FAILURE)
			continue;
		if (_pfds[i].revents & POLLIN)
			return (handle_pollin(_pfds[i]));
		else if (_pfds[i].revents & POLLOUT)
			return (handle_pollout(_pfds[i]));
	}
	return (EXIT_SUCCESS);
}

int Server::handle_pollin(pollfd &pfd) {
	request_handler* request = &_requests.find(pfd.fd)->second;

	if (accumulate(*request, pfd.fd) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (request->head_received) {
		request->status = handle_request_header(*request);
	}
	if (request->status || request->method == "GET" || request->body_received)
		pfd.events = POLLOUT;
	return EXIT_SUCCESS;
}

int Server::handle_pollout(pollfd &pfd) {
	request_handler*	request;
	std::string 		response;

	request = &_requests.find(pfd.fd)->second;
	std::cout << request->buf << std::endl;
	ResponseGenerator resp_gen(*request);
	response = resp_gen.generate_response();
	if (!response.empty()) {
		if (send(pfd.fd, response.c_str(), response.length(), 0) == EXIT_FAILURE) //todo: check if chunked!
			return EXIT_FAILURE;
        memset(request, 0, sizeof(request_handler));
	}
	close(pfd.fd);
	return EXIT_SUCCESS;
}

int Server::check_connection(pollfd &pfd) {
	if (pfd.revents & POLLERR || pfd.revents & POLLHUP || pfd.revents & POLLNVAL) {
		std::map<int, request_handler>::iterator it = _requests.find(pfd.fd);

		if (it != _requests.end())
			_requests.erase(it);
		shutdown(pfd.fd, SHUT_RDWR);
		close(pfd.fd);
		pfd.fd = -1;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int	Server::accumulate(request_handler &request, int request_fd) {
	char 										buffer[2000];
	long 										bytes;

	bytes = recv(request_fd, buffer, sizeof(buffer), MSG_DONTWAIT);
	if (bytes < 0)
		return (system_call_error(RECV_ERROR));
	request.buf += std::string(buffer, bytes);
	set_request_end_flags(request);
	return (EXIT_SUCCESS);
}

void	Server::set_request_end_flags(request_handler &request) {
	long long	max_client_body_size = request.socket.get_config().get_max_client_body_size();

	if ((static_cast<long long>(request.buf.length()) > max_client_body_size)) {
		request.status = BAD_REQUEST;
		return;
	}
	request.head_length = request.buf.find(END_OF_REQUEST);
	if (request.head_length != std::string::npos) {
		request.head_length += std::strlen(END_OF_REQUEST.c_str());
		request.head_received = true;
	}
}

int Server::handle_request_header(request_handler &request) {
	std::vector<std::string> tokens = tokenize_first_line(request.buf);
	if (tokens.size() < 3)
		return BAD_REQUEST;
	request.method = tokens[0];
	split_query(request, tokens[1]);
	return check_requested_url(request);
}

void	Server::split_query(request_handler& request, std::string& url) {
	size_t position = url.find('?');
	request.file_path = url.substr(0, position);
	if (position != std::string::npos) {
		request.query = url.substr(position + 1);
		request.body_length = request.query.length();
	}
}

int	Server::check_requested_url(request_handler &request) {
	std::vector<std::string> locations = split(request.file_path, '/');

	switch (locations.size()) {
		case 0:
			return check_main_configs(request, locations);
		case 1:
			if(locations[0].find(".html") != std::string::npos)
				return check_main_configs(request, locations);
		default:
			return check_location_config(request, locations);
	}
	return request.status;
}

int	Server::check_main_configs(request_handler &request, std::vector<std::string> &locations) {
	if (check_method(request.socket.get_config().get_methods(), request.method) == EXIT_FAILURE)
		return METHOD_NOT_ALLOWED;
	request.file_path = get_server_filepath(request, locations);
	request.body_length = get_body_length(request);
	if(request.buf.size() >= request.head_length + request.body_length)
		request.body_received = true;
	return EXIT_SUCCESS;
}

int Server::check_location_config(request_handler &request, std::vector<std::string> &locations) {
	location loc = get_location_config(request, locations);

	if (request.status)
		return request.status;
	if (check_method(loc.methods, request.method) == EXIT_FAILURE)
		return METHOD_NOT_ALLOWED;
	request.file_path = get_location_filepath(loc, locations);
	request.body_length = get_body_length(request);
	if(request.buf.size() >= request.head_length + request.body_length)
		request.body_received = true;
	return check_allowed_scripts(loc, request);
}

int Server::check_allowed_scripts(location& loc, request_handler& request) {
	if (request.file_path.find(".html") != std::string::npos)
        return EXIT_SUCCESS;

    size_t index = request.file_path.find_last_of('.');
	std::string extension = request.file_path.substr(index + 1);
	for (size_t	i = 0; i < loc.scripts.size(); i++) {
		if (extension == loc.scripts[i].first) {
			request.interpreter = loc.scripts[i].second;
			return EXIT_SUCCESS;
		}
	}
	return METHOD_NOT_ALLOWED;
}

location	Server::get_location_config(request_handler &request, std::vector<std::string> &locations) {
	for (size_t	i = 0; i < request.socket.get_config().get_locations().size(); i++) {
		if (request.socket.get_config().get_locations()[i].prefix.compare(1, locations[0].size(), locations[0]) == 0) {
			return request.socket.get_config().get_locations()[i];
		}
	}
	request.status = PAGE_NOT_FOUND;
	return location();
}

int	Server::check_method(const std::vector<std::string> &methods, std::string &method) {
	for (size_t	i = 0; i < methods.size(); i++) {
		if	(methods[i] == method)
			return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

std::string	Server::get_server_filepath(request_handler &request, std::vector<std::string> &locations) {
	if (locations.empty())
		return (request.socket.get_config().get_root() + request.socket.get_config().get_index());
	return (request.socket.get_config().get_root() + locations[0]);
}

std::string	Server::get_location_filepath(location &loc, std::vector<std::string> &locations) {
	if (locations.size() == 1)
		return (loc.root + loc.index);
	if (locations.back().find("html") != std::string::npos)
		return (loc.root + locations.back());
	if (loc.cgi_path == EMPTY_STRING)
		return EMPTY_STRING;
	return (loc.root + loc.cgi_path + locations.back());
}


size_t Server::get_body_length(request_handler &request) {
	if (request.method == "GET")
		return 0;

	size_t pos = request.buf.find("Content-Length: ");
	size_t length = static_cast<size_t> (std::atoll(request.buf.data() + pos + std::strlen("Content-Length: ")));
	return length;
}

std::vector<std::string> Server::tokenize_first_line(std::string &request) {
	std::string					first_request_line;
	std::vector<std::string>	tokens;

	size_t	new_line_position = request.find(NEWLINE);
	if (new_line_position != std::string::npos) {
		first_request_line =  request.substr(0, new_line_position);
		tokens = split(first_request_line, SPACE);
	}
	return tokens;
}

void	Server::delete_invalid_fds() {
	std::vector<pollfd>::iterator it = _pfds.begin() + _sockets.size();

	while (it != _pfds.end()) {
		if (it->fd == -1)
			it = _pfds.erase(it);
		else
			it++;
	}
}





















//TODO:implement later
//---------------------------------------------------------------------------------------------------------------------

//VIRTUAL HOSTING
int Server::process_request(const Socket &socket, pollfd &poll_fd) {
	(void )poll_fd;
	(void )socket;
	//	bool	socket_is_unique;

//	socket_is_unique = socket.get_is_unique();
//	if (socket_is_unique) {
//		if (resolve_requests(socket, poll_fd) == EXIT_FAILURE)
//			return (EXIT_FAILURE);
//	}
//	else if (serve_on_virtual_host(socket, poll_fd) < 0)
//		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int Server::serve_on_virtual_host(const Socket &socket, pollfd &poll_fd) {
	char				buf[4096];
	int 				client_socket;
	std::string 		domain;
	std::stringstream	ss;
	std::string 		request;
	std::string 		name;
	std::string 		alias;

	name = socket.get_config().get_name();
	alias = socket.get_config().get_alias();

	client_socket = accept(poll_fd.fd, NULL, NULL);
	recv(client_socket, buf, 4096, 0);
	ss << recv(client_socket, buf, 4096, 0);
	request = ss.str();

	//todo: probalby unuseful! but that's how we can get the domain name!
//	struct hostent *host = gethostbyname(alias.c_str());
//	if (host == NULL)
//		std::cout << "Error: Invalid domain name" << std::endl;
//	std::cout << host << std::endl;


	domain = extract_domain(request);
	std::cout << domain << std::endl;
	if (domain == name || domain == alias) {
		char response[] = "Hello from server!";
		send(client_socket, response, sizeof(response), 0);
	}
	close(client_socket);
	return (EXIT_SUCCESS);
}

std::string Server::extract_domain(std::string &request) {
	std::string domain;
	std::stringstream ss(request);
	std::string line;
	while (std::getline(ss, line)) {
		if (line.find("Host:") == 0) {
			domain = line.substr(6);
			break;
		}
	}
	return domain;
}


//TERMINAL FUNCTIONS
int	Server::process_cli_input() {
	switch (_cli.check_input()) {
		case CLI_EMPTY:
			return EXIT_SUCCESS;
		case CLI_FAIL:
			return terminate_with_error(system_call_error(CLI_ERROR, _sockets.front()));
		case CLI_EXIT:
			exit_server();
		case CLI_LS:
			show_connections();
			break;
		default:
			show_manual();
	}
	return EXIT_SUCCESS;
}

void	Server::exit_server() {
	for (size_t i = 0; i < _sockets.size(); i++)
		close(_pfds[i].fd);
	exit(EXIT_SUCCESS);
}

int		Server::terminate_with_error(int) {
	for (size_t i = 0; i < _sockets.size(); i++)
		close(_pfds[i].fd);
	return (EXIT_FAILURE);
}

void 	Server::show_connections() {
	print_configurations();
}

void	Server::print_configurations() {
	for (size_t i = 1; i < _sockets.size(); i++)
		std::cout << &_sockets[i].get_config() << std::endl;
}

void 	Server::show_manual() {
	std::cout << "CALLED HELP\n";
}


//ERROR MANAGEMENT
int Server::system_call_error(int error, const Socket &socket) {
	switch(error) {
		case POLL_ERROR:
			std::cerr << "Error: Failed polling sockets" << std::endl;
			break;
		case ACCEPT_ERROR:
			std::cerr << "Error: Failed to accept connection on port " << socket.get_port() << std::endl;
			break;
		case CLI_ERROR:
			std::cerr << "Error: Command line interface failed" << std::endl;
			break;
		case RECV_ERROR:
			std::cerr << "Error: Failed to receive request" << std::endl;
			break;
		case SEND_ERROR:
			std::cerr << "Error: Failed To Send a Response On Port " << socket.get_port() << std::endl;
			break;
		case ACCESS_DENIED:
			std::cerr << "Error: Access Denied!" << std::endl;
			break;
		default:
			std::cerr << "Server: unknown error" << std::endl;
	}
	return (EXIT_FAILURE);
}

