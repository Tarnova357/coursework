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
        std::cerr << "Попередження: невірні данні клієнта. " << e.what() << std::endl;

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
    std::cout << "Було викликано деструктор для Client: " << fullName << std::endl;
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
        throw std::invalid_argument("Ім'я клієнта не може бути порожнім.");
    }
    for (char c : Name) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("Цифри в імені заборонені.");
        }
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
    if (series.empty()) {
        throw std::invalid_argument("Серія паспорту не може бути порожньою.");
    }
    if (series.length() != 2) throw std::invalid_argument("Серія має бути 2 символи.");
}

void Client::validatePassportNumber(const std::string& number) {
    for (char c : number) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("Номер паспорту повинен містити лише цифри.");
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
    std::cout << "  Клієнт: " << std::setw(30) << std::left << fullName
              << " | Паспорт: " << getPassportInfo() << std::endl;
}

void Client::serialize(std::ostream& os) const {
    os << fullName << "\t"
       << passportSeries << "\t"
       << passportNumber;
}

void Client::deserialize(std::istream& is) {
    std::string fullName, series, number;

    if (!std::getline(is, fullName, '\t')) {
        throw std::runtime_error("Не вдалося прочитати ім'я клієнта");
    }
    if (!std::getline(is, series, '\t')) {
        throw std::runtime_error("Не вдалося прочитати серію паспорта");
    }
    if (!std::getline(is, number, '\t')) {
        throw std::runtime_error("Не вдалося прочитати номер паспорта");
    }

    setFullName(fullName);
    setPassport(series, number);
}