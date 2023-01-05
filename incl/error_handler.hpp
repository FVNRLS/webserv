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

#define ARG_ERR				1
#define INVALID_EXTENSION	2
#define NO_SERVERS			3
#define BAD_PERMISSIONS		4



int	print_error(int error, const std::string& path);

