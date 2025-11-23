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

void Transaction::setId(int ID) {
    id = ID;
}

int Transaction::getId() const {
    return id;
}

const Client& Transaction::getClient() const {
    return clientData;
}

void Transaction::setClient(const Client& client) {
    clientData = client;
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
    char tBuf[26]; ctime_s(tBuf, sizeof(tBuf), &timestamp); tBuf[24]='\0';
    std::cout << " ID:" << id << " | " << getOperationType() << " | " << tBuf << "\n";
    clientData.display();
    std::cout << "  " << amountCurrency << " " << currencyCode << " -> " << amountBase << " UAH\n";
    std::cout << "------------------------------------------\n";
}

void Transaction::serialize(std::ostream& os) const {
    os << id << "\t" <<getOperationType() << "\t" << timestamp << "\t";
    clientData.serialize(os);
    os << "\t" << currencyCode << "\t" << amountCurrency << "\t" << amountBase << "\n";
}
void Transaction::deserialize(std::istream& is) {
    std::string tStr;
    if(std::getline(is, tStr, '\t')) timestamp = std::stol(tStr);
    clientData.deserialize(is);
    std::getline(is, currencyCode, '\t');
    std::getline(is, tStr, '\t'); amountCurrency = std::stod(tStr);
    std::getline(is, tStr); amountBase = std::stod(tStr);
}


