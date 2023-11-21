#pragma once

#include <stdexcept>

class DesignExtractorException : public std::runtime_error {
public:
    explicit DesignExtractorException(const std::string& message) : std::runtime_error(message) {
    }
};
