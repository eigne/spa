#include "WhilePatternVisitor.h"

WhilePatternVisitor::WhilePatternVisitor(const std::shared_ptr<IWhilePatternReader> &patternReader) {
    this->patternReader = patternReader;
}

void WhilePatternVisitor::visit(const std::shared_ptr<StatementTypeSynonym>& statement) {
    statementEntityRelationships = {};
    auto statementLines = statement->getStatementNumbers();
    for (auto lineNumber : statementLines) {
        auto variables = patternReader->getVariablesUsedInWhile(lineNumber);
        for (const auto& variable : variables) {
            statementEntityRelationships.emplace_back(lineNumber, variable);
        }
    }
}

void WhilePatternVisitor::visit(const std::shared_ptr<StatementLineNumber>& statement) {
    // will not happen
    statementEntityRelationships = {};
}

void WhilePatternVisitor::visit(const std::shared_ptr<StatementWildcard>& statement) {
    // will not happen
    statementEntityRelationships = {};
}

void WhilePatternVisitor::visit(const std::shared_ptr<VariableSynonym>& variable) {
    statementEntityRelationships = {};
    auto names = variable->getNames();
    for (const auto& name : names) {
        auto whiles = patternReader->getWhilesThatUseVariable(name);
        for (auto lineNumber : whiles) {
            statementEntityRelationships.emplace_back(lineNumber, name);
        }
    }
}

void WhilePatternVisitor::visit(const std::shared_ptr<VariableName>& variable) {
    statementEntityRelationships = {};
    auto name = variable->getName();
    auto whiles = patternReader->getWhilesThatUseVariable(name);
    for (const auto& lineNumber : whiles) {
        statementEntityRelationships.emplace_back(lineNumber, name);
    }
}

void WhilePatternVisitor::visit(const std::shared_ptr<VariableWildcard>& variable) {
    statementEntityRelationships = {};
    auto whiles = patternReader->getWhilesThatUseAnyVariable();
    for (const auto& lineNumber : whiles) {
        statementEntityRelationships.emplace_back(lineNumber, "");
    }
}
