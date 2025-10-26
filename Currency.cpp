//
// Created by SergiyT on 25.10.2025.
//

#include "Currency.h"
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>

Currency::Currency() : code("N/A"), buyRate(0.0), sellRate(0.0) {}

Currency::Currency(const std::string &code, double buyRate, double sellRate)
    : code(code), buyRate(0.0), sellRate(0.0) {
    try {
        setRates(buyRate, sellRate);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Warning: Invalid rates for " << code << ". Setting to 0. Error: "
            << e.what() << std::endl;
        buyRate = 0.0;
        sellRate = 0.0;
    }
}

Currency::Currency(const Currency& other)
    : code(other.code), buyRate(other.buyRate), sellRate(other.sellRate) {
}

Currency::Currency(Currency&& other) noexcept
    : code(std::move(other.code)), buyRate(other.buyRate), sellRate(other.sellRate) {
    other.buyRate = 0.0;
    other.sellRate = 0.0;
}

Currency::~Currency() {
    std::cout << "Currency [Destructor] called for: " << code << std::endl;
}

Currency& Currency::operator=(const Currency& other) {
    if (this == &other) {
        return *this;
    }
    code = other.code;
    buyRate = other.buyRate;
    sellRate = other.sellRate;
    return *this;
}

Currency& Currency::operator=(Currency&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    code = std::move(other.code);
    buyRate = other.buyRate;
    sellRate = other.sellRate;

    other.buyRate = 0.0;
    other.sellRate = 0.0;
    return *this;
}

const std::string& Currency::getCode() const {
    return code;
}

double Currency::getBuyRate() const {
    return buyRate;
}

double Currency::getSellRate() const {
    return sellRate;
}

void Currency::setRates(double newBuy, double newSell) {
    if (newBuy <= 0 || newSell <= 0) {
        throw std::invalid_argument("Rates must be positive numbers.");
    }
    if (newBuy >= newSell) {
        throw std::invalid_argument("Buy rate must be lower than sell rate.");
    }

    buyRate = newBuy;
    sellRate = newSell;
}

bool Currency::isBaseCurrency() const {
    return code == "UAH";
}

double Currency::getSpread() const {
    return sellRate - buyRate;
}

std::ostream& operator<<(std::ostream& os, const Currency& currency) {
    os << currency.code << "," << currency.buyRate << "," << currency.sellRate;
    return os;
}

std::istream& operator>>(std::istream& is, Currency& currency) {
    std::string line;
    if (std::getline(is, line) && !line.empty()) {
        std::stringstream ss(line);
        std::string codeStr;
        std::string buyRateStr;
        std::string sellRateStr;

        try {
            if (!std::getline(ss, codeStr, ',')) {
                throw std::runtime_error("Failed to parse code");
            }
            if (!std::getline(ss, buyRateStr, ',')) {
                throw std::runtime_error("Failed to parse buy rate");
            }
            if (!std::getline(ss, sellRateStr)) {
                throw std::runtime_error("Failed to parse sell rate");
            }

            double buy = std::stod(buyRateStr);
            double sell = std::stod(sellRateStr);

            currency.code = codeStr;
            currency.setRates(buy, sell);

        } catch (const std::exception& e) {
            std::cerr << "Error parsing currency line: " << e.what()
                      << " | Line: '" << line << "'" << std::endl;
            is.setstate(std::ios::failbit);
        }
    }
    return is;
}

