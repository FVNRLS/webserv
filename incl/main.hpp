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
#include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#include "error_handler.hpp"
#include "tools.hpp"

#define NOT_FOUND			(-1)
#define	OPEN_CURLY_BRACE	'{'
#define	CLOSED_CURLY_BRACE	'}'
#define NEWLINE				'\n'
#define SPACE				' '
#define SEMICOLON			';'
#define HASH				'#'
#define NULL_TERM			'\0'
#define TAB					'\t'

