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

//BASIC PARSING ERRORS
#define ARG_ERR							1
#define INVALID_EXTENSION				2
#define NO_SERVERS						3
#define BAD_PERMISSIONS					4

//LINE ERRORS
#define INVALID_SERVER_DEFINITION		5
#define INVALID_SERVER_CONFIGURATION	6
#define INVALID_CHARACTERS_FOUND		7
#define REDEFINITION_OF_SERVER			8



int	print_error(int error, const std::string& config_file);
int	print_line_error(int error, const std::string &config_file, size_t line);
