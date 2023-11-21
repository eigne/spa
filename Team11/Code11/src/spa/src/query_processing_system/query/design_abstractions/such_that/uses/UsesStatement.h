#pragma once

#include <memory>
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/such_that/uses/UsesStatementVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class UsesStatement : public SuchThat, public std::enable_shared_from_this<UsesStatement> {
public:
    std::shared_ptr<IStatement> statementThatUses;
    std::shared_ptr<IVariable> variableUsed;

    UsesStatement(const std::shared_ptr<IStatement>& statementThatUses,
                  const std::shared_ptr<IVariable>& variableUsed,
                  bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateUsesStatement(const std::shared_ptr<IUsesStatementReader>& reader);
    int getPriority() override;
};
