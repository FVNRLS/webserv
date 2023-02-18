/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseGenerator.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:22:00 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/08 13:38:47 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "GETRequest.hpp"
#include "POSTRequest.hpp"
#include "DELETERequest.hpp"
#include "PUTRequest.hpp"


class ResponseGenerator {

private:
	request_handler&						_request;
	std::string								_response_body;
	static const std::map<int, std::string>	_reasonPhrases;

	//ERROR MANAGEMENT
	std::string			                    create_error_code_response(int status_code);

	std::string			                    generate_response_header(int status_code);
    std::string                             redirection_response(int status_code);
	static const std::map<int, std::string> make_pairs();


public:
	ResponseGenerator(request_handler &request);
	ResponseGenerator(ResponseGenerator &src);
	~ResponseGenerator();

	std::string			generate_response(Session &cookie);
};
