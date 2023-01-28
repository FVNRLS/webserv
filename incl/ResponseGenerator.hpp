/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:22:00 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/26 12:22:00 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"

class ResponseGenerator {

private:
	pollfd				_pfd;
	const Socket		&_socket;
	std::string 		&_request;

	std::string			_response;
	std::string 		_requested_path;
	std::string 		_body;
	std::stringstream 	_body_len;


	//MEMBER FUNCTIONS
	int 				check_max_client_body_size();
	std::string			get_requested_path();
	int 				get_response_body(std::string &file_path, std::ifstream &file);
	int 				open_file(std::string &file_path, std::ifstream &file);

		//ERROR MANAGEMENT
	int					system_call_error(int error, const Socket &socket);
	std::string			generate_error_code_response(int error);

public:
	ResponseGenerator(pollfd &pfd, const Socket &socket, std::string &request);
	ResponseGenerator(std::vector<Socket> &sockets, const ResponseGenerator &src);
	ResponseGenerator &operator=(const ResponseGenerator &src);
	~ResponseGenerator();

	std::string 		generate_response();
};