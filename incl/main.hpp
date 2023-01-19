/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_headers.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:12:41 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 13:12:41 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <map>

#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "error_handler.hpp"

//COVERING UINT32_MAX MACRO FOR ARCH LINUX
#ifndef UINT32_MAX
# define	UINT32_MAX __UINT32_MAX__
#endif

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
	NEGATIVE_OFFSET			= 75
};

//STRINGS
const std::string STR_CLOSED_CURLY_BRACE	=	"}";
const std::string STR_SEMICOLON 			=	";";
const std::string EQUAL_SIGN 				=	"=";

//DEFAULTS
const std::string DEFAULT_INDEX = "index.html";
const std::string DEFAULT_ERROR_PAGES_DIR = "config/default_error_pages";
const std::string DEFAULT_REDIR_FIRST = "config";
const std::string DEFAULT_REDIR_SECOND = "welcome page"; //unsure about the values
const std::string DEFAULT_CGI_PATH = "config/cgi/cgi.py"; //unsure about the values
