/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 18:13:52 by doreshev          #+#    #+#             */
/*   Updated: 2023/02/08 18:16:24 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"

//BASIC CLASS SETUP
Session::Session() { }

Session::~Session() {}


//MEMBER FUNCTIONS
int Session::create(const std::string& username, const std::string& password) {
    int key = get_key(username);

    if (key == true) {
        if (password_correct(key, password) == false)
            return false;
        return key;
    }

    key = generate_key();
    _cookies[key] = User(username, password);
    return key;
}

bool    Session::exists(const int& key) const {
    if (_cookies.find(key) == _cookies.end())
        return false;
    return true;
}

int    Session::get_key(const std::string& username) const {
    for (std::map<int, User>::const_iterator it = _cookies.begin(); it != _cookies.end(); it++) {
        if (it->second.name == username)
            return it->first;
    }
    return false;
}

bool    Session::password_correct(int key, const std::string& password) const {
    std::map<int, User>::const_iterator it = _cookies.find(key);

    if (it != _cookies.end() && it->second.pass == password)
        return true;
    return false;
}

int     Session::generate_key() {
    srand(time(0));

    int key = rand();
    while (key == false || _cookies.find(key) != _cookies.end())
        key = rand();
    return key;
}

std::string    Session::username(const int& key) const {
    std::map<int, User>::const_iterator it = _cookies.find(key);
    
    if (it == _cookies.end())
        return "";
    return it->second.name;
}

void    Session::delete_session(const int& key) {
    _cookies.erase(key);
}