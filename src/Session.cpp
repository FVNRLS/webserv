/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 18:13:52 by doreshev          #+#    #+#             */
/*   Updated: 2023/02/07 19:57:08 by doreshev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"

//BASIC CLASS SETUP
Session::Session() { }

Session::~Session() {}


//MEMBER FUNCTIONS
int Session::create(const std::string& username, const std::string& password) {
    if (user_exists(username))
        return EXIT_FAILURE;
    int key = generate_key();
    _cookies[key] = User(username, password);
    return  key;
}

int     Session::generate_key() {
    srand(time(0));

    int key = rand();
    while (key == 1 || _cookies.find(key) != _cookies.end())
        key = rand();
    return key;
}

bool    Session::user_exists(const std::string& username) {
    for (std::map<int, User>::iterator it = _cookies.begin(); it != _cookies.end(); it++) {
        if (it->second.name == username)
            return true;
    }
    return false;
}

int    Session::logout(const int& key) {
    std::map<int, User>::iterator it = _cookies.find(key);
    
    if (it == _cookies.end())
        return EXIT_FAILURE;
    it->second.logged = false;
    return EXIT_SUCCESS;
}

std::string    Session::last_access(const int& key) const {
    std::map<int, User>::const_iterator it = _cookies.find(key);
    
    if (it == _cookies.end())
        return "";
    return it->second.last_access;
}

std::string    Session::username(const int& key) const {
    std::map<int, User>::const_iterator it = _cookies.find(key);
    
    if (it == _cookies.end())
        return "";
    return it->second.name;
}

bool    Session::logged(const int& key) const {
    std::map<int, User>::const_iterator it = _cookies.find(key);
    
    if (it == _cookies.end())
        return false;
    return it->second.logged;
}

void    Session::delete_session(const int& key) {
    _cookies.erase(key);
}
