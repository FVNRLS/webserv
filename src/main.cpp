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

#include "ConfigParser.hpp"
#include "Server.hpp"

int main(int argc, char **argv) {
	if (argc != 2)
		return (parsing_error_basic(ARG_ERR, argv[0]));

	//PARSING
	std::vector<Config>	server_configs;
	ConfigParser		parser(server_configs, argv[1]);

	if (parser.parse() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	for (int i = 0; i < server_configs.size(); i++)
		std::cout << &server_configs[i] << std::endl;

	//SERVER CORE
	Server	server(server_configs);
	if (server.start() == EXIT_FAILURE)
		return (EXIT_FAILURE);


	

	return (EXIT_SUCCESS);
}
