#include "IfParser.h"

std::shared_ptr<StatementNode> IfParser::parse(CurPtr curToken,
                                               std::vector<std::shared_ptr<Token> >::iterator end,
                                               std::shared_ptr<int> lineNum) {
    int ifLineNum = *lineNum;
    curToken++;
    (*lineNum)++;
    ConditionParser c;
    auto condNode = c.parseCondition(curToken);
    curToken++;
    auto cur = (*curToken)->getStringValue();
    if (cur != "then") {
        throw std::invalid_argument("Missing then keyword");
    }
    curToken++;
    cur = (*curToken)->getStringValue();
    if (cur != "{") {
        throw std::invalid_argument("Missing open curly brace after 'then'");
    }
    curToken++;
    cur = (*curToken)->getStringValue();
    auto thenStmtList = ParserManager::generateStmtList(curToken, end, lineNum);

    cur = (*curToken)->getStringValue();
    if (cur != "}") {
        throw std::invalid_argument("Missing close curly brace in if statement");
    }

    curToken++;
    cur = (*curToken)->getStringValue();
    if (cur != "else") {
        throw std::invalid_argument("Missing else keyword");
    }
    curToken+= 2;
    auto elseStmtList = ParserManager::generateStmtList(curToken, end, lineNum);
    curToken++;
    return std::make_shared<IfNode>(condNode, thenStmtList, elseStmtList, ifLineNum);
}
