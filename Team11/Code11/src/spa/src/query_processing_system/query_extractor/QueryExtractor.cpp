#include "QueryExtractor.h"

QueryExtractor::QueryExtractor(const std::vector<std::shared_ptr<TokenGroup> >& tokenGroups) {
    this->tokenGroups = tokenGroups;
    this->tokenGroupVisitor = TokenGroupExtractor();
}

Query QueryExtractor::extractQuery() {
    Query query;
    for (const auto& tokenGroup : tokenGroups) {
        tokenGroup->buildQuery(tokenGroupVisitor, query);
    }

    return query;
}
