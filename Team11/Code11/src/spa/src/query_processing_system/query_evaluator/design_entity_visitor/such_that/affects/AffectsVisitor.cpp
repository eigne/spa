#include "AffectsVisitor.h"

AffectsVisitor::AffectsVisitor(const std::shared_ptr<IAffectsReader>& affectsReader) {
    this->affectsReader = affectsReader;
    this->queryForAffects = false;
}

void AffectsVisitor::setQueryState(bool state) {
    this->queryForAffects = state;
}

void AffectsVisitor::visit(const std::shared_ptr<StatementTypeSynonym>& statement) {
    statementStatementRelationships = {};
    auto currentPossibilities = statement->getStatementNumbers();

    for (auto thisStatement : currentPossibilities) {
        auto statements = affectsReader->queryAllAffects(thisStatement, queryForAffects);
        for (auto otherStatement : statements) {
            if (queryForAffects)
                statementStatementRelationships.emplace_back(thisStatement, otherStatement);
            else
                statementStatementRelationships.emplace_back(otherStatement, thisStatement);
        }
    }
}

void AffectsVisitor::visit(const std::shared_ptr<StatementLineNumber>& statement) {
    statementStatementRelationships = {};
    auto thisStatement = statement->getStatementLineNumber();

    for (auto otherStatement : affectsReader->queryAllAffects(thisStatement, queryForAffects)) {
        if (queryForAffects)
            statementStatementRelationships.emplace_back(thisStatement, otherStatement);
        else
            statementStatementRelationships.emplace_back(otherStatement, thisStatement);
    }
}

void AffectsVisitor::visit(const std::shared_ptr<StatementWildcard>& statement) {
    statementStatementRelationships = {};
    auto currentPossibilities = statement->getStatementNumbers();

    for (auto thisStatement : currentPossibilities) {
        auto otherStatements = affectsReader->queryAllAffects(thisStatement, queryForAffects);
        for (auto otherStatement : otherStatements) {
            if (queryForAffects)
                statementStatementRelationships.emplace_back(thisStatement, otherStatement);
            else
                statementStatementRelationships.emplace_back(otherStatement, thisStatement);
        }
    }
}

