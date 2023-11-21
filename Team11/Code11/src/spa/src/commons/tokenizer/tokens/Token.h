#pragma once

#include <string>

class Token {
protected:
    std::string stringValue;

public:
    explicit Token(std::string stringValue);
    std::string getStringValue();

    bool operator!=(const Token &other) const {
        return stringValue != other.stringValue;
    }
};
