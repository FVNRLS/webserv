/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:36:52 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/12 13:36:52 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CLI.hpp"

class Server {

private:
	const std::vector<Socket>	&_sockets;
	std::vector<pollfd>			_poll_fds;
	size_t 						_num_fds;
	CLI							_cli;

	//CORE FUNCTIONS
	int 						process_request(const Socket &socket, pollfd &poll_fd);
	int 						serve_on_port(const Socket &socket, pollfd &poll_fd);
	int 						serve_on_virtual_host(const Socket &socket, pollfd &poll_fd);
	std::string 				extract_domain(std::string &request);


	std::string 				get_request(int &socket);
	std::string 				generate_response(const std::string &request);
	std::string					get_requested_path(const std::string &request);

	//TERMINAL INTERACTION
	int							process_cli_input();
	void						exit_server();
	int 						terminate_with_error(int);
	void 						show_connections();
	void						print_configurations();
	void 						show_manual();

	//ERROR MANAGEMENT
	int 						server_error(int error, const Socket &socket);


public:
	Server(std::vector<Socket> &sockets);
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

	int 						run();
};




