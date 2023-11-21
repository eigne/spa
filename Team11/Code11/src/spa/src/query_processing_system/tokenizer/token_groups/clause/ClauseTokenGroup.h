#pragma once

#include <string>
#include <memory>
#include <vector>
#include "query_processing_system/tokenizer/token_groups/TokenGroup.h"
#include "query_processing_system/query/design_abstractions/Clause.h"

class ClauseTokenGroup : public TokenGroup {
public:
    explicit ClauseTokenGroup(const std::vector<std::shared_ptr<Token>>& tokenList);
    bool isNegated{};
};
