#ifndef SPA_PARSERFACTORY_H
#define SPA_PARSERFACTORY_H

#pragma once

#include <map>
#include <memory>
#include <string>

#include "AbstractParser.h"
#include "commons/tokenizer/tokens/Token.h"

enum class ParserType {
    ASSIGN,
    PROC,
    READ,
    PRINT,
    CALL,
    IF,
    WHILE,
};

class ParserFactory {
public:
    static std::shared_ptr<AbstractParser> getParser(const std::string& val);

private:
    static inline std::map<std::string, ParserType> parserTypeMap = {
        {"procedure", ParserType::PROC},
        {"read", ParserType::READ},
        {"print", ParserType::PRINT},
        {"call", ParserType::CALL},
        {"while", ParserType::WHILE},
        {"if", ParserType::IF},
    };
};


#endif //SPA_PARSERFACTORY_H
