#include "ParserManager.h"

/**
 * Main funtion to generate the AST. It will return a shared pointer to the root of the program
 * @param tokens list of tokens from tokenizer
 * @return shared_ptr to root program node
 */
std::shared_ptr<ProgramNode> ParserManager::generateAST(std::vector<std::shared_ptr<Token> > tokens) {
    lst = tokens;
    cur = tokens.begin();
    curLineNum = std::make_shared<int>(1);
    // Create program node here, run generate stmt list
    auto stmtList = generateProcedureList(cur, tokens.end(), curLineNum);
    auto procList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode> > >();
    /** Use a try catch here to check for potential error where one of the statements generated is not a procedure
     *  If error is thrown here, will have to check input SIMPLE, then look at ParserFactory -> ProcedureParser
     */
    for (auto it = stmtList->begin(); it != stmtList->end(); it++) {
        auto p = std::dynamic_pointer_cast<ProcedureNode>(*it);
        procList->push_back(p);
    }
    std::shared_ptr<ProgramNode> res = std::make_shared<ProgramNode>(procList);
    return res;
}

std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > ParserManager::generateProcedureList(
    std::vector<std::shared_ptr<Token> >::iterator &i,
    std::vector<std::shared_ptr<Token> >::iterator end,
    const std::shared_ptr<int>& lineNum) {
    auto res = std::make_shared<std::vector<std::shared_ptr<StatementNode> > >();
    while (i != end) {
        if ((**i).getStringValue() != "procedure") {
            throw std::invalid_argument("Statement " + (**i).getStringValue() + " not in a procedure");
        }
        ProcedureParser curParser;
        std::shared_ptr<StatementNode> curIteration = curParser.parse(i, end, lineNum);
        res->push_back(curIteration);
    }
    return res;
}

std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > ParserManager::generateStmtList(
    std::vector<std::shared_ptr<Token> >::iterator &i,
    std::vector<std::shared_ptr<Token> >::iterator end,
    const std::shared_ptr<int>& lineNum) {
    auto res = std::make_shared<std::vector<std::shared_ptr<StatementNode> > >();
    while ((**i).getStringValue() != "}" && i != end) {
        auto copy = i;
        copy++;
        std::shared_ptr<AbstractParser> curParser;
        if ((**copy).getStringValue() == "=") {
            curParser = std::make_shared<AssignmentParser>();
        } else if ((**i).getStringValue() == "procedure") {
            throw std::invalid_argument("Procedure cannot be in a procedure");
        } else {
            curParser = ParserFactory::getParser((**i).getStringValue());
        }
        std::shared_ptr<StatementNode> curIteration = curParser->parse(i, end, lineNum);
        res->push_back(curIteration);
    }
    return res;
}

std::vector<std::shared_ptr<Token> >::iterator ParserManager::getCur() {
    return cur;
}

std::vector<std::shared_ptr<Token> >::iterator ParserManager::getEnd() {
    return lst.end();
}

std::string ParserManager::peek() {
    auto nextIt = getCur();
    nextIt++;
    return (*nextIt)->getStringValue();
}
