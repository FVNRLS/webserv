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
#include "Config.hpp"

int main(int argc, char **argv) {
	if (argc != 2)
		return (print_error(ARG_ERR, argv[0]));

	std::vector<Config>	server_configs;
	ConfigParser		parser(server_configs, argv[1]);

	if (parser.parse() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	

	return (EXIT_SUCCESS);
}

//../config/test.conf