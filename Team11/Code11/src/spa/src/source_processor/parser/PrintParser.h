#ifndef SPA_PRINTPARSER_H
#define SPA_PRINTPARSER_H

#pragma once

#include "AbstractParser.h"
#include "source_processor/ast/statements/PrintNode.h"
#include "source_processor/ast/expressions/VariableNode.h"

class PrintParser : public AbstractParser {
public:
    PrintParser() = default;
    std::shared_ptr<StatementNode> parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                                         std::vector<std::shared_ptr<Token> >::iterator end,
                                         std::shared_ptr<int> lineNum) override;
};


#endif //SPA_PRINTPARSER_H
