/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 13:36:33 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/22 13:36:33 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "main.hpp"


class CLI {

private:
	pollfd		_std_in;
	std::string	_input;

	//COMMAND LINE INTERFACE FUNCTIONS

public:
	CLI();
	CLI(const CLI &src);
	CLI &operator=(const CLI &src);
	~CLI();

	int 	start();
	int 	process_input();
	int 	read_input();
};
