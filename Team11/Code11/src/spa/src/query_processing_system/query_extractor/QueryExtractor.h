#pragma once

#include "commons/tokenizer/tokens/Token.h"
#include "query_processing_system/tokenizer/TokenSplitter.h"
#include "query_processing_system/query/Query.h"
#include "query_processing_system/query_extractor/TokenGroupExtractor.h"

class QueryExtractor {
private:
    std::vector<std::shared_ptr<TokenGroup> > tokenGroups;
    TokenGroupExtractor tokenGroupVisitor;

public:
    explicit QueryExtractor(const std::vector<std::shared_ptr<TokenGroup> >& tokenGroups);
    Query extractQuery();
};
