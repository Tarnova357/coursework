//
// Created by SergiyT on 21.11.2025.
//

#ifndef SELLTRANSACTION_H
#define SELLTRANSACTION_H
#include "Transaction.h"


class SellTransaction: public Transaction{
    SellTransaction();
    SellTransaction(const Client& client, const std::string& currencyCode, double amount);

    SellTransaction(const SellTransaction& other) = default;
    SellTransaction(SellTransaction&& other) noexcept = default;

    virtual ~SellTransaction();

    std::string getOperationType() const override;
    void calculate(double buyRate, double sellRate) override;
};



#endif //SELLTRANSACTION_H
