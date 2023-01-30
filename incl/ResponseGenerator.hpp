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

class ResponseGenerator {

private:
	pollfd						_pfd;
	const Socket				&_socket;
	std::string 				&_request;

	std::vector<std::string>	_valid_locs;
	std::string					_response;
	std::string 				_method;

	//MEMBER FUNCTIONS
	int 						check_max_client_body_size();
	std::vector<std::string>	tokenize_first_line(std::vector<std::string> &tokens);
	int 						select_method(const std::vector<std::string> &tokens);
	std::string 		        get_full_location_path(std::string &file_path);


	//ERROR MANAGEMENT
	std::string 		create_error_code_response(int error);

public:
	ResponseGenerator(const Socket &socket, std::string &request);
	ResponseGenerator(const ResponseGenerator &src);
	ResponseGenerator &operator=(const ResponseGenerator &src);
	~ResponseGenerator();

	std::string 		generate_response();
};