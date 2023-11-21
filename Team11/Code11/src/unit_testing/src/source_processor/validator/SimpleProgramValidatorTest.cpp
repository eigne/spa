#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "catch.hpp"
#include "../src/source_processor/validator/SimpleProgramValidator.h"

TEST_CASE("Valid cases for Name") {
    SimpleProgramValidator simpleProgramValidator;

    REQUIRE(simpleProgramValidator.validateName("play"));
    REQUIRE(simpleProgramValidator.validateName("Player"));
    REQUIRE(simpleProgramValidator.validateName("Player123"));
    REQUIRE(simpleProgramValidator.validateName("procedure"));
    REQUIRE(simpleProgramValidator.validateName("read"));
}

TEST_CASE("Valid cases for Integer Token") {
    SimpleProgramValidator simpleProgramValidator;

    REQUIRE(simpleProgramValidator.validateInteger("0"));
    REQUIRE(simpleProgramValidator.validateInteger("123"));
    REQUIRE(simpleProgramValidator.validateInteger("1234567890987654321"));
}

TEST_CASE("Valid cases for Brackets") {
    std::vector<std::string> bracketArray;
    bracketArray.push_back("(");
    bracketArray.push_back("{");
    bracketArray.push_back("}");
    bracketArray.push_back(")");

    SimpleProgramValidator simpleProgramValidator;
    REQUIRE(simpleProgramValidator.validateBrackets(bracketArray));
}

TEST_CASE("Valid cases for Special Characters") {
    SimpleProgramValidator simpleProgramValidator;

    REQUIRE(simpleProgramValidator.validateSpecialCharacter("!"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("="));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("+"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("-"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("*"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("/"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("%"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("{"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("}"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("("));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter(")"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter(";"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("&"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("|"));

    REQUIRE(simpleProgramValidator.validateSpecialCharacter("||"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("&&"));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("!="));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter(">="));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("<="));
    REQUIRE(simpleProgramValidator.validateSpecialCharacter("=="));
}

TEST_CASE("Invalid cases for Name Token") {
    SimpleProgramValidator simpleProgramValidator;

    REQUIRE(!simpleProgramValidator.validateName("123"));
    REQUIRE(!simpleProgramValidator.validateName("8Test"));
    REQUIRE(!simpleProgramValidator.validateName("0loser"));
}

TEST_CASE("Invalid cases for Integer Token") {
    SimpleProgramValidator simpleProgramValidator;

    REQUIRE(!simpleProgramValidator.validateInteger("09W"));
    REQUIRE(!simpleProgramValidator.validateInteger("0123"));
}

TEST_CASE("Invalid case for Mismatched Brackets") {
    std::vector<std::string> bracketArray;
    bracketArray.push_back("(");
    bracketArray.push_back("}");

    SimpleProgramValidator simpleProgramValidator;
    REQUIRE(!simpleProgramValidator.validateBrackets(bracketArray));
}

TEST_CASE("Invalid Case for Repeated Brackets") {
    std::vector<std::string> bracketArray;
    bracketArray.push_back("(");
    bracketArray.push_back("(");

    SimpleProgramValidator simpleProgramValidator;
    REQUIRE(!simpleProgramValidator.validateBrackets(bracketArray));
}

TEST_CASE("Invalid cases for Special Characters") {
    SECTION("Character does not exist") {
        SimpleProgramValidator simpleProgramValidator;

        REQUIRE(!simpleProgramValidator.validateSpecialCharacter(","));
        REQUIRE(!simpleProgramValidator.validateSpecialCharacter("_"));
        REQUIRE(!simpleProgramValidator.validateSpecialCharacter("@"));
        REQUIRE(!simpleProgramValidator.validateSpecialCharacter("#"));
        REQUIRE(!simpleProgramValidator.validateSpecialCharacter("$"));
    };

    SECTION("Invalid Combinations of Special Character") {
        SimpleProgramValidator simpleProgramValidator;

        REQUIRE(!simpleProgramValidator.validateSpecialCharacter("!-"));
        REQUIRE(!simpleProgramValidator.validateSpecialCharacter("|="));
        REQUIRE(!simpleProgramValidator.validateSpecialCharacter("+="));
        REQUIRE(!simpleProgramValidator.validateSpecialCharacter("!!"));
    };
}
