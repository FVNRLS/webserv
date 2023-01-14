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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "error_handler.hpp"
#include "tools.hpp"

#ifndef UINT32_MAX
# define	UINT32_MAX __UINT32_MAX__
#endif

//CHARACTERS
#define NOT_FOUND				(-1)
#define SPEC_IDENTIFIER			(-2)
#define	OPEN_CURLY_BRACE		'{'
#define	CLOSED_CURLY_BRACE		'}'
#define NEWLINE					'\n'
#define SPACE					' '
#define SEMICOLON				';'
#define DOT						'.'
#define HASH					'#'
#define NULL_TERM				'\0'
#define TAB						'\t'


//STRINGS
const std::string STR_CLOSED_CURLY_BRACE	=	"}";
const std::string STR_SEMICOLON 			=	";";
const std::string EQUAL_SIGN 				=	"=";

//NUMERIC CONSTANTS
#define MAX_PORT_NUM			65535
#define MIN_CLIENT_BODY_SIZE	10

//DEFAULTS
const std::string DEFAULT_INDEX = "index.html";
const std::string DEFAULT_ERROR_PAGES_DIR = "config/default_error_pages";
const std::string DEFAULT_REDIR_FIRST = "config";
const std::string DEFAULT_REDIR_SECOND = "welcome page"; //unsure about the values
