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

#include "GETRequest.hpp"
#include "POSTRequest.hpp"

struct request_handler { //todo: delete item from map
	Socket							socket;
	std::string 					buf;
	std::string 					method;
	std::string 					file_path;
	bool 							body_received;
	bool 							head_received;
	int 							status;
	size_t 							body_length;
	size_t 							head_length;
};

class ResponseGenerator {

private:
	request_handler&			_request;
	std::string 				_response;
	std::string 				_request_body;

	std::string 				get_request_body();


	//ERROR MANAGEMENT
	std::string 		create_error_code_response(int error);

public:
	ResponseGenerator(request_handler &request);
	ResponseGenerator(ResponseGenerator &src);
	~ResponseGenerator();

	std::string 		generate_response();
};