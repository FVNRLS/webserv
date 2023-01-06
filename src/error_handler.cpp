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

int	print_error(int error, const std::string &path) {

	std::string	filename;
	size_t 		slash_pos;

	slash_pos = path.find_last_of('/');
	if (slash_pos == std::string::npos)
		filename = path;
	else
		filename = path.substr(slash_pos + 1);

	if (error == ARG_ERR)
		std::cerr << "Error: invalid number of arguments" << std::endl;
	else if (error == INVALID_EXTENSION)
		std::cerr << "Error: invalid extension of configuration file " << filename << std::endl;
//	else if (error == NO_SERVERS)
//		std::cerr << "Error: no server specified in the configuration file " << filename << std::endl;
	else if (error == BAD_PERMISSIONS)
		std::cerr << "Error: invalid permissions of " << filename << std::endl;

	return (EXIT_FAILURE);
}

int	print_line_error(int error, const std::string &path, int line) {
	std::string	filename;
	size_t 		slash_pos;

	slash_pos = path.find_last_of('/');
	if (slash_pos == std::string::npos)
		filename = path;
	else
		filename = path.substr(slash_pos + 1);

	if (error == INVALID_SERVER_DEFINITION)
		std::cerr << "Error: invalid server definition in the configuration file "
			<< filename <<  "on line: " << line << std::endl;
	return (EXIT_FAILURE);
}
