/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETRequest.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 16:46:33 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/30 16:46:33 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Env.hpp"
#include "CGI.hpp"

class GETRequest {

private:
	request_handler&	_request;
	CGI					_cgi;
	Env					_environment(_request);

	int					create_html_response(std::string &_response);
	int					create_cgi_response(std::string &_response);


public:
	GETRequest(request_handler& request);
	~GETRequest();

	int 				create_response(std::string &_response);
};