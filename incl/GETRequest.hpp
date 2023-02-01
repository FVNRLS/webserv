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

#include "Socket.hpp"

class GETRequest {

private:
	std::string	_body;

	int 		create_response_body(const std::string &file_path);

public:
	GETRequest();
	~GETRequest();

	int 		create_response(const std::string &file_path, std::string &_response);
};