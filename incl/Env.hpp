/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoomen <hoomen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:11:06 by hoomen            #+#    #+#             */
/*   Updated: 2023/02/14 15:03:31 by hoomen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "request_handler.hpp"

class Env {
private:
	request_handler&	_request;

	std::string			remote_addr();
	std::string			get_header_value(std::string key);

public:
	Env(request_handler& request);
	~Env();

	void				create();
};