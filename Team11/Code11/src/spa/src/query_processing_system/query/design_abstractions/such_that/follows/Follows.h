#pragma once

#include <memory>
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/such_that/follows/FollowsVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class Follows : public SuchThat, public std::enable_shared_from_this<Follows> {
public:
    bool hasStar;
    std::shared_ptr<IStatement> beforeStatement;
    std::shared_ptr<IStatement> afterStatement;

    Follows(const std::shared_ptr<IStatement>& beforeStatement,
            const std::shared_ptr<IStatement>& afterStatement,
            bool hasStar, bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateFollows(const std::shared_ptr<IFollowsReader>& reader);
    int getPriority() override;
};
