#include "IfPatternVisitor.h"

IfPatternVisitor::IfPatternVisitor(const std::shared_ptr<IIfPatternReader> &patternReader) {
    this->patternReader = patternReader;
}

void IfPatternVisitor::visit(const std::shared_ptr<StatementTypeSynonym>& statement) {
    statementEntityRelationships = {};
    auto statementLines = statement->getStatementNumbers();
    for (auto lineNumber : statementLines) {
        auto variables = patternReader->getVariablesUsedInIf(lineNumber);
        for (const auto& variable : variables) {
            statementEntityRelationships.emplace_back(lineNumber, variable);
        }
    }
}

void IfPatternVisitor::visit(const std::shared_ptr<StatementLineNumber>& statement) {
    // will not happen
    statementEntityRelationships = {};
}

void IfPatternVisitor::visit(const std::shared_ptr<StatementWildcard>& statement) {
    // will not happen
    statementEntityRelationships = {};
}

void IfPatternVisitor::visit(const std::shared_ptr<VariableSynonym>& variable) {
    statementEntityRelationships = {};
    auto names = variable->getNames();
    for (const auto& name : names) {
        auto ifs = patternReader->getIfsThatUseVariable(name);
        for (auto lineNumber : ifs) {
            statementEntityRelationships.emplace_back(lineNumber, name);
        }
    }
}

void IfPatternVisitor::visit(const std::shared_ptr<VariableName>& variable) {
    statementEntityRelationships = {};
    auto name = variable->getName();
    auto ifs = patternReader->getIfsThatUseVariable(name);
    for (const auto& lineNumber : ifs) {
        statementEntityRelationships.emplace_back(lineNumber, name);
    }
}

void IfPatternVisitor::visit(const std::shared_ptr<VariableWildcard>& variable) {
    statementEntityRelationships = {};
    auto ifs = patternReader->getIfsThatUseAnyVariable();
    for (const auto& lineNumber : ifs) {
        statementEntityRelationships.emplace_back(lineNumber, "");
    }
}
