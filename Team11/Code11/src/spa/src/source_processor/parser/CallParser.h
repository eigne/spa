#pragma once

#include "AbstractParser.h"
#include "source_processor/ast/statements/CallNode.h"
#include "source_processor/ast/ProcedureNode.h"

class CallParser : public AbstractParser {
public:
    CallParser() = default;
    std::shared_ptr<StatementNode> parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                                         std::vector<std::shared_ptr<Token> >::iterator end,
                                         std::shared_ptr<int> lineNum) override;
};
