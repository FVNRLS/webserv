/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:36:37 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/17 12:59:55 by doreshev         ###   ########.fr       */
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

/*
 * Core function.
 * Runs the server indefinitely by continuously polling the descriptors, checking for incoming data from the client socket,
 * accepting requests from the other sockets, resolving these requests and deleting invalid file descriptors.
 * */
int Server::run() {
	while (true) {
		if (poll(_pfds.data(), _pfds.size(), TIMEOUT) < 0)
			return (terminate_with_error(system_call_error(POLL_ERROR, _sockets.front())));
		if (check_cli() == EXIT_FAILURE)
			return EXIT_FAILURE;
		if (accept_requests() == EXIT_FAILURE)
			return EXIT_FAILURE;
		if (resolve_requests() == EXIT_FAILURE)
			return EXIT_FAILURE;
		delete_invalid_fds();
		// std::cout << "NUM FD'S:		" << _pfds.size() << std::endl;
	}
}

/*
 * Checks whether there is incoming data from the Command Line Interface socket.
 * If there is, it processes the data.
 * */
int	Server::check_cli() {
	if (_pfds[0].revents & POLLIN) {
		if (process_cli_input() == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*
 * Loops through all sockets in the list of Socket objects, and checks for incoming data using the pollfd-revents-flag.
 * If there is incoming data, it accepts the request and sets up the new polling descriptor for the new connection.
 * Then it sets the pollfd (connection) to non-blocking, and adds the new polling descriptor to the vector of pollfd descriptors.
 * It also sets up a new request handler for the new socket and adds the new connection to the _requests map.
 * */
int Server::accept_requests() {
	socklen_t 			client_len;
	struct sockaddr 	client_addr;
	pollfd				client_pollfd;

	for (size_t i = 1; i < _sockets.size(); i++) {
		if (_pfds[i].revents & POLLIN) {
			client_len = sizeof(client_addr);
			client_pollfd.fd = accept(_pfds[i].fd, &client_addr, &client_len);
			if (client_pollfd.fd < 0)
				return (system_call_error(ACCEPT_ERROR, _sockets[i]));
			if (fcntl(client_pollfd.fd, F_SETFL, O_NONBLOCK) < 0)
				return EXIT_FAILURE;
			client_pollfd.events = POLLIN;
			_pfds.push_back(client_pollfd);
			_requests[client_pollfd.fd] = request_handler();
            _requests[client_pollfd.fd].socket = _sockets[i];
		}
	}
	return EXIT_SUCCESS;
}

/*
 * Loops through all sockets in the list of Socket objects, and checks for incoming data.
 * If there is incoming data, it accepts the request, sets the new socket to non-blocking,
 * sets up the new polling descriptor for the new socket, and adds the new polling descriptor to the list of pollfd descriptors.
 * It also sets up a new request handler for the new socket.
 * The POLLIN/POLLOUT flag indicates reading of the request or sending of the response to the client.
 * */
int Server::resolve_requests() {
	for (size_t i = _sockets.size(); i < _pfds.size(); i++) {
		if (check_connection(_pfds[i]) == EXIT_FAILURE)
			continue;
		if (_pfds[i].revents & POLLIN)
			return (handle_pollin(_pfds[i]));
		else if (_pfds[i].revents & POLLOUT)
			return (handle_pollout(_pfds[i]));
	}
	return EXIT_SUCCESS;
}

/*
 * Handles the input for the socket by accumulating data from the socket and parsing the accumulated data to determine
 * whether the request header has been fully received, the request body has been fully received,
 * and whether the request is chunked or not.
 * */
int Server::handle_pollin(pollfd &pfd) {
    request_handler* request = &_requests.find(pfd.fd)->second;

	if (accumulate(*request, pfd.fd) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (request->head_received && !request->status) {
        if (!request->chunked) {
            request->status = handle_request_header(*request);
            if (!request->chunked && request->buf.size() >= request->head_length + request->body_length)
                request->body_received = true;
        }
        if (request->chunked && !request->body_received) {
            Chunks    chunks(*request);
            chunks.handle();
        }
    }
	if (request->status || request->method == "GET" || request->body_received)
		pfd.events = POLLOUT;
	return EXIT_SUCCESS;
}

/*
 * Handles the output for the socket by sending the response to the socket and setting the event for the polling descriptor to POLLIN.
 * */
int Server::handle_pollout(pollfd &pfd) {
	request_handler* request = &_requests.find(pfd.fd)->second;

    if (request->response.empty()) {
        ResponseGenerator resp_gen(*request);
        request->response = resp_gen.generate_response(_cookies);
    }
	if (send_response(pfd.fd, request) || request->response_sent) {
        if (request->status != CONTINUE)
            request->clear();
        request->response.clear();
        request->status = EXIT_SUCCESS;
        pfd.events = POLLIN;
    }
	return EXIT_SUCCESS;
}

/*
 * Sends the response to the client, and sets the response_sent flag to true when the entire response has been sent.
 * */
int Server::send_response(int fd, request_handler *request) {

    ssize_t bytes = send(fd, request->response.data(), request->response.size(), 0);
    if (bytes == -1)
        return EXIT_FAILURE;
    request->bytes_sent += bytes;
    if (bytes == 0 || request->bytes_sent >= request->response.size())
        request->response_sent = true;
    else
        request->response = request->response.substr(bytes);
    return EXIT_SUCCESS;
}

/*
 * Checks the validity of the connection for the socket, and closes the socket and removes the polling descriptor
 * for the socket if the connection is invalid.
 * */
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

/*
 * Accumulates data from the socket and appends it to the request buffer.
 * */
int	Server::accumulate(request_handler &request, int request_fd) {
	char 										buffer[2000];
	long 										bytes;

	bytes = recv(request_fd, buffer, sizeof(buffer), MSG_DONTWAIT);
	if (bytes < 0)
		return (system_call_error(RECV_ERROR));
	request.buf += std::string(buffer, bytes);
    if (!request.chunked)
	    set_request_end_flags(request);
	return EXIT_SUCCESS;
}

/*
 * Sets flags for the end of an HTTP request.
 * It looks for the position of ehd of the request in the request buffer, and if found, updates the head_length attribute
 * of the request object and sets the head_received flag to true.
 * If the end of the request is not found, the function simply returns.
 * It also checks if the length of the request body exceeds a specified limit and sets the status of the request to BAD_REQUEST if it does.
 * */
void	Server::set_request_end_flags(request_handler &request) {
    request.head_length = request.buf.find(END_OF_REQUEST);
	if (request.head_length != std::string::npos) {
		request.head_length += std::strlen(END_OF_REQUEST.c_str());
		request.head_received = true;
	}
    if (!request.head_received)
        return;
    long long	max_client_body_size = request.socket.get_config().get_max_client_body_size();
    if ((static_cast<long long>(request.buf.length() - request.head_length) > max_client_body_size))
    {
        request.status = BAD_REQUEST;
        return;
    }
}

/*
 * Handles the header of an HTTP request.
 * Creates a RequestParser object to parse the request.
 * If the file_path attribute of the request object is not empty, it returns the status of the request.
 * Otherwise, parses and checks the status of the request.
 * If the request object has cookies, it checks if the cookies are valid and updates them accordingly.
 * */
int Server::handle_request_header(request_handler &request) {
    RequestParser	request_parser(request);

    if (!request.file_path.empty())
        return request.status;
    request_parser.parse();
	if (request.status)
		return request.status;
	if (!_cookies.exists(request.cookies))
		request.cookies = false;
	request.cookies = check_logout(request.cookies, request.query);
    return request.status;
}

/*
 * Checks if a user has initiated a logout action.
 * If the key is not 0 and the query contains "action=logout", the function deletes the user's session and returns false.
 * Otherwise, it returns the key value.
 * */
int	Server::check_logout(const int &key, const std::string &query) {
	if (key && query.find("action=logout") != std::string::npos) {
		_cookies.delete_session(key);
		return false;
	}
	return key;
}

/*
 * Deletes invalid file descriptors from a vector of pollfds if their fd value is set to -1.
 * */
void	Server::delete_invalid_fds() {
	std::vector<pollfd>::iterator it = _pfds.begin() + _sockets.size();

	while (it != _pfds.end()) {
		if (it->fd == -1)
			it = _pfds.erase(it);
		else
			it++;
	}
}

//TERMINAL FUNCTIONS

/*
 * Is responsible for processing the input received from the command-line interface (CLI).
 *
 * Checks for any input from the command-line interface.
 * Based on the return value of check_input(), the function performs one of the following actions:
 * 		If the input is empty, the function returns EXIT_SUCCESS.
 * 		If the input is invalid, the function returns an error using the terminate_with_error() function with a system_call_error object.
 * 		If the input is CLI_EXIT, the function calls the exit_server() function.
 * 		If the input is CLI_LS, the function calls the show_connections() function.
 * 		If the input is any other value, the function calls the show_manual() function.
 * Returns EXIT_SUCCESS by default.
 * */
int	Server::process_cli_input() {
	int cli_input;

	cli_input = _cli.check_input();
	switch (cli_input) {
		case CLI_EMPTY:
			return EXIT_SUCCESS;
		case CLI_FAIL:
			return terminate_with_error(system_call_error(CLI_ERROR, _sockets.front()));
		case CLI_LS:
			show_connections();
			break;
		case CLI_EXIT:
			exit_server();
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
	return EXIT_FAILURE;
}

void 	Server::show_connections() {
	print_configurations();
}

void	Server::print_configurations() {
	for (size_t i = 1; i < _sockets.size(); i++)
		std::cout << &_sockets[i].get_config() << std::endl;
}

void 	Server::show_manual() {
	std::cout << "Let me help you!\nSIGINT was silenced in order to close webserver correctly!\n"
    << "Commands:\n"
    << "ls     prints server configuration\n"
    << "exit   exits webserver properly!\n";
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
	return EXIT_FAILURE;
}

