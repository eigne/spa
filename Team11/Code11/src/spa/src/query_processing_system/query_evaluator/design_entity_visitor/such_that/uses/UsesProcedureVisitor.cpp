#include "UsesProcedureVisitor.h"

UsesProcedureVisitor::UsesProcedureVisitor(const std::shared_ptr<IUsesProcedureReader> &usesReader) {
    this->usesReader = usesReader;
}

void UsesProcedureVisitor::visit(const std::shared_ptr<ProcedureSynonym>& procedure) {
    entityEntityRelationships = {};
    auto currentPossibilities = procedure->getNames();
    for (const auto& name : currentPossibilities) {
        auto variables = usesReader->getVariablesUsedInProcedure(name);
        for (const auto& variable : variables) {
            entityEntityRelationships.emplace_back(name, variable);
        }
    }
}

void UsesProcedureVisitor::visit(const std::shared_ptr<ProcedureName>& procedure) {
    entityEntityRelationships = {};
    auto name = procedure->getName();
    auto variables = usesReader->getVariablesUsedInProcedure(name);
    for (const auto& variable : variables) {
        entityEntityRelationships.emplace_back(name, variable);
    }
}

void UsesProcedureVisitor::visit(const std::shared_ptr<VariableSynonym>& variable) {
    entityEntityRelationships = {};
    auto currentPossibilities = variable->getNames();
    for (const auto& name : currentPossibilities) {
        auto procedures = usesReader->getProceduresThatUseVariable(name);
        for (const auto& procedure : procedures) {
            entityEntityRelationships.emplace_back(procedure, name);
        }
    }
}

void UsesProcedureVisitor::visit(const std::shared_ptr<ProcedureWildcard> &statement) {
    // shouldn't happen
}

void UsesProcedureVisitor::visit(const std::shared_ptr<VariableName>& variable) {
    entityEntityRelationships = {};
    auto name = variable->getName();
    auto procedures = usesReader->getProceduresThatUseVariable(name);
    for (const auto& procedure : procedures) {
        entityEntityRelationships.emplace_back(procedure, name);
    }
}

void UsesProcedureVisitor::visit(const std::shared_ptr<VariableWildcard>& variable) {
    entityEntityRelationships = {};
    auto procedures = usesReader->getProceduresThatUseAnyVariable();
    for (const auto& procedure : procedures) {
        entityEntityRelationships.emplace_back(procedure, "");
    }
}
