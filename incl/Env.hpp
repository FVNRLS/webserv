/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoomen <hoomen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 13:11:06 by hoomen            #+#    #+#             */
/*   Updated: 2023/02/06 13:21:53 by hoomen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "requestHandler.hpp"

class Env {
private:
  request_handler&                _request;

	int 				                    set_script_path();
	std::string			                http_user_agent();
	std::string 		                remote_addr();

public:
	Env(request_handler& request);
	~Env();

	void                     				create_env();
};