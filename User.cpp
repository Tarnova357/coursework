//
// Created by SergiyT on 21.11.2025.
//

#include "User.h"

#include <stdexcept>

User::User() : username(""), password(""), admin(false) {}

User::User(const std::string& Username, const std::string& Password, bool isAdmin) {
    if (username.empty()) {
        throw std::invalid_argument("Username cannot be empty.");
    }
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty.");
    }
    if (username.find(':') != std::string::npos || password.find(':') != std::string::npos) {
        throw std::invalid_argument("Username and password cannot contain ':' character.");
    }
    username=Username;
    password=Password;
    admin=isAdmin;
}

const std::string& User::getUsername() const {
    return username;
}

bool User::checkPassword(const std::string& inputPassword) const {
    return password == inputPassword;
}

bool User::isAdmin() const {
    return admin;
}

void User::setPassword(const std::string& newPassword) {
    if (newPassword.empty()) {
        throw std::invalid_argument("New password cannot be empty.");
    }
    if (newPassword.find(':') != std::string::npos) {
        throw std::invalid_argument("Password cannot contain ':' character.");
    }
    password = newPassword;
}