/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 18:12:06 by doreshev          #+#    #+#             */
/*   Updated: 2023/02/07 19:57:20 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>

struct User
{
    std::string                                         name;
    std::string                                         pass;
    std::string                                         last_access;
    bool                                                logged;

    User() {}
    User(const std::string& username, const std::string& password) : name(username),
            pass(password), logged(true) {
        time_t  current_time = time(0);
        last_access = ctime(&current_time);
    }
};

class Session {
private:
    std::map<int, User> _cookies;

public:
	Session();
	~Session();

	int 			create(const std::string& username, const std::string& password);
    int             logout(const int& key);
    
    
    std::string     username(const int& key) const;
    std::string     last_access(const int& key) const;
    bool            logged(const int& key) const;
    
    void            delete_session(const int& key);

private:
    int             generate_key();
    bool            user_exists(const std::string& username);
};