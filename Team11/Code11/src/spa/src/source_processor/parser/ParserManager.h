#ifndef SPA_PARSERMANAGER_H
#define SPA_PARSERMANAGER_H

#pragma once

#include <vector>
#include <memory>
#include <stack>
#include <iostream>

#include "ParserManager.h"
#include "ParserFactory.h"
#include "commons/tokenizer/tokens/Token.h"
#include "source_processor/ast/ProgramNode.h"
#include "source_processor/parser/AssignmentParser.h"
#include "source_processor/parser/ProcedureParser.h"

class ParserManager {
public:
    ParserManager() = default;
    ~ParserManager() = default;
    std::shared_ptr<ProgramNode> generateAST(std::vector<std::shared_ptr<Token> > tokens);
    static std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > generateProcedureList(
        std::vector<std::shared_ptr<Token> >::iterator& i, std::vector<std::shared_ptr<Token> >::iterator end,
        const std::shared_ptr<int>& lineNum);
    static std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > >
    generateStmtList(std::vector<std::shared_ptr<Token> >::iterator &i,
                     std::vector<std::shared_ptr<Token> >::iterator end,
                     const std::shared_ptr<int>& lineNum);
    std::vector<std::shared_ptr<Token> >::iterator getCur();
    std::vector<std::shared_ptr<Token> >::iterator getEnd();
    std::string peek();

private:
    std::vector<std::shared_ptr<Token> > lst;
    std::vector<std::shared_ptr<Token> >::iterator cur;
    std::shared_ptr<int> curLineNum;
};


#endif //SPA_PARSERMANAGER_H
