#pragma once

#include <vector>
#include <unordered_set>
#include <string>
#include <stack>
#include <stdexcept>

class SyntaxValidator {
private:
    const std::unordered_set<std::string> validExpressionOperators{
        "+", "-", "*", "/", "%"
    };
    std::unordered_set<std::string> RelationSymbols{ "<", "<=", ">", ">=", "==", "!=" };
    std::unordered_set<std::string> ConditionSymbols{ "&&", "||" };

public:
    using tokenPointer = std::vector<std::string>::iterator;

    SyntaxValidator() = default;
    bool validateName(const std::string& name);
    bool validateInteger(const std::string& integer);
    bool validateBrackets(const std::vector<std::string>& bracketArray);
    bool validateExpression(const std::vector<std::string>& s);
    void validateCondExpr(std::vector<std::string> t);
    void validateRelExpr(std::vector<std::string>);
    std::vector<std::string> getSubVectorBetweenParen(tokenPointer t);
    tokenPointer getEndPointerOfParen(tokenPointer t);
    inline bool isRelationOperator(const std::string& cur) {
        return RelationSymbols.count(cur) > 0;
    }
    inline bool isConditionOperator(const std::string& cur) {
        return ConditionSymbols.count(cur) > 0;
    }
};
