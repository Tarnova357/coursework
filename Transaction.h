//
// Created by SergiyT on 01.11.2025.
//

#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "Client.h"
#include "ISerializable.h"


class Transaction: ISerializable {
protected:
    int id;
    Client clientData;
    std::string currencyCode;
    double amountCurrency;
    double amountBase;
    time_t timestamp;

public:
    Transaction();
    Transaction(const Client& client, const std::string& currencyCode, double amount);

    Transaction(const Transaction& other);
    Transaction(Transaction&& other) noexcept;

    virtual ~Transaction();

    Transaction& operator=(const Transaction& other);
    Transaction& operator=(Transaction&& other) noexcept;

    virtual std::string getOperationType() const = 0;
    virtual void calculate(double buyRate, double sellRate) = 0;

    void setId(int id);
    int getId() const;
    void setClient(const Client& client);

    const Client& getClient() const;
    const std::string& getCurrencyCode() const;
    double getAmountCurrency() const;
    double getAmountBase() const;
    time_t getTimestamp() const;

    void setAmountCurrency(double amount);

    virtual void display() const;

    void serialize(std::ostream& os) const override;

    void deserialize(std::istream& is) override;

};



#endif //TRANSACTION_H
