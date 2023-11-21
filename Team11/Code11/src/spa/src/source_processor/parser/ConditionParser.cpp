#include "ConditionParser.h"
#include <iostream>


/**
 * Parses the condition expression for control flow statements
 * @param curToken
 * @return starts with left outer parenthesis for condition, ends with pointer pointing to next token after right outer parenthesis
 * e.g "while (...){" starts with "("
 */
std::shared_ptr<ConditionNode> ConditionParser::parseCondition(ConditionParser::CurPtr curToken) {
    std::stack<std::string> braces;
    std::string cur = (*curToken)->getStringValue();
    if (cur != "(") {
        throw std::invalid_argument("Condition to be parsed needs to start with brace");
    }
    braces.emplace("(");
    curToken++;
    cur = (*curToken)->getStringValue();
    while (!braces.empty()) {
        if (cur == "(") {
            braces.push(cur);
        } else if (isalpha(cur[0])) {
            varList.push_back(cur);
        } else if (isdigit(cur[0])) {
            constList.push_back(cur);
        } else if (cur == ")") {
            braces.pop();
            if (braces.empty()) {
                break;
            }
        }
        cond += cur;
        listOfCondTokens.push_back(cur);
        curToken++;
        cur = (*curToken)->getStringValue();
    }
    SyntaxValidator s;
    s.validateCondExpr(listOfCondTokens);
    sort(varList.begin(), varList.end());
    varList.erase(unique(varList.begin(), varList.end()), varList.end());
    sort(constList.begin(), constList.end());
    constList.erase(unique(constList.begin(), constList.end()), constList.end());
    return std::make_shared<ConditionNode>(varList, constList, cond);
}

void ConditionParser::printConstList() {
    for (auto & i : constList)
        std::cout << i << std::endl;
}

void ConditionParser::printVarList() {
    for (auto & i : varList) {
        std::cout << i << std::endl;
    }
}
