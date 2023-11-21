#pragma once

#include <memory>
#include <set>
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class Clause {
public:
    bool isNegated;
    int clauseWeight;
    std::shared_ptr<std::set<std::string>> synonym;
    Clause(bool isNegated, int clauseWeight) : isNegated(isNegated), clauseWeight(clauseWeight) {}
    std::shared_ptr<std::set<std::string>> getSynonyms() { return synonym; }

    virtual bool evaluate(const std::shared_ptr<IStorageReader>& reader) = 0;
    virtual int getPriority() = 0;
};
