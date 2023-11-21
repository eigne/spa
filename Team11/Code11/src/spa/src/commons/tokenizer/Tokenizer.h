#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include "commons/tokenizer/tokens/Token.h"

class Tokenizer {
protected:
    std::unordered_set<std::string> joinableFirstTokens;
    std::unordered_set<std::string> joinedTokens;

    std::istream* stream;
    std::vector<std::shared_ptr<Token> > tokens;
    // This is a storage string to add chars to
    // it for evaluation before creating a Token Object
    std::string currentString;
    char peekNextChar();
    char readNextChar();
    void readUntilNotWhitespace();
    std::string readToken();
    void addToken();

public:
    explicit Tokenizer(std::istream* stream);
    Tokenizer(std::istream* stream,
              std::unordered_set<std::string> joinableFirstTokens,
              std::unordered_set<std::string> joinedTokens);
    virtual std::vector<std::shared_ptr<Token> > tokenize();
};
