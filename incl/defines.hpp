/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:28:14 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 13:28:14 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <termios.h>
#include <csignal>
#include <cstddef>
#include <poll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

template <typename T>
void	print_vector(T vector, size_t size) {
	if (size == 0)
		return;
	for (size_t i = 0; i < size; i++) {
		if (i < size - 1)
			std::cout << vector[i] << ' ';
		else
			std::cout << vector[i];
	}
	std::cout << std::endl;
}

//CHARACTERS
enum chars {
	NOT_FOUND 			= (-1),
	SPEC_IDENTIFIER 	= (-2),
	OPEN_CURLY_BRACE 	= '{',
	CLOSED_CURLY_BRACE	= '}',
	NEWLINE				= '\n',
	SPACE				= ' ',
	SEMICOLON			= ';',
	DOT					= '.',
	HASH				= '#',
	NULL_TERM			= '\0',
	TAB					= '\t'
};



//NUMERIC CONSTANTS
enum num_constants {
	MAX_PORT_NUM			= 65535,
	MIN_CLIENT_BODY_SIZE	= 10,
	NEGATIVE_OFFSET			= 75,
	MAX_CONNECTIONS			= 5,
};

//STRINGS
const std::string STR_CLOSED_CURLY_BRACE	=	"}";
const std::string STR_SEMICOLON 			=	";";
const std::string EQUAL_SIGN 				=	"=";

//DEFAULTS
const std::string DEFAULT_PATH = "config/test.conf";
const std::string DEFAULT_INDEX_PAGE = "html/index.html";
const std::string DEFAULT_ERROR_PAGES_DIR = "config/default_error_pages";
const std::string DEFAULT_REDIR_FIRST = "config";
const std::string DEFAULT_REDIR_SECOND = "welcome page"; //unsure about the values
const std::string DEFAULT_CGI_PATH = "config/cgi/cgi.py"; //unsure about the values

const std::string RESPONSE_HEADER = "HTTP/1.1 200 OK\nContent-Type: html\nContent-Length: ";

//COVERING UINT32_MAX MACRO FOR ARCH LINUX
#ifndef UINT32_MAX
# define	UINT32_MAX __UINT32_MAX__
#endif


//PARSING ERRORS
enum parsing_errors {
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

//SOCKET ERRORS
enum socket_errors {
	SOCKET_OPEN_ERROR,
	BIND_ERROR,
	LISTEN_ERROR,
};

//SERVER ERRORS
enum server_errors {
	CLI_ERROR,
	POLL_ERROR,
	ACCEPT_ERROR,
	RECV_ERROR,
	ERROR_404,
	ACCESS_DENIED
};

enum cli_flags {
	CLI_EMPTY,
	CLI_FAIL,
	CLI_EXIT,
	CLI_LS,
	CLI_HELP,
};