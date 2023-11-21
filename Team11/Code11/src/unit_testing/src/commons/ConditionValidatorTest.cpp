#include <string>
#include <vector>
#include <algorithm>
#include "commons/validator/SyntaxValidator.h"
#include "catch.hpp"

SyntaxValidator s;

std::vector<std::string> justRelExpr = {"a", "<", "b"};
std::vector<std::string> andCase = {"(", "a", "<", "b", ")", "&&", "(", "a", "<", "b", ")"};
std::vector<std::string> orCase = {"(", "a", "<", "b", ")", "||", "(", "a", "<", "b", ")"};

TEST_CASE("Correct conditions"){
    CHECK_NOTHROW(s.validateCondExpr(andCase));
    CHECK_NOTHROW(s.validateCondExpr(orCase));
    CHECK_NOTHROW(s.validateCondExpr(justRelExpr));
}

std::vector<std::string> nestedCondExprFront = {"(", "(", "a", "<", "b", ")", "&&", "(", "a", "<", "b", ")", ")", "&&", "(", "a", "<", "b", ")"};
std::vector<std::string> nestedCondExprBack = {"(", "a", "<", "b", ")", "&&", "(", "(", "a", "<", "b", ")", "&&", "(", "a", "<", "b", ")", ")"};
std::vector<std::string> nestedCondExprBoth = {"(", "(", "a", "<", "b", ")", "&&", "(", "a", "<", "b", ")", ")", "&&", "(", "(", "a", "<", "b", ")", "&&", "(", "a", "<", "b", ")", ")"};
TEST_CASE("Nested condition operator is allowed"){
    CHECK_NOTHROW(s.validateCondExpr(nestedCondExprFront));
    CHECK_NOTHROW(s.validateCondExpr(nestedCondExprBack));
    CHECK_NOTHROW(s.validateCondExpr(nestedCondExprBoth));
}


std::vector<std::string> multiCondOp = {"(", "(", "a", "<", "b", ")", ")", "&&", "(", "a", "<", "b", ")", "||", "(", "a", "<", "b", ")",};
TEST_CASE("Multiple condition operator fails"){
    CHECK_THROWS_WITH(s.validateCondExpr(multiCondOp), "Conditional expression has character after closed parenthesis");
}

std::vector<std::string> invalidCondOp = {"||", "(", "(", "a", "<", "b", ")", ")", "&&", "(", "a", "<", "b", ")"};
TEST_CASE("Invalid condition expression fails"){
    CHECK_THROWS_WITH(s.validateCondExpr(invalidCondOp), "Conditional expression does not follow syntax with two parenthesis at ||");
}

std::vector<std::string> justRelExprInBrace = {"(", "a", "<", "b", ")"};
std::vector<std::string> nestedArgInAndCase = {"(", "(", "a", "<", "b", ")", ")", "&&", "(", "a", "<", "b", ")"};

TEST_CASE("Relational expression cannot be surrounded with brace unless in a condition"){
    CHECK_THROWS_WITH(s.validateCondExpr(justRelExprInBrace), "Brace alone without conditional operator is not valid syntax");
    CHECK_THROWS_WITH(s.validateCondExpr(nestedArgInAndCase), "Brace alone without conditional operator is not valid syntax");
}

TEST_CASE("Relational expression with no relational operator should fail") {
    std::vector<std::string> relExprWithNoOp = {"(", "a", ")"};
    CHECK_THROWS(s.validateRelExpr(relExprWithNoOp));
}