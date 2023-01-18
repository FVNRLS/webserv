/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:39:22 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 14:39:22 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

enum errors {
	ARG_ERR,
	INVALID_EXTENSION,
	NO_SERVER,
	BAD_PERMISSIONS,

	INVALID_SERVER_DEFINITION,
	INVALID_CHARACTERS_FOUND,
	REDEFINITION_OF_SERVER,
	BRACES_NOT_CLOSED,
	TOO_MANY_ARGS,
	NO_SEMICOLON,

	INVALID_IDENTIFIER,
	INVALID_SCOPE,

	INVALID_PARAMETER,
	INVALID_NUM_OF_PARAMETERS,
	REDEFINITION_OF_SERVER_PARAMETER,
};


int	print_error(int error, const std::string& config_file);
int	print_line_error(int error, const std::string &config_file, size_t line);
