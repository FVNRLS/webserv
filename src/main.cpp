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

#include "Config.hpp"
#include "Server.hpp"

int main(int argc, char **argv) {
	Config				conf(argv[1]);
	std::vector<Server> servers;

	if (argc != 2)
		return (print_error(ARG_ERR, argv[1]));

	if (conf.parse(servers, argv[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	

	return (EXIT_SUCCESS);
}
