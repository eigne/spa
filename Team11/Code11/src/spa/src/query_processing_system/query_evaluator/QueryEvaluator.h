#pragma once

#include <string>
#include <memory>
#include <list>
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"
#include "query_processing_system/query/Query.h"

class QueryEvaluator {
private:
    std::shared_ptr<IVisitsSynonym> synonymVisitor;
    std::shared_ptr<IStorageReader> storageReader;

public:
    QueryEvaluator() = default;
    explicit QueryEvaluator(const std::shared_ptr<IStorageReader>& storageReader);
    std::list<std::string> evaluateQuery(Query& query);
};
