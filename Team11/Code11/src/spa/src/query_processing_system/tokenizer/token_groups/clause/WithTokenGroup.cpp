#include "WithTokenGroup.h"
#include "query_processing_system/query_extractor/TokenGroupExtractor.h"
#include "query_processing_system/validator/TokenGroupSyntaxValidator.h"
#include "query_processing_system/validator/TokenGroupSemanticValidator.h"


WithTokenGroup::WithTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList) : ClauseTokenGroup(tokenList) {
}

bool WithTokenGroup::validateSyntax(TokenGroupSyntaxValidator& v) {
    return v.visit(shared_from_this());
}

bool WithTokenGroup::validateSemantics(TokenGroupSemanticValidator& v) {
    return v.visit(shared_from_this());
}

void WithTokenGroup::buildQuery(TokenGroupExtractor& v, Query& q) {
    v.visit(shared_from_this(), q);
}
