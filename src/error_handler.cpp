/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:42:34 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 14:42:34 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_handler.hpp"

int	print_error(int error, const std::string &file) {
	if (error == ARG_ERR)
		std::cerr << "Error: invalid number of arguments" << std::endl;
	else if (error == INVALID_EXTENSION)
		std::cerr << "Error: invalid extension of file " << file << std::endl;
	else if (error == NO_SERVERS)
		std::cerr << "Error: no server specified in the configuration file " << file << std::endl;

	return (1);
}