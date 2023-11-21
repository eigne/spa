#pragma once

#include <memory>
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/such_that/affects/AffectsVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class Affects : public SuchThat {
public:
    std::shared_ptr<IStatement> affectingStatement;
    std::shared_ptr<IStatement> affectedStatement;

    Affects(const std::shared_ptr<IStatement>& beforeStatement,
        const std::shared_ptr<IStatement>& afterStatement,
        bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateAffects(const std::shared_ptr<IAffectsReader>& reader);
    int getPriority() override;
};
