#include "FollowsManager.h"

void FollowsManager::clearFollowsStack() {
    FollowsManager::followsStack.clear();
}

void FollowsManager::addNewNestingLevel() {
    FollowsManager::followsStack.emplace_back();
}

void FollowsManager::removeLastNestingLevel() {
    FollowsManager::followsStack.pop_back();
}

void FollowsManager::addStatementToStack(int stmtNum) {
    FollowsManager::followsStack.back().push_back(stmtNum);
}

void FollowsManager::addFollows(int stmtNum, const std::shared_ptr<IRelationshipWriter>& pkb) {
    auto currentNestingLevel = FollowsManager::followsStack.back();

    if (!currentNestingLevel.empty()) {
        pkb->insertFollows(currentNestingLevel.back(), stmtNum);
    }

    for (auto& prevStmt : currentNestingLevel) {
        pkb->insertFollowsT(prevStmt, stmtNum);
    }

    addStatementToStack(stmtNum);
}
