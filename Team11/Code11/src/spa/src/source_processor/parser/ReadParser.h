#ifndef SPA_READPARSER_H
#define SPA_READPARSER_H

#pragma once

#include "AbstractParser.h"
#include "source_processor/ast/statements/ReadNode.h"
#include "source_processor/ast/expressions/VariableNode.h"

class ReadParser : public AbstractParser {
public:
    ReadParser() = default;
    std::shared_ptr<StatementNode> parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                                         std::vector<std::shared_ptr<Token> >::iterator end,
                                         std::shared_ptr<int> lineNum) override;
};


#endif //SPA_READPARSER_H
