/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:20:38 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/26 12:20:38 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

//BASIC CLASS SETUP
ResponseGenerator::ResponseGenerator(const std::string &response) : _response(response) {

}

ResponseGenerator::ResponseGenerator(const ResponseGenerator &src) { *this = src; }

ResponseGenerator	&ResponseGenerator::operator=(const ResponseGenerator &src) {
	if (this == &src)
		return (*this);
	_response = src._response;
	return (*this);
}

ResponseGenerator::~ResponseGenerator() {}


//MEMBER FUNCTIONS
std::string	ResponseGenerator::get_location() {
	return ("");
}
