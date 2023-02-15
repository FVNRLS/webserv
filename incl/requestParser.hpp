/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:26:10 by doreshev          #+#    #+#             */
/*   Updated: 2023/02/14 17:47:12 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "request_handler.hpp"


class requestParser {

private:
    request_handler             &_request;
    std::vector<std::string>    _locations;
    location                    _location_config;
    int                         _url_type;
    enum                        url_types{SERVER_INDEX, SERVER, LOCATION_INDEX, LOCATION};

    void                        parse_request_line();
    std::vector<std::string>    tokenize_first_line();
    void	                    split_query();
    void	                    set_cookies();
    void                        set_url_type();
    void                        set_location_config();
    void                        check_method();
    void	                    set_interpreter();
    void	                    translate_path();
    void                        set_body_length();


public:
	requestParser(request_handler &request);
	~requestParser();

	void                        parse();
};