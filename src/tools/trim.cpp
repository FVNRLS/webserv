/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 18:49:10 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/06 18:49:10 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

std::string trim(std::string &s) {
	size_t	front_pos;
	size_t	back_pos;

	if (s.empty())
		return ("");
	front_pos = s.find_first_not_of(SPACE);
	back_pos = s.find_last_not_of(SPACE);
	s = s.substr(front_pos, back_pos + 1);

	front_pos = s.find_first_not_of(TAB);
	back_pos = s.find_last_not_of(TAB);
	s = s.substr(front_pos, back_pos + 1);

	return (s.substr(front_pos, back_pos + 1));
}