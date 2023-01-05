/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:11:11 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 13:11:11 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

class Client {

private:

public:
	//BASIC CLASS SETUP
	Client();
	Client(char *path);
	Client(const Client &src);
	Client &operator=(const Client &src);
	~Client();
};