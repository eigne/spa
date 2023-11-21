#pragma once

#include "source_processor/ast/statements/AssignNode.h"
#include "source_processor/ast/expressions/VariableNode.h"
#include "commons/validator/SyntaxValidator.h"
#include "OperatorParser.h"
#include "AbstractParser.h"

class AssignmentParser : public AbstractParser {
public:
    AssignmentParser() = default;
    std::shared_ptr<StatementNode> parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                                         std::vector<std::shared_ptr<Token> >::iterator end,
                                         std::shared_ptr<int> lineNum) override;
};
