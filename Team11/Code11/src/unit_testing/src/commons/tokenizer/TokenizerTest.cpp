#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "catch.hpp"
#include "commons/tokenizer/Tokenizer.h"
#include "query_processing_system/tokenizer/QueryTokenizer.h"

TEST_CASE("Normal Input") {
    std::istringstream input("Line 1\nLine 2\n");
    std::vector<std::shared_ptr<Token>> expectedTokens;

    expectedTokens.push_back(std::make_shared<Token>("Line"));
    expectedTokens.push_back(std::make_shared<Token>("1"));
    expectedTokens.push_back(std::make_shared<Token>("Line"));
    expectedTokens.push_back(std::make_shared<Token>("2"));

    Tokenizer tokenizer = Tokenizer(&input);
    std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();

    bool areEqual = expectedTokens.size() == tokens.size();

    for (size_t i = 0; i < expectedTokens.size() && areEqual; ++i) {
        if (expectedTokens[i]->getStringValue() != tokens[i]->getStringValue()) {
            areEqual = false;
        }        
    }

    REQUIRE(areEqual);
}

//TEST_CASE("Simple Source Code Input 2") {
//
//}
//
//TEST_CASE("Simple Source Code Input 3") {
//
//}

TEST_CASE("Query Tokenisation 1") {
    std::istringstream input("assign a1, a2; variable v;\n Select v such that Uses(a1, v)");
    std::vector<std::shared_ptr<Token>> expectedTokens;

    expectedTokens.push_back(std::make_shared<Token>("assign"));
    expectedTokens.push_back(std::make_shared<Token>("a1"));
    expectedTokens.push_back(std::make_shared<Token>(","));
    expectedTokens.push_back(std::make_shared<Token>("a2"));
    expectedTokens.push_back(std::make_shared<Token>(";"));
    expectedTokens.push_back(std::make_shared<Token>("variable"));
    expectedTokens.push_back(std::make_shared<Token>("v"));
    expectedTokens.push_back(std::make_shared<Token>(";"));
    expectedTokens.push_back(std::make_shared<Token>("Select"));
    expectedTokens.push_back(std::make_shared<Token>("v"));
    expectedTokens.push_back(std::make_shared<Token>("such"));
    expectedTokens.push_back(std::make_shared<Token>("that"));
    expectedTokens.push_back(std::make_shared<Token>("Uses"));
    expectedTokens.push_back(std::make_shared<Token>("("));
    expectedTokens.push_back(std::make_shared<Token>("a1"));
    expectedTokens.push_back(std::make_shared<Token>(","));
    expectedTokens.push_back(std::make_shared<Token>("v"));
    expectedTokens.push_back(std::make_shared<Token>(")"));

    QueryTokenizer tokenizer(&input);
    std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();

    bool areEqual = expectedTokens.size() == tokens.size();

    for (size_t i = 0; i < expectedTokens.size() && areEqual; ++i) {
        if (*expectedTokens[i] != *tokens[i]) {
            areEqual = false;
        }
    }

    REQUIRE(areEqual);
}

TEST_CASE("Query Tokenisation 2") {
    std::istringstream input("assign a1, a2; \n Select a1 such that Follows*(a1, a2)");
    std::vector<std::shared_ptr<Token>> expectedTokens;

    expectedTokens.push_back(std::make_shared<Token>("assign"));
    expectedTokens.push_back(std::make_shared<Token>("a1"));
    expectedTokens.push_back(std::make_shared<Token>(","));
    expectedTokens.push_back(std::make_shared<Token>("a2"));
    expectedTokens.push_back(std::make_shared<Token>(";"));
    expectedTokens.push_back(std::make_shared<Token>("Select"));
    expectedTokens.push_back(std::make_shared<Token>("a1"));
    expectedTokens.push_back(std::make_shared<Token>("such"));
    expectedTokens.push_back(std::make_shared<Token>("that"));
    expectedTokens.push_back(std::make_shared<Token>("Follows*"));
    expectedTokens.push_back(std::make_shared<Token>("("));
    expectedTokens.push_back(std::make_shared<Token>("a1"));
    expectedTokens.push_back(std::make_shared<Token>(","));
    expectedTokens.push_back(std::make_shared<Token>("a2"));
    expectedTokens.push_back(std::make_shared<Token>(")"));

    QueryTokenizer tokenizer(&input);
    std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();

    bool areEqual = expectedTokens.size() == tokens.size();

    for (size_t i = 0; i < expectedTokens.size() && areEqual; ++i) {
        if (*expectedTokens[i] != *tokens[i]) {
            areEqual = false;
        }
    }

    REQUIRE(areEqual);
}


TEST_CASE("Query Tokenisation 3") {
    std::istringstream input("assign a1;\nSelect a1 such that Follows(a1, _)");
    std::vector<std::shared_ptr<Token>> expectedTokens;

    expectedTokens.push_back(std::make_shared<Token>("assign"));
    expectedTokens.push_back(std::make_shared<Token>("a1"));
    expectedTokens.push_back(std::make_shared <Token>(";"));
    expectedTokens.push_back(std::make_shared<Token>("Select"));
    expectedTokens.push_back(std::make_shared<Token>("a1"));
    expectedTokens.push_back(std::make_shared<Token>("such"));
    expectedTokens.push_back(std::make_shared<Token>("that"));
    expectedTokens.push_back(std::make_shared<Token>("Follows"));
    expectedTokens.push_back(std::make_shared <Token>("("));
    expectedTokens.push_back(std::make_shared<Token>("a1"));
    expectedTokens.push_back(std::make_shared <Token>(","));
    expectedTokens.push_back(std::make_shared <Token>("_"));
    expectedTokens.push_back(std::make_shared <Token>(")"));

    QueryTokenizer tokenizer(&input);
    std::vector<std::shared_ptr<Token>> tokens = tokenizer.tokenize();

    bool areEqual = expectedTokens.size() == tokens.size();

    for (size_t i = 0; i < expectedTokens.size() && areEqual; ++i) {
        if (*expectedTokens[i] != *tokens[i]) {
            areEqual = false;
        }
    }

    REQUIRE(areEqual);
}
