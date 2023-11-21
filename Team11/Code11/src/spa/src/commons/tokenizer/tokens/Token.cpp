#include "Token.h"
#include <string>
#include <utility>

Token::Token(std::string stringValue) : stringValue(std::move(stringValue)) {
}

std::string Token::getStringValue() {
    return stringValue;
}
