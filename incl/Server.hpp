/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoomen <hoomen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:36:52 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/02 09:44:39 by hoomen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CLI.hpp"
#include "ResponseGenerator.hpp"



class Server {

private:
	const std::vector<Socket>					&_sockets;
	std::vector<pollfd>							_pfds;
	CLI											_cli;
	std::map<int, request_handler>				_requests;



	//CORE FUNCTIONS
	int							check_cli();
	int 						accept_requests();
	int 						resolve_requests();
	int							accumulate(std::map<int, request_handler>::iterator	request);
	void 						set_request_end_flags(request_handler&	request);
	int 						handle_request_header(std::map<int, request_handler>::iterator	request);
	size_t						get_body_length(request_handler &request);
	std::vector<std::string> 	tokenize(std::string& request);
	std::vector<std::string> 	get_allowed_methods(std::map<int, request_handler>::iterator	request);
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

public:
	Server(std::vector<Socket> &sockets);
	~Server();

	int 						run();
};
