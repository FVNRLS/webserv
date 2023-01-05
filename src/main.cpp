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

	int num_servers;

	if (argc != 2)
		return (print_error(ARG_ERR, argv[1]));

	Config	conf(argv[1]);
	if (conf.check_extension() == false)
		return (print_error(INVALID_EXTENSION, argv[1]));

	num_servers = conf.count_servers();
	if (num_servers == 0)
		return (print_error(NO_SERVERS, argv[1]));
	else if (num_servers == -1)
		return (print_error(BAD_PERMISSIONS, argv[1]));
	Server	servers[num_servers];

	conf.parse(*servers);
	if (conf.get_is_parsed() == false)
		return (1);

	return (0);
}
