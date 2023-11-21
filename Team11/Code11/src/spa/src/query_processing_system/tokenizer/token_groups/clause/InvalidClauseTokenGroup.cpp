#include "InvalidClauseTokenGroup.h"
#include "query_processing_system/query_extractor/TokenGroupExtractor.h"
#include "query_processing_system/validator/TokenGroupSyntaxValidator.h"
#include "query_processing_system/validator/TokenGroupSemanticValidator.h"


InvalidClauseTokenGroup::InvalidClauseTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList) :
    ClauseTokenGroup(tokenList) {
}

bool InvalidClauseTokenGroup::validateSyntax(TokenGroupSyntaxValidator &v) {
    return v.visit(shared_from_this());
}

bool InvalidClauseTokenGroup::validateSemantics(TokenGroupSemanticValidator &v) {
    return v.visit(shared_from_this());
}

void InvalidClauseTokenGroup::buildQuery(TokenGroupExtractor &v, Query &q) {
    v.visit(shared_from_this(), q);
}
