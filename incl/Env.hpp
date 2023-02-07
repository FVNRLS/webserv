/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoomen <hoomen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:11:06 by hoomen            #+#    #+#             */
/*   Updated: 2023/02/07 11:06:40 by hoomen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "requestHandler.hpp"

class Env {
private:
	request_handler&					_request;

	std::string     http_user_agent();
	std::string     remote_addr();
	std::string			get_content_type();
	// std::string			get_header_value(const char* key);

public:
	Env(request_handler& request);
	~Env();

	void								create();
};