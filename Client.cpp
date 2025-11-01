//
// Created by SergiyT on 26.10.2025.
//

#include "Client.h"

#include <iomanip>
#include <iostream>


Client::Client()
    : fullName("N/A"), passportSeries("XX"), passportNumber("000000") {

}

Client::Client(const std::string& Name, const std::string& series, const std::string& number) {
    try {
        setFullName(Name);
        setPassport(series, number);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Warning: Invalid client data. " << e.what() << std::endl;

        fullName = "N/A";
        passportSeries = "XX";
        passportNumber = "000000";
    }
}

Client::Client(const Client& other)
    : fullName(other.fullName),
      passportSeries(other.passportSeries),
      passportNumber(other.passportNumber) {
}

Client::Client(Client&& other) noexcept
    : fullName(std::move(other.fullName)),
      passportSeries(std::move(other.passportSeries)),
      passportNumber(std::move(other.passportNumber)) {

    other.fullName = "N/A";
    other.passportSeries = "XX";
    other.passportNumber = "000000";
}

Client::~Client() {
    std::cout << "Client [Destructor] called for: " << fullName << std::endl;
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        fullName = other.fullName;
        passportSeries = other.passportSeries;
        passportNumber = other.passportNumber;
    }
    return *this;
}

Client& Client::operator=(Client&& other) noexcept {
    if (this != &other) {
        fullName = std::move(other.fullName);
        passportSeries = std::move(other.passportSeries);
        passportNumber = std::move(other.passportNumber);

        other.fullName = "N/A";
        other.passportSeries = "XX";
        other.passportNumber = "000000";
    }
    return *this;
}

const std::string& Client::getFullName() const {
    return fullName;
}

const std::string& Client::getPassportSeries() const {
    return passportSeries;
}

const std::string& Client::getPassportNumber() const {
    return passportNumber;
}

void Client::setFullName(const std::string& Name) {
    if (Name.empty()) {
        throw std::invalid_argument("Full name cannot be empty.");
    }
    bool hasLetter = false;
    for (char c : Name) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            hasLetter = true;
            break;
        }
    }
    if (!hasLetter) {
        throw std::invalid_argument("Full name must contain letters.");
    }
    fullName = Name;
}

void Client::setPassportSeries(const std::string& series) {
    validatePassportSeries(series);
    passportSeries = series;
}

void Client::setPassportNumber(const std::string& number) {
    validatePassportNumber(number);
    passportNumber = number;
}

void Client::validatePassportSeries(const std::string& series) {
    for (char c : series) {
        if (!std::isupper(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("Passport series must be uppercase letters.");
        }
    }
}

void Client::validatePassportNumber(const std::string& number) {
    for (char c : number) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("Passport number must contain only digits.");
        }
    }
}


void Client::setPassport(const std::string& series, const std::string& number) {
    validatePassportSeries(series);
    validatePassportNumber(number);

    setPassportSeries(series);
    setPassportNumber(number);
}

std::string Client::getPassportInfo() const {
    return passportSeries + " " + passportNumber;
}

bool Client::isValid() const {
    return !fullName.empty() && fullName != "N/A" &&
           !passportSeries.empty() && passportSeries != "XX" &&
           !passportNumber.empty() && passportNumber != "000000";
}

void Client::display() const {
    std::cout << "  Client: " << std::setw(30) << std::left << fullName
              << " | Passport: " << getPassportInfo() << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Client& client) {

    os << client.fullName << "\t"
       << client.passportSeries << "\t"
       << client.passportNumber;
    return os;
}