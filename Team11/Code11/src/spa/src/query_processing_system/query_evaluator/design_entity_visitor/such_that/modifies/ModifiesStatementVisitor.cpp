#include "ModifiesStatementVisitor.h"

ModifiesStatementVisitor::ModifiesStatementVisitor(const std::shared_ptr<IModifiesStatementReader> &modifiesReader) {
    this->modifiesReader = modifiesReader;
}

void ModifiesStatementVisitor::visit(const std::shared_ptr<StatementTypeSynonym>& statement) {
    statementEntityRelationships = {};
    auto currentPossibilities = statement->getStatementNumbers();
    for (auto statementNumber : currentPossibilities) {
        auto variables = modifiesReader->getVariablesModifiedInStatement(statementNumber);
        for (const auto& variable : variables) {
            statementEntityRelationships.emplace_back(statementNumber, variable);
        }
    }
}

void ModifiesStatementVisitor::visit(const std::shared_ptr<StatementLineNumber>& statement) {
    statementEntityRelationships = {};
    auto statementNumber = statement->getStatementLineNumber();
    auto variables = modifiesReader->getVariablesModifiedInStatement(statementNumber);
    for (const auto& variable : variables) {
        statementEntityRelationships.emplace_back(statementNumber, variable);
    }
}

void ModifiesStatementVisitor::visit(const std::shared_ptr<StatementWildcard>& statement) {
    // invalid clause: statement of modifies cannot be wildcard
    statementEntityRelationships = {};
}

void ModifiesStatementVisitor::visit(const std::shared_ptr<VariableSynonym>& variable) {
    statementEntityRelationships = {};
    auto currentPossibilities = variable->getNames();
    for (const auto& name : currentPossibilities) {
        auto statements = modifiesReader->getStatementsThatModifyVariable(name);
        for (const auto& statement : statements) {
            statementEntityRelationships.emplace_back(statement, name);
        }
    }
}

void ModifiesStatementVisitor::visit(const std::shared_ptr<VariableName>& variable) {
    statementEntityRelationships = {};
    auto name = variable->getName();
    auto statements = modifiesReader->getStatementsThatModifyVariable(name);
    for (const auto& statement : statements) {
        statementEntityRelationships.emplace_back(statement, name);
    }
}

void ModifiesStatementVisitor::visit(const std::shared_ptr<VariableWildcard>& variable) {
    statementEntityRelationships = {};
    auto statements = modifiesReader->getStatementsThatModifyAnyVariable();
    for (const auto& statement : statements) {
        statementEntityRelationships.emplace_back(statement, "");
    }
}
