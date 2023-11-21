#include "CallParser.h"

std::shared_ptr<StatementNode>
CallParser::parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                  std::vector<std::shared_ptr<Token> >::iterator end,
                  std::shared_ptr<int> lineNum) {
    *curToken++;
    auto emptyList = std::make_shared<std::vector<std::shared_ptr<StatementNode> > >();
    auto res = std::make_shared<CallNode>((**curToken).getStringValue(), *lineNum);
    curToken = curToken + 1;
    if ((**curToken).getStringValue() != ";") {
        std::string err;
        err += "Missing semicolon in call " + res->getProc()+ " at line number " +  std::to_string(*lineNum);
        throw std::invalid_argument(err);
    }
    curToken = curToken + 1;
    *lineNum = *lineNum + 1;
    return res;
}
