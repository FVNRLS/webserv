/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 12:56:00 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 12:56:00 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//BASIC CLASS SETUP
Server::Server() {}

Server::Server(const Server &src) {
	*this = src;
}

Server &Server::operator=(const Server &src) {
	if (this == &src)
		return (*this);
	return (*this);
}

Server::~Server() {}

