#ifndef SPA_CONDITIONPARSER_H
#define SPA_CONDITIONPARSER_H

#pragma once

#include "source_processor/parser/OperatorParser.h"
#include "source_processor/ast/statements/ConditionNode.h"
#include "commons/validator/SyntaxValidator.h"

class ConditionParser {
    using CurPtr = std::vector<std::shared_ptr<Token> >::iterator&;

public:
    ConditionParser() = default;
    std::shared_ptr<ConditionNode> parseCondition(CurPtr curToken);
    void parseRelFactor(CurPtr tokens);
    void parseRelExpr(CurPtr tokens);
    void parseCondExpr(CurPtr tokens);
    void printVarList();
    void printConstList();

private:
    std::unordered_set<std::string> ExprSymbols {"+", "-", "*", "/", "%"};
    std::unordered_set<std::string> RelationSymbols {"<", "<=", ">", ">=", "==", "!="};
    std::unordered_set<std::string> ConditionSymbols {"&&", "||"};
    inline bool isRelationOperator(const std::string& cur) {
        return RelationSymbols.find(cur) != RelationSymbols.end();
    }
    inline bool isConditionOperator(const std::string& cur) {
        return ConditionSymbols.find(cur) != ConditionSymbols.end();
    }
    inline bool isExprOperator(const std::string& cur) {
        return ExprSymbols.find(cur) != ExprSymbols.end();
    }
    std::vector<std::string> varList;
    std::vector<std::string> constList;
    std::string cond;
    std::vector<std::string> listOfCondTokens;
};


#endif //SPA_CONDITIONPARSER_H
