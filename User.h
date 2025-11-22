//
// Created by SergiyT on 21.11.2025.
//

#ifndef USER_H
#define USER_H
#include <string>


class User {
private:
    std::string username;
    std::string password;
    bool admin;

public:
    User();
    User(const std::string& username, const std::string& password, bool isAdmin);

    const std::string& getUsername() const;
    bool checkPassword(const std::string& inputPassword) const;
    bool isAdmin() const;

    void setPassword(const std::string& newPassword);

};



#endif //USER_H
