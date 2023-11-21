#include "UsesStatementVisitor.h"

UsesStatementVisitor::UsesStatementVisitor(const std::shared_ptr<IUsesStatementReader> &usesReader) {
    this->usesReader = usesReader;
}

void UsesStatementVisitor::visit(const std::shared_ptr<StatementTypeSynonym>& statement) {
    statementEntityRelationships = {};
    auto currentPossibilities = statement->getStatementNumbers();
    for (auto statementNumber : currentPossibilities) {
        auto variables = usesReader->getVariablesUsedInStatement(statementNumber);
        for (const auto& variable : variables) {
            statementEntityRelationships.emplace_back(statementNumber, variable);
        }
    }
}

void UsesStatementVisitor::visit(const std::shared_ptr<StatementLineNumber>& statement) {
    statementEntityRelationships = {};
    auto lineNumber = statement->getStatementLineNumber();
    auto variables = usesReader->getVariablesUsedInStatement(lineNumber);
    for (const auto& variable : variables) {
        statementEntityRelationships.emplace_back(lineNumber, variable);
    }
}

void UsesStatementVisitor::visit(const std::shared_ptr<StatementWildcard>& statement) {
    // invalid clause: statement of uses cannot be wildcard
    statementEntityRelationships = {};
}

void UsesStatementVisitor::visit(const std::shared_ptr<VariableSynonym>& variable) {
    statementEntityRelationships = {};
    auto currentPossibilities = variable->getNames();
    for (const auto& name : currentPossibilities) {
        auto statements = usesReader->getStatementsThatUseVariable(name);
        for (const auto &statement : statements) {
            statementEntityRelationships.emplace_back(statement, name);
        }
    }
}

void UsesStatementVisitor::visit(const std::shared_ptr<VariableName>& variable) {
    statementEntityRelationships = {};
    auto name = variable->getName();
    auto statements = usesReader->getStatementsThatUseVariable(name);
    for (const auto &statement : statements) {
        statementEntityRelationships.emplace_back(statement, name);
    }
}

void UsesStatementVisitor::visit(const std::shared_ptr<VariableWildcard>& variable) {
    statementEntityRelationships = {};
    auto statements = usesReader->getStatementsThatUseAnyVariable();
    for (const auto &statement : statements) {
        statementEntityRelationships.emplace_back(statement, "");
    }
}
