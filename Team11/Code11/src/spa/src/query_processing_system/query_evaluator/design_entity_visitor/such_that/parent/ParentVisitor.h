#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"
#include \
    "query_processing_system/query_evaluator/design_entity_visitor/interfaces/relationships/StatementStatementRelationship.h"
#include "query_processing_system/query_evaluator/storage_reader/IParentReader.h"

class ParentVisitor : public IVisitsStatement, public StatementStatementRelationship {
private:
    bool onlyQueryImmediate;
    bool queryForParents;
    std::shared_ptr<IParentReader> parentReader;
    std::unordered_set<int> queryBasedOnState(int statementNumber);

public:
    explicit ParentVisitor(const std::shared_ptr<IParentReader>& parentReader);
    void visit(const std::shared_ptr<StatementTypeSynonym>& statement) override;
    void visit(const std::shared_ptr<StatementLineNumber>& statement) override;
    void visit(const std::shared_ptr<StatementWildcard>& statement) override;
    void setImmediateState(bool onlyImmediate);
    void setParentQueryState(bool queryParents);
};
