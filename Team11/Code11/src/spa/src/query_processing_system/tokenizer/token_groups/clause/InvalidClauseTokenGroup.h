#pragma once

#include <string>
#include <memory>
#include <vector>
#include "query_processing_system/tokenizer/token_groups/clause/ClauseTokenGroup.h"

class InvalidClauseTokenGroup : public ClauseTokenGroup, public std::enable_shared_from_this<InvalidClauseTokenGroup> {
public:
    explicit InvalidClauseTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList);
    bool validateSyntax(TokenGroupSyntaxValidator& v) override;
    bool validateSemantics(TokenGroupSemanticValidator& v) override;
    void buildQuery(TokenGroupExtractor& v, Query& q) override;
};
