#include "UsesTokenGroup.h"
#include "query_processing_system/query_extractor/TokenGroupExtractor.h"
#include "query_processing_system/validator/TokenGroupSyntaxValidator.h"
#include "query_processing_system/validator/TokenGroupSemanticValidator.h"

bool UsesTokenGroup::validateSyntax(TokenGroupSyntaxValidator &v) {
    return v.visit(shared_from_this());
}

bool UsesTokenGroup::validateSemantics(TokenGroupSemanticValidator &v) {
    return v.visit(shared_from_this());
}

void UsesTokenGroup::buildQuery(TokenGroupExtractor &v, Query &q) {
    v.visit(shared_from_this(), q);
}
