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
	pollfd			_pfd;
	const Socket	&_socket;
	std::string 	&_request;
	std::string		_response;


	std::string		get_requested_path();

	//ERROR MANAGEMENT
	int				system_call_error(int error, const Socket &socket);
	std::string		generate_error_code_response(int error);

public:
	ResponseGenerator(pollfd &pfd, const Socket &socket, std::string &request);
	ResponseGenerator(std::vector<Socket> &sockets, const ResponseGenerator &src);
	ResponseGenerator &operator=(const ResponseGenerator &src);
	~ResponseGenerator();

	std::string 	generate_response();
};