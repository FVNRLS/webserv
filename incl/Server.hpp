/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 13:36:52 by rmazurit          #+#    #+#             */
/*   Updated: 2023/02/03 18:41:21 by doreshev         ###   ########.fr       */
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
	int							handle_pollin(pollfd& pfd);
	int							handle_pollout(pollfd& pfd);
	int							accumulate(request_handler& request, int request_fd);
	void 						set_request_end_flags(request_handler& request);
	int 						handle_request_header(request_handler& request);
	size_t						get_body_length(request_handler &request);
	std::vector<std::string> 	tokenize_first_line(std::string& request);
	std::vector<std::string> 	get_methods(request_handler& request);
	std::vector<std::string>	get_methods_in_location(request_handler& request, std::vector<std::string>& locations);
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
