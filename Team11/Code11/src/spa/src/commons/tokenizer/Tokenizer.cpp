#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "Tokenizer.h"

Tokenizer::Tokenizer(std::istream* stream) {
    this->stream = stream;
    this->joinableFirstTokens = {};
    this->joinedTokens = {};
}

Tokenizer::Tokenizer(std::istream* stream,
                     std::unordered_set<std::string> joinableFirstTokens,
                     std::unordered_set<std::string> joinedTokens) {
    this->stream = stream;
    this->joinableFirstTokens = std::move(joinableFirstTokens);
    this->joinedTokens = std::move(joinedTokens);
}

char Tokenizer::peekNextChar() {
    return static_cast<char>(stream->peek());
}

char Tokenizer::readNextChar() {
    return static_cast<char>(stream->get());
}

void Tokenizer::readUntilNotWhitespace() {
    while (!stream->eof() && isspace(peekNextChar())) {
        readNextChar();
    }
}

std::string Tokenizer::readToken() {
    std::string token;
    if (isalnum(peekNextChar())) {
        while (isalnum(peekNextChar())) {
            token += readNextChar();
        }
    } else {
        token = readNextChar();
    }

    return token;
}

void Tokenizer::addToken() {
    auto token1 = readToken();
    if (isspace(peekNextChar()) || peekNextChar() == EOF) {
        tokens.push_back(std::make_shared<Token>(token1));
        return;
    }

    if (joinableFirstTokens.find(token1) != joinableFirstTokens.end()) {
        auto token2 = readToken();
        if (joinedTokens.find(token1 + token2) != joinedTokens.end()) {
            tokens.emplace_back(std::make_shared<Token>(token1 + token2));
        } else {
            tokens.emplace_back(std::make_shared<Token>(token1));
            tokens.emplace_back(std::make_shared<Token>(token2));
        }
    } else {
        tokens.emplace_back(std::make_shared<Token>(token1));
    }
}

std::vector<std::shared_ptr<Token> > Tokenizer::tokenize() {
    readUntilNotWhitespace();
    while (!stream->eof()) {
        addToken();
        readUntilNotWhitespace();
    }

    return tokens;
}
