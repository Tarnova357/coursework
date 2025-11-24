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
        std::cerr << "Невірні курси валют для " << code << ".Встановлюєм на 0. Помилка: "
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
    std::cout << "Було викликано деструктор для Currency: " << code << std::endl;
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
        throw std::invalid_argument("Курси валют повинні бути більше 0.");
    }
    if (newBuy >= newSell) {
        throw std::invalid_argument("Курс купівлі має бути нижчим за курс продажу.");
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
void Currency::display() const {
    std::cout << std::fixed << std::setprecision(2)
              << "  [" << code << "] "
              << "Купівля: " << std::setw(7) << std::left << buyRate
              << "Продаж: " << std::setw(7) << std::left << sellRate
              << "Різниця: " << getSpread()
              << std::endl;
}
void Currency::serialize(std::ostream& os) const {
    os << code << "\t"
       << buyRate << "\t"
       << sellRate << "\n";
}

void Currency::deserialize(std::istream& is) {
    std::string line;
    if (std::getline(is, line) && !line.empty()) {
        std::stringstream ss(line);
        std::string codeStr;
        std::string buyRateStr;
        std::string sellRateStr;

        try {
            if (!std::getline(ss, codeStr, '\t')) {
                throw std::runtime_error("Не вдалося прочитати код валюти");
            }
            if (!std::getline(ss, buyRateStr, '\t')) {
                throw std::runtime_error("Не вдалося прочитати курс купівлі");
            }
            if (!std::getline(ss, sellRateStr)) {
                throw std::runtime_error("Не вдалося прочитати курс продажу");
            }

            double buy = std::stod(buyRateStr);
            double sell = std::stod(sellRateStr);

            code = codeStr;
            setRates(buy, sell);

        } catch (const std::exception& e) {
            std::cerr << "[Помилка] Збій читання валюти: " << e.what()
                      << " | Рядок: '" << line << "'" << std::endl;
            is.setstate(std::ios::failbit);
        }
    }
}

