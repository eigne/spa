#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "commons/tokenizer/tokens/Token.h"
#include "query_processing_system/query/Query.h"

class TokenGroupExtractor;
class TokenGroupSyntaxValidator;
class TokenGroupSemanticValidator;

class TokenGroup {
protected:
    std::vector<std::shared_ptr<Token> > tokenList;

public:
    TokenGroup() = default;
    explicit TokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList);
    int getSize();
    virtual bool validateSyntax(TokenGroupSyntaxValidator& v) = 0;
    virtual bool validateSemantics(TokenGroupSemanticValidator& v) = 0;
    std::vector<std::string> getStringifiedTokens();
    virtual void buildQuery(TokenGroupExtractor& v, Query& q) = 0;
};
