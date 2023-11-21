#pragma once

#include <memory>
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/such_that/parent/ParentVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class Parent : public SuchThat, public std::enable_shared_from_this<Parent> {
public:
    bool hasStar;
    std::shared_ptr<IStatement> parentStatement;
    std::shared_ptr<IStatement> childStatement;

    Parent(const std::shared_ptr<IStatement>& parentStatement,
           const std::shared_ptr<IStatement>& childStatement,
           bool hasStar, bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateParent(const std::shared_ptr<IParentReader>& reader);
    int getPriority() override;
};
