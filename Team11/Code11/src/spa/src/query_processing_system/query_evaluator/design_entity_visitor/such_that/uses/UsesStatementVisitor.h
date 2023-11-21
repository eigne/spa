#pragma once

#include <memory>

#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsVariable.h"
#include \
    "query_processing_system/query_evaluator/design_entity_visitor/interfaces/relationships/StatementEntityRelationship.h"

#include "query_processing_system/query_evaluator/storage_reader/IUsesStatementReader.h"

class UsesStatementVisitor : public IVisitsStatement, public IVisitsVariable, public StatementEntityRelationship {
private:
    std::shared_ptr<IUsesStatementReader> usesReader;
public:
    explicit UsesStatementVisitor(const std::shared_ptr<IUsesStatementReader>& usesReader);

    void visit(const std::shared_ptr<StatementTypeSynonym>& statement) override;
    void visit(const std::shared_ptr<StatementLineNumber>& statement) override;
    void visit(const std::shared_ptr<StatementWildcard>& statement) override;

    void visit(const std::shared_ptr<VariableSynonym>& variable) override;
    void visit(const std::shared_ptr<VariableName>& variable) override;
    void visit(const std::shared_ptr<VariableWildcard>& variable) override;
};
