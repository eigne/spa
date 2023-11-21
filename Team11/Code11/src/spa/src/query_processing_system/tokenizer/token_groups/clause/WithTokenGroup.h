#pragma once

#include <string>
#include <memory>
#include <vector>
#include "query_processing_system/tokenizer/token_groups/TokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/ClauseTokenGroup.h"
#include "query_processing_system/query/design_abstractions/Clause.h"

class WithTokenGroup : public ClauseTokenGroup, public std::enable_shared_from_this<WithTokenGroup> {
public:
    std::string firstRef;
    std::string firstRefAttrName; // If there is an attrName, means that it is a synonym
    bool isFirstRefInteger; // If true, Integer, else "IDENT"
    std::string secondRef;
    std::string secondRefAttrName;
    bool isSecondRefInteger;;

    explicit WithTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList);

    bool validateSyntax(TokenGroupSyntaxValidator& v) override;
    bool validateSemantics(TokenGroupSemanticValidator& v) override;
    void buildQuery(TokenGroupExtractor& v, Query& q) override;
};
