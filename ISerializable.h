//
// Created by SergiyT on 01.11.2025.
//

#ifndef ISERIALIZE_H
#define ISERIALIZE_H
#include <iostream>


class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual void serialize(std::ostream& os) const = 0;
    virtual void deserialize(std::istream& is) = 0;
};



#endif //ISERIALIZE_H
