/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:11:06 by hoomen            #+#    #+#             */
/*   Updated: 2023/02/08 12:56:36 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "requestHandler.hpp"

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