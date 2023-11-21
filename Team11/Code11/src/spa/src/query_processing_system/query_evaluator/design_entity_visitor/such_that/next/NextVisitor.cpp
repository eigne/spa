#include "NextVisitor.h"

NextVisitor::NextVisitor(const std::shared_ptr<INextReader> &nextReader) {
    this->nextReader = nextReader;
    this->onlyQueryImmediate = false;
    this->queryForNext = false;
}

void NextVisitor::setImmediateState(bool onlyImmediate) {
    this->onlyQueryImmediate = onlyImmediate;
}

void NextVisitor::setNextQueryState(bool queryNext) {
    this->queryForNext = queryNext;
}

std::unordered_set<int> NextVisitor::queryBasedOnState(int statementNumber) {
    if (onlyQueryImmediate) {
        if (queryForNext) {
            return nextReader->getNext(statementNumber);
        } else {
            return nextReader->getPrevious(statementNumber);
        }
    } else {
        return nextReader->queryNextStar(statementNumber, {}, queryForNext);
    }
}

void NextVisitor::visit(const std::shared_ptr<StatementTypeSynonym>& statement) {
    statementStatementRelationships = {};
    auto currentPossibilities = statement->getStatementNumbers();
    for (auto thisStatement : currentPossibilities) {
        auto otherStatements = queryBasedOnState(thisStatement);
        for (int otherStatement : otherStatements) {
            if (queryForNext) {
                statementStatementRelationships.emplace_back(thisStatement, otherStatement);
            } else {
                statementStatementRelationships.emplace_back(otherStatement, thisStatement);
            }
        }
    }
}

void NextVisitor::visit(const std::shared_ptr<StatementLineNumber>& statement) {
    statementStatementRelationships = {};
    auto thisStatement = statement->getStatementLineNumber();
    std::unordered_set<int> otherStatements = queryBasedOnState(thisStatement);

    for (int otherStatement : otherStatements) {
        if (queryForNext) {
            statementStatementRelationships.emplace_back(thisStatement, otherStatement);
        } else {
            statementStatementRelationships.emplace_back(otherStatement, thisStatement);
        }
    }
}

void NextVisitor::visit(const std::shared_ptr<StatementWildcard>& statement) {
    statementStatementRelationships = {};
    auto currentPossibilities = statement->getStatementNumbers();

    for (auto thisStatement : currentPossibilities) {
        auto otherStatements = queryBasedOnState(thisStatement);
        for (int otherStatement : otherStatements) {
            if (queryForNext) {
                statementStatementRelationships.emplace_back(thisStatement, otherStatement);
            } else {
                statementStatementRelationships.emplace_back(otherStatement, thisStatement);
            }
        }
    }
}
