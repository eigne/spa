#pragma once

#include <string>
#include <memory>
#include <vector>
#include "query_processing_system/tokenizer/token_groups/TokenGroup.h"
#include "query_processing_system/query/design_entities/ISynonym.h"

class DeclarationTokenGroup : public TokenGroup, public std::enable_shared_from_this<DeclarationTokenGroup> {
public:
    std::string type;
    std::vector<std::string> synonyms;
    explicit DeclarationTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList);
    bool validateSyntax(TokenGroupSyntaxValidator& v) override;
    bool validateSemantics(TokenGroupSemanticValidator& v) override;
    void buildQuery(TokenGroupExtractor& v, Query& q) override;
};
