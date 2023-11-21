#pragma once

#include <string>
#include <memory>
#include <vector>
#include "query_processing_system/tokenizer/token_groups/clause/ClauseTokenGroup.h"

class PatternTokenGroup : public ClauseTokenGroup, public std::enable_shared_from_this<PatternTokenGroup> {
public:
    std::string type; // assign, while, if
    std::string statement; // synonym before the brackets
    std::string entity; // first argument
    std::string expression; // only relevant for assignment type
    bool secondArgumentIsWildcard{};
    bool thirdArgumentIsWildcard{};
    bool hasWildcard{}; // only relevant for assignment type
    bool hasThirdExpression{};

    explicit PatternTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList);
    bool validateSyntax(TokenGroupSyntaxValidator& v) override;
    bool validateSemantics(TokenGroupSemanticValidator& v) override;
    void buildQuery(TokenGroupExtractor& v, Query& q) override;
};
