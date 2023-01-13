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

int	print_error(int error, const std::string &config_file) {

	std::string	filename;
	size_t 		slash_pos;

	slash_pos = config_file.find_last_of('/');
	if (slash_pos == std::string::npos)
		filename = config_file;
	else
		filename = config_file.substr(slash_pos + 1);

	if (error == ARG_ERR)
		std::cerr << "Error: invalid number of arguments" << std::endl;
	else if (error == INVALID_EXTENSION)
		std::cerr << "Error: invalid extension of configuration file " << filename << std::endl;
	else if (error == NO_SERVER)
		std::cerr << "Error: no server specified in configuration file " << filename << std::endl;
	else if (error == BAD_PERMISSIONS)
		std::cerr << "Error: invalid permissions of " << filename << std::endl;

	return (EXIT_FAILURE);
}

int	print_line_error(int error, const std::string &config_file, size_t line) {
	std::string	filename;
	size_t 		slash_pos;

	slash_pos = config_file.find_last_of('/');
	if (slash_pos == std::string::npos)
		filename = config_file;
	else
		filename = config_file.substr(slash_pos + 1);

	if (error == INVALID_SERVER_DEFINITION)
		std::cerr << "Error: invalid server definition in configuration file "
			<< filename <<  " on line: " << line << std::endl;
	else if (error == INVALID_CHARACTERS_FOUND)
		std::cerr << "Error: invalid characters found in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	else if (error == REDEFINITION_OF_SERVER)
		std::cerr << "Error: redefinition of server in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	else if (error == BRACES_NOT_CLOSED)
		std::cerr << "Error: curly braces are not closed in server definition in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	else if (error == TOO_MANY_ARGS)
		std::cerr << "Error: Too many arguments in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	else if (error == NO_SEMICOLON)
		std::cerr << "Error: missing ';' in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	else if (error == INVALID_IDENTIFIER)
		std::cerr << "Error: invalid identifier found in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	else if (error == INVALID_SCOPE)
		std::cerr << "Error: parameter defined in an invalid scope in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	else if (error == REDEFINITION_OF_SERVER_PARAMETER)
		std::cerr << "Error: redefinition of server identifier in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	else if (error == INVALID_NUM_OF_PARAMETERS)
		std::cerr << "Error: invalid number of parameters in the definition of a server identifier in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	else if (error == INVALID_PARAMETER)
		std::cerr << "Error: invalid parameter in the definition of a server identifier in configuration file "
				  << filename <<  " on line: " << line << std::endl;
	return (EXIT_FAILURE);
}

