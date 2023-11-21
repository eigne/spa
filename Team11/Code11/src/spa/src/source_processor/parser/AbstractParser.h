#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include "source_processor/ast/statements/StatementNode.h"
#include "commons/tokenizer/tokens/Token.h"
#include "commons/validator/SyntaxValidator.h"

class AbstractParser {
public:
    virtual ~AbstractParser() = default;
    virtual std::shared_ptr<StatementNode> parse(std::vector<std::shared_ptr<Token> >::iterator& curToken,
                                                 std::vector<std::shared_ptr<Token> >::iterator end,
                                                 std::shared_ptr<int> lineNum) = 0;
};
