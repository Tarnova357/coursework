//
// Created by SergiyT on 01.11.2025.
//

#ifndef BUYTRANSACTION_H
#define BUYTRANSACTION_H
#include <string>

#include "Client.h"
#include "Transaction.h"


class BuyTransaction: public Transaction {
    public:
    BuyTransaction();
    BuyTransaction(const Client& client, const std::string& currencyCode, double amount);

    BuyTransaction(const BuyTransaction& other) = default;
    BuyTransaction(BuyTransaction&& other) noexcept = default;

    virtual ~BuyTransaction();

    std::string getOperationType() const override;
    void calculate(double buyRate, double sellRate) override;

};



#endif //BUYTRANSACTION_H
