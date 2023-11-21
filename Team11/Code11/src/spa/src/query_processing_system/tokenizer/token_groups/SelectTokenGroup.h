#pragma once

#include <string>
#include <memory>
#include <vector>
#include "query_processing_system/tokenizer/token_groups/TokenGroup.h"
#include "query_processing_system/query/query_selection/QuerySelection.h"

class SelectTokenGroup : public TokenGroup, public std::enable_shared_from_this<SelectTokenGroup> {
public:
    bool isBOOLEAN = false;
    std::vector<std::string> synonyms;
    std::vector<std::pair<std::string, std::string>> synonymsAndAttributes;
    SelectTokenGroup() = default;
    explicit SelectTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList);
    bool validateSyntax(TokenGroupSyntaxValidator& v) override;
    bool validateSemantics(TokenGroupSemanticValidator& v) override;
    void buildQuery(TokenGroupExtractor& v, Query& q) override;
};
