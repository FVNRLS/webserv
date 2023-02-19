/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:26:10 by doreshev          #+#    #+#             */
/*   Updated: 2023/02/17 12:39:33 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "request_handler.hpp"


class RequestParser {

private:
    request_handler             &_request;
    std::vector<std::string>    _locations;
    location                    _location_config;
    int                         _url_type;
    enum                        url_types{SERVER_INDEX, SERVER, LOCATION_INDEX, LOCATION};

    void                        parse_request_line();
    std::vector<std::string>    tokenize_first_line();
    void                        check_chunked();
    void	                    split_query();
    void	                    set_cookies();
    void                        set_url_type();
    void                        set_location_config();
    void                        check_method();
    void	                    set_interpreter();
    void	                    translate_path();
    void	                    check_file_path();
    void                        set_body_length();
    void                        set_cgi_path();
    void                        get_body_length_chunked();
    void                        check_redirection();

public:
	RequestParser(request_handler &request);
	~RequestParser();

	void                        parse();
};
