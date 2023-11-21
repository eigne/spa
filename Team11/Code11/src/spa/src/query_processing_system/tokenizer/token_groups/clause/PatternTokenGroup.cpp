#include "PatternTokenGroup.h"
#include "query_processing_system/query_extractor/TokenGroupExtractor.h"
#include "query_processing_system/validator/TokenGroupSyntaxValidator.h"
#include "query_processing_system/validator/TokenGroupSemanticValidator.h"


PatternTokenGroup::PatternTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList)
        : ClauseTokenGroup(tokenList) {
}

bool PatternTokenGroup::validateSyntax(TokenGroupSyntaxValidator &v) {
    return v.visit(shared_from_this());
}

bool PatternTokenGroup::validateSemantics(TokenGroupSemanticValidator &v) {
    return v.visit(shared_from_this());
}

void PatternTokenGroup::buildQuery(TokenGroupExtractor &v, Query &q) {
    v.visit(shared_from_this(), q);
}
