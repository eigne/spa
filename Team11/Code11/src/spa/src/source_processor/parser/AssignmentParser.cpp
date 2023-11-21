#include "AssignmentParser.h"

std::shared_ptr<StatementNode>
AssignmentParser::parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                        std::vector<std::shared_ptr<Token> >::iterator end,
                        std::shared_ptr<int> lineNum) {
    std::string v = (*curToken)->getStringValue();
    SyntaxValidator s;
    if (!s.validateName(v)) {
        throw std::invalid_argument("Variable " + v + " to be assigned is not a valid variable name");
    }
    auto left = std::make_shared<VariableNode>((**curToken).getStringValue(), *lineNum);
    *curToken++;
    if ((*curToken)->getStringValue() != "=") {
        throw std::invalid_argument("Missing '=' in assignment at line number " + std::to_string(*lineNum));
    }
    *curToken++;
    OperatorParser opParser;
    auto right = opParser.parse(curToken, *lineNum);
    auto res = std::make_shared<AssignNode>(left, right, *lineNum);
    (*lineNum)++;
    *curToken++;
    return res;
}
