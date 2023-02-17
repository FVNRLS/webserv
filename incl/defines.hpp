/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:28:14 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/09 14:29:06 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <map>
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
	NOT_FOUND			= (-1),
	SPEC_IDENTIFIER		= (-2),
	OPEN_CURLY_BRACE	= '{',
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
	MAX_CONNECTIONS			= SOMAXCONN,
};

//STRINGS SEPARATORS
const std::string STR_CLOSED_CURLY_BRACE	=	"}";
const std::string STR_SEMICOLON 			=	";";
const std::string EQUAL_SIGN				=	"=";
const std::string EMPTY_STRING;

//DEFAULT STRING CONSTANTS
const std::string RESPONSE_HEADER			= "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
const std::string DEFAULT_CONF_PATH			= "config/clion_test.conf";
const std::string DEFAULT_INDEX_PAGE		= "html/index.html";
const std::string DEFAULT_ERROR_PAGES_DIR 	= "html/error_pages/";
const std::string DEFAULT_REDIR_FIRST		= "config";
const std::string DEFAULT_REDIR_SECOND		= "welcome page"; //unsure about the values
const std::string DEFAULT_PAGE_ERROR		= "SERVER ERROR: ERROR PAGE UNAVAILABLE!"; //if error page can not be opened
const std::string END_OF_REQUEST			= "\r\n\r\n";
const std::string CRLF          			= "\r\n";

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

//SYSTEM CALL ERRORS
enum server_system_call_errors {
	CLI_ERROR,
	POLL_ERROR,
	ACCEPT_ERROR,
	RECV_ERROR,
	SEND_ERROR,
	ACCESS_DENIED,
};

//RESPONSE CODES
enum response_codes {
		//INFORMATIONAL RESPONSE
		CONTINUE 						= 100,
		SWITCHING_PROTOCOL				= 101,
		PROCESSING						= 102,
		EARLY_HINTS						= 103,

		//SUCCESS
		OK								= 200,
		CREATED							= 201,
		ACCEPTED						= 202,
		NON_AUTHORITATIVE_INFO			= 203,
		NO_CONTENT						= 204,
		RESET_CONTENT					= 205,
		PARTIAL_CONTENT					= 206,
		MULTI_STATUS					= 207,
		ALREADY_REPORTED				= 208,
		IM_USED							= 226,

		//REDIRECTION
		MULTIPLE_CHOICES				= 300,
		MOVED_PERMANENTLY				= 301,
		FOUND							= 302,
		SEE_OTHER						= 303,
		NOT_MODIFIED					= 304,
		USE_PROXY						= 305,
		UNUSED							= 306,
		TEMPORARY_REDIRECT				= 307,
		PERMANENT_REDIRECT				= 308,

		//CLIENT ERRORS
		BAD_REQUEST 					= 400,
		UNAUTHORIZED					= 401,
		PAYMENT_REQUIRED				= 402,
		FORBIDDEN						= 403,
		PAGE_NOT_FOUND					= 404,
		METHOD_NOT_ALLOWED				= 405,
		NOT_ACCEPTABLE					= 406,
		PROXY_AUTHENTICATION_REQUIRED	= 407,
		REQUEST_TIMEOUT					= 408,
		CONFLICT						= 409,
		GONE							= 410,
		LENGTH_REQUIRED					= 411,
		PRECONDITION_FAILED				= 412,
		PAYLOAD_TOO_LARGE				= 413,
		URI_TOO_LONG					= 414,
		UNSUPPORTED_MEDIA_TYPE			= 415,
		RANGE_NOT_SATISFIABLE			= 416,
		EXPECTATION_FAILED				= 417,
		IM_A_TEAPOT						= 418,
		MISDIRECTED_REQUEST				= 421,
		UNPROCESSABLE_ENTITY			= 422,
		LOCKED							= 423,
		FAILED_DEPENDENCY				= 424,
		TOO_EARLY						= 425,
		UPGRADE_REQUIRED				= 426,
		PRECONDITION_REQUIRED			= 428,
		TOO_MANY_REQUESTS 				= 429,
		REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
		UNAVAILABLE_FOR_LEGAL_REASONS	= 451,

		//UNOFFICIAL NGINX CODES
		NO_RESPONSE 					= 444,
		REQUEST_HEADER_TOO_LARGE		= 494,
		SSL_CERTIFICATE_ERROR			= 495,
		SSL_CERTIFICATE_REQUIRED		= 496,
		HTTP_REQUEST_SENT_TO_HTTPS_PORT	= 497,
		CLIENT_CLOSED_REQUEST			= 499,

		//SERVER ERRORS
		INTERNAL_SERVER_ERROR 			= 500,
		NOT_IMPLEMENTED 				= 501,
		BAD_GATEWAY 					= 502,
		SERVICE_UNAVAILABLE 			= 503,
		GATEWAY_TIMEOUT 				= 504,
		HTTP_VERSION_NOT_SUPPORTED 		= 505,
		VARIANT_ALSO_NEGOTIATES 		= 506,
		INSUFFICIENT_STORAGE 			= 507,
		LOOP_DETECTED 					= 508,
		NOT_EXTENDED 					= 510,
		NETWORK_AUTHENTICATION_REQUIRED = 511,
};

enum cli_flags {
	CLI_EMPTY,
	CLI_FAIL,
	CLI_EXIT,
	CLI_LS,
	CLI_HELP,
};

enum server_constants {
	TIMEOUT	 	= 5000,
    CHUNK_SIZE  = 40000,
};
