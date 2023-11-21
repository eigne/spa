#include "CallsVisitor.h"

CallsVisitor::CallsVisitor(const std::shared_ptr<ICallsReader> &callsReader) {
    this->callsReader = callsReader;
    this->onlyQueryImmediate = false;
    this->queryForCalled = false;
}

void CallsVisitor::setImmediateState(bool onlyImmediate) {
    this->onlyQueryImmediate = onlyImmediate;
}

void CallsVisitor::setCalledQueryState(bool queryCalled) {
    this->queryForCalled = queryCalled;
}

std::unordered_set<std::string> CallsVisitor::queryBasedOnState(const std::string& procedureName) {
    if (queryForCalled) {
        return callsReader->getProceduresCalledBy(procedureName, onlyQueryImmediate);
    } else {
        return callsReader->getProceduresCalling(procedureName, onlyQueryImmediate);
    }
}

void CallsVisitor::visit(const std::shared_ptr<ProcedureSynonym>& procedure) {
    entityEntityRelationships = {};
    auto currentPossibilities = procedure->getNames();
    for (const auto& thisProcedure : currentPossibilities) {
        auto procedures = queryBasedOnState(thisProcedure);
        for (const auto& otherProcedure : procedures) {
            if (queryForCalled) { // other is the called
                entityEntityRelationships.emplace_back(thisProcedure, otherProcedure);
            } else {
                entityEntityRelationships.emplace_back(otherProcedure, thisProcedure);
            }
        }
    }
}

void CallsVisitor::visit(const std::shared_ptr<ProcedureName>& procedure) {
    entityEntityRelationships = {};
    auto thisProcedure = procedure->getName();
    auto procedures = queryBasedOnState(thisProcedure);
    for (const auto& otherProcedure : procedures) {
        if (queryForCalled) { // other is the follow
            entityEntityRelationships.emplace_back(thisProcedure, otherProcedure);
        } else {
            entityEntityRelationships.emplace_back(otherProcedure, thisProcedure);
        }
    }
}

void CallsVisitor::visit(const std::shared_ptr<ProcedureWildcard>& procedure) {
    entityEntityRelationships = {};
    if (queryForCalled) {
        for (const auto& otherProcedure : callsReader->getAllCalledProcedures()) {
            entityEntityRelationships.emplace_back("", otherProcedure);
        }
    } else {
        for (const auto& otherProcedure : callsReader->getProceduresThatCall()) {
            entityEntityRelationships.emplace_back(otherProcedure, "");
        }
    }
}
