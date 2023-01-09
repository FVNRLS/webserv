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

#include "main.hpp"

std::string	trim(std::string &s);
std::vector<std::string> split(std::string &s, char sep);


//TEMPLATES
template <typename T>

void	print_vector(T vector, size_t size) {
	if (size == 0)
		return;
	for (int i = 0; i < size; i++) {
		std::cout << vector[i] << ' ';
	}
	std::cout << std::endl;
}