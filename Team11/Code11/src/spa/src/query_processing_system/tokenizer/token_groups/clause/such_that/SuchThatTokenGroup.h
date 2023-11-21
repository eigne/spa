#pragma once

#include <string>
#include <memory>
#include <vector>
#include "query_processing_system/tokenizer/token_groups/clause/ClauseTokenGroup.h"

class SuchThatTokenGroup : public ClauseTokenGroup {
public:
    std::string type;
    std::string arg1;
    std::string arg2;

// arg1 attributes
    bool arg1IsWildcard{};
    bool arg1IsString{};
    bool arg1IsNumber{};
    bool arg1IsSynonym{};

// arg2 attributes
    bool arg2IsWildcard{};
    bool arg2IsString{};
    bool arg2IsNumber{};
    bool arg2IsSynonym{};

    explicit SuchThatTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList);
    void setAttributes();
};
