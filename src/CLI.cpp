/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 13:38:32 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/22 13:38:32 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CLI.hpp"

CLI::CLI() {
	_std_in.events = POLLIN;
	_std_in.fd = STDIN_FILENO;
}

CLI::CLI(const CLI &src) {
	*this = src;
}

//TODO: complete in the end!
CLI &CLI::operator=(const CLI &src) {
	if (this == &src)
		return (*this);
	_std_in = src._std_in;
	return (*this);
}

CLI::~CLI() {}

int CLI::start() {
	if (fcntl(_std_in.fd, F_SETFL, fcntl(_std_in.fd, F_GETFL) | O_NONBLOCK) < 0)
		return  (server_error(SOCKET_OPEN_ERROR, NULL, 0));
	return (EXIT_SUCCESS);
}

int CLI::process_input() {
	if (read_input() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	for (std::string::iterator it = _input.begin(); it != _input.end(); it++)
		*it = std::toupper(*it);
	std::cout << _input << std::endl;

	if (_input.empty())
		return (CLI_EMPTY);
	if (_input == "EXIT\n")
		return (EXIT);
	if (_input == "LS\n")
		return (LS);
	return (HELP);
}

int CLI::read_input() {
	std::ifstream	std_in;

	std_in.open("/dev/stdin");
	if (!std_in.is_open() || std_in.fail()) {
		server_error(SOCKET_OPEN_ERROR, NULL, 0);
		return (CLI_FAIL);
	}
	_input.append((std::istreambuf_iterator<char>(std_in)), std::istreambuf_iterator<char>());
	std_in.close();
	return (EXIT_SUCCESS);
}
