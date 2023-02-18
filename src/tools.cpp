/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 11:44:45 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/03 14:03:48 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools.hpp"

std::string trim(std::string &s) {
	size_t 	front_pos;
	size_t 	back_pos;

	//TRIM SPACES
	if (s.empty())
		return ("");
	front_pos = s.find_first_not_of(SPACE);
	if (front_pos == std::string::npos)
		return (s);
	back_pos = s.find_last_not_of(SPACE);
	if (front_pos != back_pos)
		s = s.substr(front_pos, back_pos + 1);

	//TRIM TABS
	front_pos = s.find_first_not_of(TAB);
	if (front_pos == std::string::npos)
		return (s);
	back_pos = s.find_last_not_of(TAB);
	if (front_pos != back_pos)
		s = s.substr(front_pos, back_pos + 1);
	return (s);
}

std::vector<std::string>	split(std::string &s, char sep) {
	std::vector<std::string>	tokens;
	std::string 				tok;
	std::istringstream 			iss(s);

	s = trim(s);
	while (getline(iss, tok, sep)) {
		tok = trim(tok);
		if (tok.empty())
			continue;
		tokens.push_back(tok);
	}
	return (tokens);
}

void silence_sigint() {
	struct sigaction action;
	struct termios	term;

	tcgetattr(0, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &term);
	action.sa_handler = SIG_IGN;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
}

int open_file(const std::string &file_path, std::ifstream &file) {
	file.open(file_path);
	if (!file.is_open() || file.fail())
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int count_occurrences(const std::string &haystack, const std::string &needle) {
	if (needle.empty())
		return 0;
	int count = 0;
	std::string::size_type pos = 0;
	while ((pos = haystack.find(needle, pos)) != std::string::npos) {
		count++;
		pos += needle.size();
	}
	return count;
}

bool is_regular_file(const char* fileName)
{
    struct stat path;

    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}

std::string get_header_field_value(const char* key, std::string const& buf) {
    size_t keypos = buf.find(key);
    if (keypos == std::string::npos) {
//        std::cerr << "get_header_field_value: key not found\n";
        return "";
    }
    size_t value_offset = keypos + std::strlen(key);
    while (std::isspace(buf[value_offset]))
        ++value_offset;
    if (value_offset >= buf.size()) {
//        std::cerr << "get_header_field_value: value_offset not found\n";
        return "";
    }
    std::string value = buf.substr(value_offset);
    size_t end_of_line = value.find('\n');
    if (end_of_line == std::string::npos) {
//        std::cerr << "get_header_field_value: end_of_line not found\n";
        return "";
    }
    value = value.substr(0, end_of_line);
//    std::cerr << "untrimmed value = " << value << std::endl;
    value = value.substr(0, value.find_first_of(" \t\r"));
//    std::cerr << "trimmed value = --" << value << "--" << std::endl;
    return value;
}

bool header_value_is_equal_to(const char* key, const char* value, std::string
const& buf) {
    if (get_header_field_value(key, buf) == value)
        return true;
//    std::cerr << "value of " << key << " field is: " <<
//    get_header_field_value(key, buf) << std::endl;
    return false;
}

bool header_key_exists(const char* key, std::string const& buf) {
    size_t keypos = buf.find(key);
    if (keypos == std::string::npos)
        return false;
    return true;
}

void   trim_first_line(std::string& buf) {
    size_t endofline = buf.find("\r\n");
    if (endofline == std::string::npos)
        return ;
    buf = buf.substr(endofline + 2);
}

void    trim_endofrequest(std::string& buf) {
    buf = buf.substr(0, buf.find_last_of(END_OF_REQUEST));
}

std::string get_header_value(const char* key, std::string &buf) {
    size_t begin;
    size_t end;

    begin = buf.find(key);
    if (begin == std::string::npos)
        return EMPTY_STRING;
    begin += std::strlen(key);
    while (buf[begin] == SPACE)
        begin++;
    end = buf.find(NEWLINE, begin);
    if (end == std::string::npos)
        return EMPTY_STRING;
    return buf.substr(begin, end - begin);
}