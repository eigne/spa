#include "FollowsVisitor.h"

FollowsVisitor::FollowsVisitor(const std::shared_ptr<IFollowsReader> &followsReader) {
    this->followsReader = followsReader;
    this->onlyQueryImmediate = false;
    this->queryForFollows = false;
}

void FollowsVisitor::setImmediateState(bool onlyImmediate) {
    this->onlyQueryImmediate = onlyImmediate;
}

void FollowsVisitor::setFollowsQueryState(bool queryFollows) {
    this->queryForFollows = queryFollows;
}

std::unordered_set<int> FollowsVisitor::queryBasedOnState(int statementNumber) {
    if (queryForFollows) {
        return followsReader->getFollows(statementNumber, onlyQueryImmediate);
    } else {
        return followsReader->getBefore(statementNumber, onlyQueryImmediate);
    }
}

void FollowsVisitor::visit(const std::shared_ptr<StatementTypeSynonym>& statement) {
    statementStatementRelationships = {};
    auto currentPossibilities = statement->getStatementNumbers();
    for (auto thisStatement : currentPossibilities) {
        auto statements = queryBasedOnState(thisStatement);
        for (auto otherStatement : statements) {
            if (queryForFollows) { // other is the follow
                statementStatementRelationships.emplace_back(thisStatement, otherStatement);
            } else {
                statementStatementRelationships.emplace_back(otherStatement, thisStatement);
            }
        }
    }
}

void FollowsVisitor::visit(const std::shared_ptr<StatementLineNumber>& statement) {
    statementStatementRelationships = {};
    auto thisStatement = statement->getStatementLineNumber();
    std::unordered_set<int> res = queryBasedOnState(thisStatement);
    for (auto otherStatement : res) {
        if (queryForFollows) { // other is the follow
            statementStatementRelationships.emplace_back(thisStatement, otherStatement);
        } else {
            statementStatementRelationships.emplace_back(otherStatement, thisStatement);
        }
    }
}

void FollowsVisitor::visit(const std::shared_ptr<StatementWildcard>& statement) {
    statementStatementRelationships = {};
    if (queryForFollows) {
        // getAllBefore equivalent is getAllWithFollows, other is the follow
        for (auto otherStatement : followsReader->getAllWithBefore()) {
            statementStatementRelationships.emplace_back(0, otherStatement);
        }
    } else {
        for (auto otherStatement : followsReader->getAllWithFollows()) {
            statementStatementRelationships.emplace_back(otherStatement, 0);
        }
    }
}
