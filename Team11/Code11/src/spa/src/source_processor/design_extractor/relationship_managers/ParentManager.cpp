#include "ParentManager.h"

void ParentManager::clearParentStack() {
    ParentManager::parentStack.clear();
}

void ParentManager::removeLastParent() {
    ParentManager::parentStack.pop_back();
}

const std::vector<int>& ParentManager::getParentStack() {
    return ParentManager::parentStack;
}

void ParentManager::addStatementToStack(int parent) {
    ParentManager::parentStack.push_back(parent);
}

void ParentManager::addChild(int child, const std::shared_ptr<IRelationshipWriter>& pkb) {
    if (!ParentManager::parentStack.empty()) {
        pkb->insertParents(ParentManager::parentStack.back(), child);
    }

    for (auto& previousStmtNum : ParentManager::parentStack) {
        pkb->insertParentsT(previousStmtNum, child);
    }
}
