#pragma once

#include <memory>
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"
#include \
    "query_processing_system/query_evaluator/design_entity_visitor/interfaces/relationships/StatementStatementRelationship.h"
#include "query_processing_system/query_evaluator/storage_reader/IAffectsReader.h"

class AffectsVisitor : public IVisitsStatement, public StatementStatementRelationship {
private:
    std::shared_ptr<IAffectsReader> affectsReader;
    bool queryForAffects;

public:
    explicit AffectsVisitor(const std::shared_ptr<IAffectsReader>& nextReader);

    void visit(const std::shared_ptr<StatementTypeSynonym>& statement) override;
    void visit(const std::shared_ptr<StatementLineNumber>& statement) override;
    void visit(const std::shared_ptr<StatementWildcard>& statement) override;

    void setQueryState(bool state);
};
