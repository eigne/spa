#include "commons/validator/SyntaxValidator.h"
#include "catch.hpp"

TEST_CASE("Empty expression validation") {
    REQUIRE_FALSE(SyntaxValidator{}.validateExpression({}));
}

TEST_CASE("Empty brackets validation") {
    REQUIRE_FALSE(SyntaxValidator{}.validateExpression({"(", ")"}));
}

TEST_CASE("Single integer validation") {
    REQUIRE(SyntaxValidator{}.validateExpression({"1"}));
}

TEST_CASE("Single name validation") {
    REQUIRE(SyntaxValidator{}.validateExpression({"hello123"}));
}

TEST_CASE("Single factor with brackets validation") {
    REQUIRE(SyntaxValidator{}.validateExpression({"(", "1", ")"}));
}

TEST_CASE("Single factor with a lot of brackets validation") {
    REQUIRE(SyntaxValidator{}.validateExpression({"(", "(", "(", "1", ")", ")", ")"}));
}

TEST_CASE("Single factor with balanced but wrong brackets validation") {
    REQUIRE_FALSE(SyntaxValidator{}.validateExpression({"(", "1", "(", ")" ")"}));
}

TEST_CASE("Single operator validation") {
    REQUIRE(SyntaxValidator{}.validateExpression({"1", "*", "2"}));
    REQUIRE(SyntaxValidator{}.validateExpression({"1", "/", "a"}));
    REQUIRE(SyntaxValidator{}.validateExpression({"a", "%", "b"}));
    REQUIRE(SyntaxValidator{}.validateExpression({"a", "+", "b1"}));
    REQUIRE(SyntaxValidator{}.validateExpression({"a2", "-", "B"}));
}

TEST_CASE("One level of brackets validation") {
    REQUIRE(SyntaxValidator{}.validateExpression({"1", "+", "(", "a", "*", "b", ")", "-", "(", "2", ")"}));
}

TEST_CASE("Two levels of brackets validation") {
    REQUIRE(SyntaxValidator{}.validateExpression({"1", "+", "(", "a", "*", "(", "b", "+", "C", ")", ")", "-", "(", "2", ")"}));
}
