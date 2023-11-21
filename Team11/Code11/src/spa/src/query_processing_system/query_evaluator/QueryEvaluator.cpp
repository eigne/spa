#include "QueryEvaluator.h"
#include "design_entity_visitor/SynonymVisitor.h"

QueryEvaluator::QueryEvaluator(const std::shared_ptr<IStorageReader>& storageReader) {
    this->storageReader = storageReader;
    this->synonymVisitor = std::make_shared<SynonymVisitor>(storageReader);
}

std::list<std::string> QueryEvaluator::evaluateQuery(Query& query) {
    auto synonyms = query.getSynonyms();
    for (const auto& synonym : synonyms) {
        synonym->initializePossibleValues(synonymVisitor);
    }

    while (query.hasClauses()) {
        auto clause = query.getNextClause();
        bool isTruthy = clause->evaluate(this->storageReader);
        if (!isTruthy) {
            return query.getSelected().getAnswer(false);
        }
    }

    return query.getSelected().getAnswer(true);
}
