#pragma once

#include <memory>
#include <vector>
#include <string>

#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsVariable.h"
#include \
    "query_processing_system/query_evaluator/design_entity_visitor/interfaces/relationships/StatementEntityRelationship.h"
#include "query_processing_system/query_evaluator/storage_reader/IAssignPatternReader.h"


class AssignPatternVisitor : public IVisitsStatement, public IVisitsVariable, public StatementEntityRelationship {
private:
    std::shared_ptr<IAssignPatternReader> patternReader;
    bool hasWildcard;
    std::vector<std::string> postfixExpression;
public:
    explicit AssignPatternVisitor(const std::shared_ptr<IAssignPatternReader>& patternReader);
    void setPatternState(bool wildcard, const std::vector<std::string>& expression);

    void visit(const std::shared_ptr<StatementTypeSynonym>& statement) override;
    void visit(const std::shared_ptr<StatementLineNumber>& statement) override;
    void visit(const std::shared_ptr<StatementWildcard>& statement) override;

    void visit(const std::shared_ptr<VariableSynonym>& variable) override;
    void visit(const std::shared_ptr<VariableName>& variable) override;
    void visit(const std::shared_ptr<VariableWildcard>& variable) override;
};
