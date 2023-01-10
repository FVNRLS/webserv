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
	size_t 	front_pos;
	size_t 	back_pos;

	if (s.empty())
		return ("");
	front_pos = s.find_first_not_of(SPACE);
	if (front_pos == std::string::npos)
		return (s);
	back_pos = s.find_last_not_of(SPACE);
//	std::cout << front_pos << " , " << back_pos <<  " , " << s.substr(front_pos, back_pos + 1) << std::endl;
	if (front_pos != back_pos)
		s = s.substr(front_pos, back_pos + 1);

	front_pos = s.find_first_not_of(TAB);
	if (front_pos == std::string::npos)
		return (s);
	back_pos = s.find_last_not_of(TAB);
	if (front_pos != back_pos)
		s = s.substr(front_pos, back_pos + 1);

	return (s);
}