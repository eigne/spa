#include "DeclarationTokenGroup.h"
#include "query_processing_system/query_extractor/TokenGroupExtractor.h"
#include "query_processing_system/validator/TokenGroupSyntaxValidator.h"
#include "query_processing_system/validator/TokenGroupSemanticValidator.h"


DeclarationTokenGroup::DeclarationTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList) : TokenGroup(
        tokenList) {
}

bool DeclarationTokenGroup::validateSyntax(TokenGroupSyntaxValidator &v) {
    return v.visit(shared_from_this());
}

bool DeclarationTokenGroup::validateSemantics(TokenGroupSemanticValidator &v) {
    return v.visit(shared_from_this());
}

void DeclarationTokenGroup::buildQuery(TokenGroupExtractor &v, Query &q) {
    v.visit(shared_from_this(), q);
}
