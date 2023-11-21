#include "ReadParser.h"

std::shared_ptr<StatementNode> ReadParser::parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                                                 std::vector<std::shared_ptr<Token> >::iterator end,
                                                 std::shared_ptr<int> lineNum) {
    curToken++;
    SyntaxValidator s;
    if (!s.validateName((**curToken).getStringValue())) {
        throw std::invalid_argument("Variable to be read is not valid variable");
    }
    std::shared_ptr<VariableNode> v = std::make_shared<VariableNode>((**curToken).getStringValue(), *lineNum);
    auto res = std::make_shared<ReadNode>(v, *lineNum);
    curToken = curToken + 1;
    if ((**curToken).getStringValue() != ";") {
        std::string err;
        err += "Missing semicolon in read " + v->getVarName() + " at line number " + std::to_string(
            v->getAssociatedLineNum());
        throw std::invalid_argument(err);
    }
    curToken = curToken + 1;
    *lineNum = *lineNum + 1;
    return res;
}
