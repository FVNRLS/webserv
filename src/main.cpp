/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:12:08 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/23 13:14:29 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int main(int argc, char **argv) {
	//PARSING
	if (argc > 2) {
		std::cerr << "Error: invalid number of arguments" << std::endl;
		return (EXIT_FAILURE);
	}
	std::vector<Config>	server_configs;
	ConfigParser		parser(server_configs, argv[1]);
	std::vector<Socket>	sockets;

	if (parser.parse() == EXIT_FAILURE)
		return EXIT_FAILURE;

	//CREATING PULL OF ACTIVE SOCKETS //TODO: problem with unique flag!
	for (size_t i = 0; i < server_configs.size(); i++) {
		Socket	new_socket(server_configs[i]);
		if (new_socket.activate() == EXIT_SUCCESS)
			sockets.push_back(new_socket);
	}

	//SERVER CORE (MAIN LOOP)
	if (!sockets.empty()) {
		silence_sigint();
		Server	server(sockets);
		if (server.run() == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
