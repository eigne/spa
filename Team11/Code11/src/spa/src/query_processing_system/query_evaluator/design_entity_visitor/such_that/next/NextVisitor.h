#pragma once

#include <memory>

#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"
#include \
    "query_processing_system/query_evaluator/design_entity_visitor/interfaces/relationships/StatementStatementRelationship.h"
#include "query_processing_system/query_evaluator/storage_reader/INextReader.h"

class NextVisitor : public IVisitsStatement, public StatementStatementRelationship {
private:
    bool onlyQueryImmediate;
    bool queryForNext;
    std::shared_ptr<INextReader> nextReader;
    std::unordered_set<int> queryBasedOnState(int statementNumber);
    void queryAllNext(int statementNumber, std::unordered_set<int>& visitedStatementsPtr);

public:
    explicit NextVisitor(const std::shared_ptr<INextReader>& nextReader);

    void visit(const std::shared_ptr<StatementTypeSynonym>& statement) override;
    void visit(const std::shared_ptr<StatementLineNumber>& statement) override;
    void visit(const std::shared_ptr<StatementWildcard>& statement) override;
    void setImmediateState(bool onlyImmediate);
    void setNextQueryState(bool queryNext);
};
