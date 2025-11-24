//
// Created by SergiyT on 21.11.2025.
//

#include "User.h"

#include <stdexcept>

User::User() : username(""), password(""), admin(false) {}

User::User(const std::string& t_username, const std::string& t_password, bool isAdmin) {
    if (t_username.empty()) {
        throw std::invalid_argument("Ім'я користувача не може бути порожнім.");
    }
    if (t_password.empty()) {
        throw std::invalid_argument("Пароль не може бути порожнім.");
    }
    if (t_username.find(':') != std::string::npos || t_password.find(':') != std::string::npos) {
        throw std::invalid_argument("Ім'я та пароль не можуть містити символ ':'.");
    }
    username=t_username;
    password=t_password;
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