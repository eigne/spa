#pragma once

#include <unordered_set>
#include <memory>
#include "commons/validator/SyntaxValidator.h"
#include "commons/tokenizer/tokens/Token.h"
#include "query_processing_system/tokenizer/TokenSplitter.h"
#include "query_processing_system/validator/TokenGroupSyntaxValidator.h"
#include "query_processing_system/validator/TokenGroupSemanticValidator.h"

class QPSValidator : public SyntaxValidator  {
private:
    std::vector<std::shared_ptr<TokenGroup> > tokenGroups;
    TokenGroupSyntaxValidator syntaxValidator;
    TokenGroupSemanticValidator semanticValidator;

public:
    explicit QPSValidator(const std::vector<std::shared_ptr<TokenGroup> >& tokenGroups);
    std::string validate();
};
