/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 18:12:06 by doreshev          #+#    #+#             */
/*   Updated: 2023/02/08 13:00:34 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"

struct User {
    std::string                                         name;
    std::string                                         pass;

    User() {}
    User(const std::string& username, const std::string& password) 
        : name(username), pass(password) {}
};

class Session {
private:
    std::map<int, User> _cookies;

public:
	Session();
	~Session();

	int			create(const std::string& username, const std::string& password);
    std::string     username(const int& key) const;
    void            delete_session(const int& key);

private:
    int             get_key(const std::string& username) const;
    bool            password_correct(int key, const std::string& password) const;
    int             generate_key();
};