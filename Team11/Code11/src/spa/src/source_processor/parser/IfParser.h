#ifndef SPA_IFPARSER_H
#define SPA_IFPARSER_H

#pragma once

#include "source_processor/parser/ParserManager.h"
#include "source_processor/parser/ConditionParser.h"
#include "source_processor/ast/statements/IfNode.h"
#include "source_processor/ast/statements/StatementNode.h"
#include "commons/tokenizer/tokens/Token.h"

class IfParser : public AbstractParser {
    using CurPtr = std::vector<std::shared_ptr<Token> >::iterator&;
public:
    IfParser() = default;
    std::shared_ptr<StatementNode> parse(CurPtr curToken,
                                         std::vector<std::shared_ptr<Token> >::iterator end,
                                         std::shared_ptr<int> lineNum) override;
};


#endif //SPA_IFPARSER_H
