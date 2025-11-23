//
// Created by SergiyT on 21.11.2025.
//

#include "User.h"

#include <stdexcept>

User::User() : username(""), password(""), admin(false) {}

User::User(const std::string& Username, const std::string& Password, bool isAdmin) {
    if (username.empty()) {
        throw std::invalid_argument("Ім'я користувача не може бути порожнім.");
    }
    if (password.empty()) {
        throw std::invalid_argument("Пароль не може бути порожнім.");
    }
    if (username.find(':') != std::string::npos || password.find(':') != std::string::npos) {
        throw std::invalid_argument("Ім'я та пароль не можуть містити символ ':'.");
    }
    username=Username;
    password=Password;
    admin=isAdmin;
}

const std::string& User::getUsername() const {
    return username;
}

const std::string& User::getPassword() const {
    return password;
}

bool User::checkPassword(const std::string& inputPassword) const {
    return password == inputPassword;
}

bool User::isAdmin() const {
    return admin;
}

void User::setPassword(const std::string& newPassword) {
    if (newPassword.empty()) {
        throw std::invalid_argument("Новий пароль не може бути порожнім.");
    }
    if (newPassword.find(':') != std::string::npos) {
        throw std::invalid_argument("Пароль не може містити символ ':'.");
    }
    password = newPassword;
}