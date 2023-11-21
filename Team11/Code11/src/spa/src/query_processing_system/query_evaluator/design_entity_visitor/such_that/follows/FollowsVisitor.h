#pragma once

#include <memory>

#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"
#include \
    "query_processing_system/query_evaluator/design_entity_visitor/interfaces/relationships/StatementStatementRelationship.h"
#include "query_processing_system/query_evaluator/storage_reader/IFollowsReader.h"

class FollowsVisitor : public IVisitsStatement, public StatementStatementRelationship {
private:
    bool onlyQueryImmediate;
    bool queryForFollows;
    std::shared_ptr<IFollowsReader> followsReader;
    std::unordered_set<int> queryBasedOnState(int statementNumber);
public:
    explicit FollowsVisitor(const std::shared_ptr<IFollowsReader>& followsReader);

    void visit(const std::shared_ptr<StatementTypeSynonym>& statement) override;
    void visit(const std::shared_ptr<StatementLineNumber>& statement) override;
    void visit(const std::shared_ptr<StatementWildcard>& statement) override;
    void setImmediateState(bool onlyImmediate);
    void setFollowsQueryState(bool queryFollows);
};
