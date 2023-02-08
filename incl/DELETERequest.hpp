/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETRequest.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 16:46:33 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/30 16:46:33 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "requestHandler.hpp"

class DELETERequest {
private:

public:
	DELETERequest();
	~DELETERequest();

	int		create_response(const request_handler &request);
};