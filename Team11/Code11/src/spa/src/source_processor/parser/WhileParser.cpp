#include "WhileParser.h"


std::shared_ptr<StatementNode> WhileParser::parse(CurPtr &curToken,
                                                  std::vector<std::shared_ptr<Token> >::iterator end,
                                                  std::shared_ptr<int> lineNum) {
    curToken++;
    int whileLineNum = *lineNum;
    (*lineNum)++;
    if ((*curToken)->getStringValue() != "(") {
        throw std::invalid_argument("Missing open parenthesis for condition in while loop");
    }
    ConditionParser c;
    auto condNode = c.parseCondition(curToken);
    curToken += 2;
    auto stmtList = ParserManager::generateStmtList(curToken, end, lineNum);
    curToken++;
    return std::make_shared<WhileNode>(condNode, stmtList, whileLineNum);
}
