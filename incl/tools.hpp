/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 18:51:52 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/08 13:09:19 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"
#include <sys/stat.h>

std::string 				trim(std::string &s);
std::vector<std::string>	split(std::string &s, char sep);
void						silence_sigint();
int							open_file(const std::string &file_path, std::ifstream &file);
int							count_occurrences(const std::string &haystack, const std::string &needle);
bool                        is_regular_file(const char* fileName);
std::string                 get_header_field_value(const char* key,
                                                   std::string const& buf);
bool                        header_value_is_equal_to(const char* key, const
                                                    char* value, std::string
                                                    const& buf);
bool                        header_key_exists(const char* key, std::string
                                                    const& buf);
void                        trim_first_line(std::string& buf);
void                        trim_endofrequest(std::string& buf);
std::string                 get_header_value(const char* key, std::string& buf);

template <class T>
std::string	toString(T i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}