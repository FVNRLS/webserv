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
#include "tools.hpp"

int	run_servers(const std::vector<Config> &server_configs, std::vector<Server> &servers) {
	pid_t	pid;

	for (size_t i = 0; i < server_configs.size(); i++) {
		pid = fork();
		if (pid < 0) {
			perror("Fork failed!\n");
			return (EXIT_FAILURE);
		}
		if (pid == 0) {
			if (servers[i].run() == EXIT_FAILURE)
				exit (EXIT_FAILURE); //todo: it will be fork!
			exit(EXIT_SUCCESS);
		}
	}
	siginfo_t infop;
	while (waitid(P_ALL, 0, &infop, WEXITED | WSTOPPED | WCONTINUED) == 0);
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	if (argc != 2)
		return (parsing_error_basic(ARG_ERR, argv[0]));

	//PARSING
	std::vector<Config>	server_configs;
	ConfigParser		parser(server_configs, argv[1]);
	std::vector<Server> servers;

	if (parser.parse() == EXIT_FAILURE)
		return (EXIT_FAILURE);
//	print_configurations(server_configs);

	//SERVER CORE
	servers.reserve(server_configs.size());
	for (size_t i = 0; i < server_configs.size(); i++)
		servers[i] = Server(server_configs[i]);

//	if (servers[0].run() == EXIT_FAILURE)
//		return (EXIT_FAILURE);

	return (run_servers(server_configs, servers));
}
