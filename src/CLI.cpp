/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 13:38:32 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/22 17:44:21 by doreshev         ###   ########.fr       */
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

CLI &CLI::operator=(const CLI &src) {
	if (this == &src)
		return (*this);
	_std_in = src._std_in;
	return (*this);
}

CLI::~CLI() {}

/* Unblock the cli_fd to be polled in Server poll() */
int CLI::start() {
	if (fcntl(_std_in.fd, F_SETFL, O_NONBLOCK) < 0)
		return  (cli_error(SOCKET_OPEN_ERROR));
	return (EXIT_SUCCESS);
}

/*
 * The function is used to check the user input from the command line interface.
 * It clears the input buffer and reads input from the standard input.
 * It converts the input to upper case and checks for specific input commands.
 * If the input is "EXIT\n", it returns CLI_EXIT. If the input is "LS\n", it returns CLI_LS.
 * Otherwise, it returns CLI_HELP.
 * */
int CLI::check_input() {
	_input.clear();
	if (read_input() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (_input.empty())
		return (CLI_EMPTY);
	for (std::string::iterator it = _input.begin(); it != _input.end(); it++)
		*it = std::toupper(*it);
	if (_input == "EXIT")
		return (CLI_EXIT);
	if (_input == "LS")
		return (CLI_LS);
	return (CLI_HELP);
}

/*
 * The function in reads data from the standard input and appends it to the _input member variable.
 * The function first opens the standard input device using the open function and checks if it was successfully opened.
 * It then reads the contents of the standard input and
 * returns EXIT_SUCCESS if the input was read successfully, or CLI_FAIL if there was an error.
 * */
int CLI::read_input() {
	int				std_in;
	std::string 	line;
	char 			buf[100];

	std_in = open("/dev/stdin", O_RDONLY);
	if (std_in < 0) {
		cli_error(SOCKET_OPEN_ERROR);
		return (CLI_FAIL);
	}
	read(std_in, &buf, sizeof(buf));
	_input += buf;
	close(std_in);
	_input = _input.substr(0, _input.find(NEWLINE));
	return (EXIT_SUCCESS);
}

pollfd CLI::get_pollfd() {
	return (_std_in);
}

/* error message for failed opening of CLI socket */
int		CLI::cli_error(int error)	const {
	switch(error) {
		case SOCKET_OPEN_ERROR:
			std::cerr << "Error: failed to open CLI socket" << std::endl;
			break;
		default:
			std::cerr << "CLI: unknown error" << std::endl;
	}
	return (EXIT_FAILURE);
}