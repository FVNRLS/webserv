/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:12:08 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 13:12:08 by rmazurit         ###   ########.fr       */
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

	server_configs = parser.parse();
	if (server_configs.empty())
		return (EXIT_FAILURE);

	//CREATING PULL OF ACTIVE SOCKETS
	for (size_t i = 0; i < server_configs.size(); i++) {
		size_t	num_ports = server_configs[i].get_ports().size();
		for (size_t j = 0; j < num_ports; j++) {
			Socket	new_socket(server_configs[i], j);
			if (new_socket.activate() == EXIT_SUCCESS)
				sockets.push_back(new_socket);
		}
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
