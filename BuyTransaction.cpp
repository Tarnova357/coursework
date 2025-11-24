//
// Created by SergiyT on 01.11.2025.
//

#include "BuyTransaction.h"
BuyTransaction::BuyTransaction() : Transaction() {}

BuyTransaction::BuyTransaction(const Client& client, const std::string& currencyCode, double amount)
    : Transaction(client, currencyCode, amount) {}

BuyTransaction::~BuyTransaction() {
    std::cout << "Був викликаний деструктор BuyTransaction." << std::endl;
}

std::string BuyTransaction::getOperationType() const {
    return "КУПІВЛЯ";
}

void BuyTransaction::calculate(double buyRate, double sellRate) {
    (void)sellRate;
    amountBase = amountCurrency * buyRate;
}