#include "AssignPatternVisitor.h"

AssignPatternVisitor::AssignPatternVisitor(const std::shared_ptr<IAssignPatternReader> &patternReader) {
    this->patternReader = patternReader;
    this->hasWildcard = true;
}

void AssignPatternVisitor::setPatternState(bool wildcard, const std::vector<std::string>& expression) {
    this->hasWildcard = wildcard;
    this->postfixExpression = expression;
}

void AssignPatternVisitor::visit(const std::shared_ptr<StatementTypeSynonym>& statement) {
    statementEntityRelationships = {};
    auto statementLines = statement->getStatementNumbers();
    for (auto lineNumber : statementLines) {
        auto variables = patternReader->getVariablesModifiedInStatement(lineNumber);
        bool match = patternReader->hasMatchingPattern(lineNumber, postfixExpression, !hasWildcard);
        if (match) {
            for (const auto& variable : variables) {
                statementEntityRelationships.emplace_back(lineNumber, variable);
            }
        }
    }
}

void AssignPatternVisitor::visit(const std::shared_ptr<StatementLineNumber>& statement) {
    // will not happen
    statementEntityRelationships = {};
}

void AssignPatternVisitor::visit(const std::shared_ptr<StatementWildcard>& statement) {
    // will not happen
    statementEntityRelationships = {};
}

void AssignPatternVisitor::visit(const std::shared_ptr<VariableSynonym>& variable) {
    statementEntityRelationships = {};
    auto variableNames = variable->getNames();
    for (const auto& name : variableNames) {
        auto statementLines = patternReader->getStatementsThatModifyVariable(name);

        for (auto lineNumber : statementLines) {
            bool match = patternReader->hasMatchingPattern(lineNumber, postfixExpression, !hasWildcard);
            if (match) {
                statementEntityRelationships.emplace_back(lineNumber, name);
            }
        }
    }
}

void AssignPatternVisitor::visit(const std::shared_ptr<VariableName>& variable) {
    statementEntityRelationships = {};
    auto name = variable->getName();
    auto statementLines = patternReader->getStatementsThatModifyVariable(name);

    for (auto lineNumber : statementLines) {
        bool match = patternReader->hasMatchingPattern(lineNumber, postfixExpression, !hasWildcard);
        if (match) {
            statementEntityRelationships.emplace_back(lineNumber, name);
        }
    }
}

void AssignPatternVisitor::visit(const std::shared_ptr<VariableWildcard>& variable) {
    statementEntityRelationships = {};
    auto statementLines = patternReader->getStatementsThatModifyAnyVariable();

    for (auto lineNumber : statementLines) {
        bool match = patternReader->hasMatchingPattern(lineNumber, postfixExpression, !hasWildcard);
        if (match) {
            statementEntityRelationships.emplace_back(lineNumber, "");
        }
    }
}
