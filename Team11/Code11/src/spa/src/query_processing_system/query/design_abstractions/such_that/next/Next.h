#pragma once

#include <memory>
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/such_that/next/NextVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class Next : public SuchThat, public std::enable_shared_from_this<Next> {
public:
    bool hasStar;
    std::shared_ptr<IStatement> previousStatement;
    std::shared_ptr<IStatement> nextStatement;

    Next(const std::shared_ptr<IStatement>& previousStatement,
        const std::shared_ptr<IStatement>& nextStatement, bool hasStar,
        bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateNext(const std::shared_ptr<INextReader>& reader);
    int getPriority() override;
};
