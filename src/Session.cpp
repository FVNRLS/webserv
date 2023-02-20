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

/*
 * Creates a new session for the user with the provided username and password.
 * Is responsible for authenticating users and creating new sessions for them.
 *
 * It takes two string arguments, username and password, and returns an integer value.
 * First, it checks whether the username is valid and retrieves the corresponding session key.
 *
 * If the username already exists, it checks whether the provided password is correct.
 * If the password is correct, it returns the existing session key.
 * If the password is incorrect, it returns false.
 *
 * If the username does not exist, it generates a new session key and creates a new user object with the provided username and password.
 * It then adds the new user object to the _cookies map with the session key as the key and returns the new session key.
 * */
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

/*
 * Checks if a cookie with the given key exists in the session by searching for the key in the _cookies map.*
 * Returns true if the key exists, and false otherwise.
 * */
bool    Session::exists(const int& key) const {
    if (_cookies.find(key) == _cookies.end())
        return false;
    return true;
}

/*
 * Searches for the key associated with a given username.
 * It iterates through the _cookies map and returns the first key that has the matching username.
 * If no matching username is found, it returns false.
 * */
int    Session::get_key(const std::string& username) const {
    for (std::map<int, User>::const_iterator it = _cookies.begin(); it != _cookies.end(); it++) {
        if (it->second.name == username)
            return it->first;
    }
    return false;
}

/*
 * Checks whether the provided password matches the password of the user associated with the given key in the _cookies map.
 * Returns true if the password is correct and false otherwise.
 * */
bool    Session::password_correct(int key, const std::string& password) const {
    std::map<int, User>::const_iterator it = _cookies.find(key);

    if (it != _cookies.end() && it->second.pass == password)
        return true;
    return false;
}

/*
 * Generates a random integer value to be used as a key for a new session.
 *
 * It first seeds the random number generator with the current time, then generates a random integer value
 * and checks that it is not equal to false (which represents an invalid key value).
 * It also checks that the generated key is not already in use by an existing session.
 * If the generated key is invalid or already in use, it generates another random integer until a valid, unused key is found.
 * Once a valid key is generated, it is returned by the function.
 * */
int     Session::generate_key() {
    srand(time(0));

    int key = rand();
    while (key == false || _cookies.find(key) != _cookies.end())
        key = rand();
    return key;
}

/*
 * Returns the username associated with the given session key.
 * It first looks up the key in the _cookies map and returns an empty string if the key is not found.
 * If the key is found, it returns the associated User object's name attribute, which contains the username.
 * */
std::string    Session::username(const int& key) const {
    std::map<int, User>::const_iterator it = _cookies.find(key);
    
    if (it == _cookies.end())
        return "";
    return it->second.name;
}

/*
 * Deletes a cookie session from the _cookies map
 * */
void    Session::delete_session(const int& key) {
    _cookies.erase(key);
}