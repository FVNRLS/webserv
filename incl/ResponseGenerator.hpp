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

class ResponseGenerator {

private:
	const Socket				&_socket;
	std::string 				&_request;

	std::string					_response;
	std::string 				_method;

	//MEMBER FUNCTIONS
	int 						check_max_client_body_size();
	std::vector<std::string>	tokenize_first_line(std::vector<std::string> &tokens);
	int 						select_method(const std::vector<std::string> &tokens);
	std::vector<std::string> 	get_allowed_methods(std::string &file_path);

	//ERROR MANAGEMENT
	std::string 		create_error_code_response(int error);

public:
	ResponseGenerator(const Socket &socket, std::string &request);
	ResponseGenerator(const ResponseGenerator &src);
	ResponseGenerator &operator=(const ResponseGenerator &src);
	~ResponseGenerator();

	std::string 		generate_response();
};