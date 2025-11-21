//
// Created by SergiyT on 21.11.2025.
//

#include "SellTransaction.h"

SellTransaction::SellTransaction(): Transaction() {}

SellTransaction::SellTransaction(const Client &client, const std::string &currencyCode, double amount): Transaction(client, currencyCode, amount){}

SellTransaction::~SellTransaction() {
    std::cout << "SellTransaction [Destructor] called."<<std::endl;
}

std::string SellTransaction::getOperationType() const {
    return "SELL";
}

void SellTransaction::calculate(double buyRate, double sellRate) {
    amountBase = amountCurrency * sellRate;
}


