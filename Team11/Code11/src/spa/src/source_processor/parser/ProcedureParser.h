#ifndef SPA_PROCEDUREPARSER_H
#define SPA_PROCEDUREPARSER_H

#pragma once

#include "source_processor/ast/ProcedureNode.h"
#include "commons/tokenizer/tokens/Token.h"
#include "ParserManager.h"

class ProcedureParser : public AbstractParser {
public:
    ProcedureParser() = default;
    std::shared_ptr<StatementNode> parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                                         std::vector<std::shared_ptr<Token> >::iterator end,
                                         std::shared_ptr<int> lineNum) override;
};


#endif //SPA_PROCEDUREPARSER_H
