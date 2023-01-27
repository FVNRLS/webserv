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

struct request_handler {
	Socket		socket;
	std::string buf;

};

class Server {

private:
	const std::vector<Socket>		&_sockets;
	std::vector<pollfd>				_pfds;
	CLI								_cli;
	std::map<int, request_handler>	_requests;

	//CORE FUNCTIONS
	int							check_cli();
	int 						accept_requests();
	int 						resolve_requests();
	int							get_request(int &client_fd);
	std::string 				generate_response(const std::string &request);
	std::string					get_requested_path(const std::string &request);
	int 						check_connection(pollfd& pfd);
	void						delete_invalid_fds();


	//VIRTUAL HOSTING //todo: later!
	int 						process_request(const Socket &socket, pollfd &poll_fd);
	int 						serve_on_virtual_host(const Socket &socket, pollfd &poll_fd);
	std::string 				extract_domain(std::string &request);

	//TERMINAL INTERACTION
	int							process_cli_input();
	void						exit_server();
	int 						terminate_with_error(int);
	void 						show_connections();
	void						print_configurations();
	void 						show_manual();

	//ERROR MANAGEMENT
	int 						system_call_error(int error, const Socket &socket = Socket());
	int 						server_error(int error, int pfd, const Socket &socket = Socket());

		public:
	Server(std::vector<Socket> &sockets);
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

	int 						run();
};




