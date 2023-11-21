#include "ParentVisitor.h"

ParentVisitor::ParentVisitor(const std::shared_ptr<IParentReader> &parentReader) {
    this->parentReader = parentReader;
    this->onlyQueryImmediate = false;
    this->queryForParents = false;
}

void ParentVisitor::setImmediateState(bool onlyImmediate) {
    this->onlyQueryImmediate = onlyImmediate;
}

void ParentVisitor::setParentQueryState(bool queryParents) {
    this->queryForParents = queryParents;
}

std::unordered_set<int> ParentVisitor::queryBasedOnState(int statementNumber) {
    if (queryForParents) {
        return parentReader->getParent(statementNumber, onlyQueryImmediate);
    } else {
        return parentReader->getChild(statementNumber, onlyQueryImmediate);
    }
}

void ParentVisitor::visit(const std::shared_ptr<StatementTypeSynonym>& statement) {
    statementStatementRelationships = {};
    auto currentPossibilities = statement->getStatementNumbers();
    for (auto thisStatement : currentPossibilities) {
        auto statements = queryBasedOnState(thisStatement);
        for (auto otherStatement : statements) {
            if (queryForParents) {
                statementStatementRelationships.emplace_back(otherStatement, thisStatement);
            } else {
                statementStatementRelationships.emplace_back(thisStatement, otherStatement);
            }
        }
    }
}

void ParentVisitor::visit(const std::shared_ptr<StatementLineNumber>& statement) {
    statementStatementRelationships = {};
    auto thisStatement = statement->getStatementLineNumber();
    std::unordered_set<int> res = queryBasedOnState(thisStatement);
    for (auto otherStatement : res) {
        if (queryForParents) {
            statementStatementRelationships.emplace_back(otherStatement, thisStatement);
        } else {
            statementStatementRelationships.emplace_back(thisStatement, otherStatement);
        }
    }
}

void ParentVisitor::visit(const std::shared_ptr<StatementWildcard>& statement) {
    statementStatementRelationships = {};
    if (queryForParents) {
        // getAllParents equivalent is getAllWithChild
        for (auto otherStatement : parentReader->getAllWithChild()) {
            statementStatementRelationships.emplace_back(otherStatement, 0);
        }
    } else {
        for (auto otherStatement : parentReader->getAllWithParent()) {
            statementStatementRelationships.emplace_back(0, otherStatement);
        }
    }
}
