#pragma once

#include <exception>
#include <string>
#include <utility>

class PKBException : public std::exception {
private:
    std::string message;
public:
    explicit PKBException(std::string message) : message{std::move(message)}
    {}
    std::string what() {
        return message;
    }
};
