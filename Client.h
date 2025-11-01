//
// Created by SergiyT on 26.10.2025.
//

#ifndef CLIENT_H
#define CLIENT_H
#include <string>

#include "ISerializable.h"


class Client: ISerializable {
    private:
    std::string fullName;
    std::string passportSeries;
    std::string passportNumber;

    public:
    Client();
    Client(const std::string &fullName, const std::string &passportSeries,
        const std::string &passportNumber);

    Client(const Client &other);
    Client (Client &&other) noexcept;

    ~Client();

    Client &operator=(const Client &other);
    Client &operator=(Client &&other) noexcept;

    const std::string& getFullName() const;
    const std::string& getPassportSeries() const;
    const std::string& getPassportNumber() const;

    void setFullName(const std::string& fullName);
    void setPassportSeries(const std::string& series);
    void setPassportNumber(const std::string& number);

    void validatePassportSeries(const std::string& series);
    void validatePassportNumber(const std::string& number);
    void setPassport(const std::string& series, const std::string& number);
    std::string getPassportInfo() const;
    bool isValid() const;
    void display() const;

    void serialize(std::ostream& os) const;
    void deserialize(std::istream& is);




};



#endif //CLIENT_H
