#include "SelectTokenGroup.h"
#include "query_processing_system/query_extractor/TokenGroupExtractor.h"
#include "query_processing_system/validator/TokenGroupSyntaxValidator.h"
#include "query_processing_system/validator/TokenGroupSemanticValidator.h"

SelectTokenGroup::SelectTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList) : TokenGroup(tokenList) {
}

bool SelectTokenGroup::validateSyntax(TokenGroupSyntaxValidator &v) {
    return v.visit(shared_from_this());
}

bool SelectTokenGroup::validateSemantics(TokenGroupSemanticValidator &v) {
    return v.visit(shared_from_this());
}

void SelectTokenGroup::buildQuery(TokenGroupExtractor &v, Query &q) {
    return v.visit(shared_from_this(), q);
}
