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

#include "requestHandler.hpp"

class ResponseGenerator {

private:
	request_handler&			_request;
	std::string 				_response;
	std::string 				_request_body;

	//ERROR MANAGEMENT
	std::string 		create_error_code_response(int error);

public:
	ResponseGenerator(request_handler &request);
	ResponseGenerator(ResponseGenerator &src);
	~ResponseGenerator();

	std::string 		generate_response();
};