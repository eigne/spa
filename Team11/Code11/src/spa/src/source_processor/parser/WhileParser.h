#ifndef SPA_WHILEPARSER_H
#define SPA_WHILEPARSER_H

#pragma once

#include "source_processor/ast/statements/WhileNode.h"
#include "source_processor/ast/statements/StatementNode.h"
#include "source_processor/parser/ParserManager.h"
#include "source_processor/parser/ConditionParser.h"
#include "commons/tokenizer/tokens/Token.h"

class WhileParser : public AbstractParser {
    using CurPtr = std::vector<std::shared_ptr<Token> >::iterator&;
public:
    WhileParser() = default;
    std::shared_ptr<StatementNode> parse(CurPtr curToken,
                                         std::vector<std::shared_ptr<Token> >::iterator end,
                                         std::shared_ptr<int> lineNum) override;
};


#endif //SPA_WHILEPARSER_H
