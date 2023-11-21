#pragma once

#include <memory>
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/such_that/modifies/ModifiesStatementVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class ModifiesStatement : public SuchThat, public std::enable_shared_from_this<ModifiesStatement> {
public:
    std::shared_ptr<IStatement> statementThatModifies;
    std::shared_ptr<IVariable> variableModified;

    ModifiesStatement(const std::shared_ptr<IStatement>& statementThatModifies,
                      const std::shared_ptr<IVariable>& variableModified,
                      bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateModifiesStatement(const std::shared_ptr<IModifiesStatementReader>& reader);
    int getPriority() override;
};
