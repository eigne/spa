#include "ProcedureParser.h"

std::shared_ptr<StatementNode>
ProcedureParser::parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                       std::vector<std::shared_ptr<Token> >::iterator end,
                       std::shared_ptr<int> lineNum) {
    std::string cur = (*curToken)->getStringValue();
    if (cur != "procedure") {
        // Throw error here and exit
    }
    curToken++;
    std::string procName = (*curToken)->getStringValue();
    curToken += 2;
    auto stmtLst = ParserManager::generateStmtList(curToken, end, lineNum);
    auto newProc = std::make_shared<ProcedureNode>(procName, stmtLst);
    cur = (*curToken)->getStringValue();
    if (cur != "}") {
        throw std::invalid_argument("Procedure " + procName + " has no closing brace");
    }
    curToken++;
    return newProc;
}
