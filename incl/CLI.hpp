/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 13:36:33 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/08 12:49:43 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"


class CLI {

private:
	pollfd		_std_in;
	std::string	_input;

	//ERROR MANAGEMENT
	int		cli_error(int error)	const;

public:
	CLI();
	CLI(const CLI &src);
	CLI &operator=(const CLI &src);
	~CLI();

	int		start();
	int		check_input();
	int		read_input();

	pollfd	get_pollfd();
};
