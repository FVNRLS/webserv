/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 18:51:52 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/06 18:51:52 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tools.hpp"

//TEMPLATES
template <typename T>

void	print_vector(T vector, size_t size) {
	if (size == 0)
		return;
	for (size_t i = 0; i < size; i++) {
		if (i < size - 1)
			std::cout << vector[i] << ' ';
		else
			std::cout << vector[i];
	}
	std::cout << std::endl;
}

void	print_configurations(std::vector<Config> &server_configs) {
	for (size_t i = 0; i < server_configs.size(); i++)
		std::cout << &server_configs[i] << std::endl;
}
