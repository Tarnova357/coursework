//
// Created by SergiyT on 25.10.2025.
//

#ifndef CURRENCY_H
#define CURRENCY_H
#include <string>

#include "ISerializable.h"


class Currency: ISerializable {
private:
    std::string code;
    double buyRate;
    double sellRate;

    public:
    Currency();
    Currency(const std::string &code, double buyRate, double sellRate);
    Currency(const Currency& other);
    Currency(Currency&& other) noexcept;
    ~Currency() override;
    Currency& operator=(const Currency& other);
    Currency& operator=(Currency&& other) noexcept;
    const std::string& getCode() const;
    double getBuyRate() const;
    double getSellRate() const;
    void setRates(double buyRate, double sellRate);
    double getSpread() const;
    bool isBaseCurrency() const;
    void display() const;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
};



#endif //CURRENCY_H
