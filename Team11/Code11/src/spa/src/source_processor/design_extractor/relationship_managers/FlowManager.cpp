#include "FlowManager.h"

std::vector<int> FlowManager::prevStatements;

void FlowManager::addFlow(int stmtIndex, const std::shared_ptr<IRelationshipWriter>& pkb) {
    for (auto& prevStmt : FlowManager::prevStatements) {
        pkb->insertNext(prevStmt, stmtIndex);
    }
}

void FlowManager::resetFlow(int stmtIndex) {
    FlowManager::prevStatements.clear();
    FlowManager::prevStatements.push_back(stmtIndex);
}

void FlowManager::initializeFlowManager() {
    FlowManager::prevStatements.clear();
}
