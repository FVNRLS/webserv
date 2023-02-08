/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 18:51:52 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/08 13:02:07 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"

std::string 				trim(std::string &s);
std::vector<std::string>	split(std::string &s, char sep);
void						silence_sigint();
int 						open_file(const std::string &file_path, std::ifstream &file);
int 						count_occurrences(const std::string &haystack, const std::string &needle);


template <class T>
std::string	toString(T i) {
	std::stringstream ss;
    ss << i;
    return ss.str();
}