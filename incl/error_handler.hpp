/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:39:22 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 14:39:22 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

class Config;

//PARSING ERRORS
enum parsing_errors {
	ARG_ERR,
	INVALID_EXTENSION,
	NO_SERVER,
	BAD_PERMISSIONS,
	NO_FILE,

	INVALID_SERVER_DEFINITION,
	NESTED_LOCATION_DEFINITION,
	INVALID_CHARACTERS_FOUND,
	REDEFINITION_OF_SERVER,
	BRACES_NOT_CLOSED,
	TOO_MANY_ARGS,
	NO_SEMICOLON,

	INVALID_IDENTIFIER,
	INVALID_SCOPE,

	INVALID_PARAMETER,
	INVALID_NUM_OF_PARAMETERS,
	REDEFINITION_OF_SERVER_IDENT,
	REDEFINITION_OF_SERVER_PARAM,

	SERVER_NAME_NOT_UNIQUE,
	NO_IP_ADDRESS,
	NO_PORTS,
	NO_ROOT,
	NO_INDEX,
	DUPLICATE_IP_PORT_COMB
};

int	parsing_error_basic(int error, const std::string& config_file);
int	parsing_error_line(int error, const std::string &config_file, size_t line);
int	parsing_error_param(int error, const std::string &config_file, std::string &param);


//SERVER ERRORS
enum server_errors {
	SOCKET_OPEN_ERROR,
	BIND_ERROR,
	CONNECT_ERROR,
	LISTEN_ERROR,
	POLL_ERROR,
	ACCEPT_ERROR,
	CLI_ERROR,
};

int socket_error(int error, const Config *conf, size_t i);
