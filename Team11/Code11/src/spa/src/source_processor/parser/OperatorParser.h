#ifndef SPA_OPERATORPARSER_H
#define SPA_OPERATORPARSER_H

#pragma once

#include "source_processor/ast/expressions/ExpressionNode.h"
#include "source_processor/ast/expressions/VariableNode.h"
#include "source_processor/ast/expressions/ConstantNode.h"
#include "commons/tokenizer/tokens/Token.h"
#include "commons/validator/SyntaxValidator.h"

class OperatorParser {
public:
    OperatorParser() = default;
    std::shared_ptr<ExpressionNode> parse(std::vector<std::shared_ptr<Token> >::iterator& curToken, int index);
};


#endif //SPA_OPERATORPARSER_H
