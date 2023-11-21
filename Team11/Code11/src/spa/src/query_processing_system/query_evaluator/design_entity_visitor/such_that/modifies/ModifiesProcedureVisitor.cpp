#include "ModifiesProcedureVisitor.h"

ModifiesProcedureVisitor::ModifiesProcedureVisitor(const std::shared_ptr<IModifiesProcedureReader> &modifiesReader) {
    this->modifiesReader = modifiesReader;
}

void ModifiesProcedureVisitor::visit(const std::shared_ptr<ProcedureSynonym>& procedure) {
    entityEntityRelationships = {};
    auto currentPossibilities = procedure->getNames();
    for (const auto& procName : currentPossibilities) {
        auto variables = modifiesReader->getVariablesModifiedInProcedure(procName);
        for (const auto& variable : variables) {
            entityEntityRelationships.emplace_back(procName, variable);
        }
    }
}

void ModifiesProcedureVisitor::visit(const std::shared_ptr<ProcedureName>& procedure) {
    entityEntityRelationships = {};
    auto procName = procedure->getName();
    auto variables = modifiesReader->getVariablesModifiedInProcedure(procName);
    for (const auto& variable : variables) {
        entityEntityRelationships.emplace_back(procName, variable);
    }
}

void ModifiesProcedureVisitor::visit(const std::shared_ptr<ProcedureWildcard> &statement) {
    // shouldn't happen
}

void ModifiesProcedureVisitor::visit(const std::shared_ptr<VariableSynonym>& variable) {
    entityEntityRelationships = {};
    auto currentPossibilities = variable->getNames();
    for (const auto& name : currentPossibilities) {
        auto procedures = modifiesReader->getProceduresThatModifyVariable(name);
        for (const auto& procedure : procedures) {
            entityEntityRelationships.emplace_back(procedure, name);
        }
    }
}

void ModifiesProcedureVisitor::visit(const std::shared_ptr<VariableName>& variable) {
    entityEntityRelationships = {};
    auto name = variable->getName();
    auto procedures = modifiesReader->getProceduresThatModifyVariable(name);
    for (const auto& procedure : procedures) {
        entityEntityRelationships.emplace_back(procedure, name);
    }
}

void ModifiesProcedureVisitor::visit(const std::shared_ptr<VariableWildcard>& variable) {
    entityEntityRelationships = {};
    auto procedures = modifiesReader->getProceduresThatModifyAnyVariable();
    for (const auto& procedure : procedures) {
        entityEntityRelationships.emplace_back(procedure, "");
    }
}
