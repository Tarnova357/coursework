//
// Created by SergiyT on 01.11.2025.
//

#include "Transaction.h"
#include <ctime>
#include <iomanip>

Transaction::Transaction()
    : clientData(),
      currencyCode("N/A"),
      amountCurrency(0.0),
      amountBase(0.0),
      timestamp(0) {}

Transaction::Transaction(const Client& client, const std::string& currencyCode, double amount)
    : clientData(client),
      currencyCode(currencyCode),
      amountBase(0.0),
      timestamp(std::time(nullptr)) {
    try {
        setAmountCurrency(amount);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Warning: Invalid amount for transaction. " << e.what() << std::endl;
        amountCurrency = 0.0;
    }
}

Transaction::Transaction(const Transaction& other)
    : clientData(other.clientData),
      currencyCode(other.currencyCode),
      amountCurrency(other.amountCurrency),
      amountBase(other.amountBase),
      timestamp(other.timestamp) {
}

Transaction::Transaction(Transaction&& other) noexcept
    : clientData(std::move(other.clientData)),
      currencyCode(std::move(other.currencyCode)),
      amountCurrency(other.amountCurrency),
      amountBase(other.amountBase),
      timestamp(other.timestamp) {
    other.amountCurrency = 0.0;
    other.amountBase = 0.0;
    other.timestamp = 0;
}

Transaction::~Transaction() {
    std::cout << "Transaction [Destructor] called for: " << currencyCode << std::endl;
}

Transaction& Transaction::operator=(const Transaction& other) {
    if (this != &other) {
        clientData = other.clientData;
        currencyCode = other.currencyCode;
        amountCurrency = other.amountCurrency;
        amountBase = other.amountBase;
        timestamp = other.timestamp;
    }
    return *this;
}

Transaction& Transaction::operator=(Transaction&& other) noexcept {
    if (this != &other) {
        clientData = std::move(other.clientData);
        currencyCode = std::move(other.currencyCode);
        amountCurrency = other.amountCurrency;
        amountBase = other.amountBase;
        timestamp = other.timestamp;

        other.amountCurrency = 0.0;
        other.amountBase = 0.0;
        other.timestamp = 0;
    }
    return *this;
}

const Client& Transaction::getClient() const {
    return clientData;
}

const std::string& Transaction::getCurrencyCode() const {
    return currencyCode;
}

double Transaction::getAmountCurrency() const {
    return amountCurrency;
}

double Transaction::getAmountBase() const {
    return amountBase;
}

time_t Transaction::getTimestamp() const {
    return timestamp;
}

void Transaction::setAmountCurrency(double amount) {
    if (amount <= 0.0) {
        throw std::invalid_argument("Transaction amount must be positive.");
    }

    amountCurrency = amount;
}

void Transaction::display() const {
    char timeBuf[26];
    ctime_s(timeBuf, sizeof(timeBuf), &timestamp);
    timeBuf[24] = '\0';

    std::cout << "------------------------------------------\n"
              << " Transaction: " << std::setw(5) << std::left << getOperationType()
              << " | " << timeBuf << "\n";

    clientData.display();

    std::cout << "  Currency: " << std::setw(3) << currencyCode
              << std::fixed << std::setprecision(2)
              << " | Amount (Curr): " << std::setw(10) << amountCurrency
              << " | Amount (Base): " << amountBase
              << "\n------------------------------------------" << std::endl;
}

void Transaction::serialize(std::ostream& os) const {
    os << getOperationType() << "\t"
       << timestamp << "\t";

    clientData.serialize(os);

    os << "\t" << currencyCode << "\t"
       << amountCurrency << "\t"
       << amountBase << "\n";
}
void Transaction::deserialize(std::istream& is) {
    std::string line;
    std::getline(is, line);
    if (line.empty()) return;

    std::stringstream ss(line);

    std::string temp;

    try {
        std::getline(ss, temp, '\t');
        timestamp =static_cast<time_t>(std::stol(temp));

        clientData.deserialize(ss);

        std::getline(ss, currencyCode, '\t');

        std::getline(ss, temp, '\t');
        amountCurrency = std::stod(temp);

        std::getline(ss, temp);
        amountBase = std::stod(temp);

    } catch (const std::exception& e) {
        std::cerr << "Error deserializing transaction: " << e.what() << std::endl;
        is.setstate(std::ios::failbit);
    }
}
