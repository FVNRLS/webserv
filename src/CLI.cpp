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
		return  (socket_error(SOCKET_OPEN_ERROR, NULL, 0));
	return (EXIT_SUCCESS);
}

int CLI::check_input() {
	if (read_input() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (_input.empty())
		return (CLI_EMPTY);

	for (std::string::iterator it = _input.begin(); it != _input.end(); it++)
		*it = std::toupper(*it);
	std::cout << _input << std::endl;

	if (_input == "CLI_EXIT\n")
		return (CLI_EXIT);
	if (_input == "CLI_LS\n")
		return (CLI_LS);
	return (CLI_HELP);
}

int CLI::read_input() {
	std::ifstream	std_in;

	std_in.open("/dev/stdin");
	if (!std_in.is_open() || std_in.fail()) {
		socket_error(SOCKET_OPEN_ERROR, NULL, 0);
		return (CLI_FAIL);
	}
	_input.append((std::istreambuf_iterator<char>(std_in)), std::istreambuf_iterator<char>());
	std_in.close();
	return (EXIT_SUCCESS);
}

pollfd CLI::get_pollfd() {
	return (_std_in);
}