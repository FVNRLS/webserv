/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmazurit <rmazurit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:28:14 by rmazurit          #+#    #+#             */
/*   Updated: 2023/01/05 13:28:14 by rmazurit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

class Config {

private:

public:

	typedef struct location_info {
		std::string	directory;
		bool		GET;
		bool		POST;
		bool		DELETE;

		bool		dir_listing;
		std::string	def_file;
		std::string	root;	//if root empty -> uses the server_root
		size_t		id;
		size_t		upload_size;
		bool		upload_size_bool;
		std::string	redirect;
		location_info();
	} loc_inf;

};
