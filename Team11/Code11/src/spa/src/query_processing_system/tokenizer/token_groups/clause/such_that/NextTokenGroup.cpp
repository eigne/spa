#include "NextTokenGroup.h"
#include "query_processing_system/query_extractor/TokenGroupExtractor.h"
#include "query_processing_system/validator/TokenGroupSyntaxValidator.h"
#include "query_processing_system/validator/TokenGroupSemanticValidator.h"

bool NextTokenGroup::validateSyntax(TokenGroupSyntaxValidator &v) {
    return v.visit(shared_from_this());
}

bool NextTokenGroup::validateSemantics(TokenGroupSemanticValidator &v) {
    return v.visit(shared_from_this());
}

void NextTokenGroup::buildQuery(TokenGroupExtractor &v, Query &q) {
    v.visit(shared_from_this(), q);
}
