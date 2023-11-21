#include "Affects.h"

Affects::Affects(const std::shared_ptr<IStatement>& affectingStatement,
       const std::shared_ptr<IStatement>& affectedStatement, bool isNegated) : SuchThat(isNegated, 11) {
    this->affectingStatement = affectingStatement;
    this->affectedStatement = affectedStatement;
}

bool Affects::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateAffects(reader);
}

bool Affects::evaluateAffects(const std::shared_ptr<IAffectsReader>& reader) {
    auto affectsVisitor = std::make_shared<AffectsVisitor>(reader);
    bool hasSynonym = affectingStatement->isSynonym() || affectedStatement->isSynonym();
    affectsVisitor->setQueryState(true);
    affectingStatement->queryAbstractions(affectsVisitor);
    auto affectedStatements = affectsVisitor->getStatementTwoRelationships();
    bool isNonEmptyIntersection1 = (isNegated && hasSynonym)
        ? affectedStatement->differenceWithStatement(affectingStatement,
                       affectedStatements)
        : affectedStatement->intersectWithStatement(affectingStatement,
                       affectedStatements);

    affectsVisitor->setQueryState(false);
    affectedStatement->queryAbstractions(affectsVisitor);
    auto affectingStatements = affectsVisitor->getStatementOneRelationships();
    bool isNonEmptyIntersection2 = (isNegated && hasSynonym)
        ? affectingStatement->differenceWithStatement(affectedStatement,
                                  affectingStatements)
        : affectingStatement->intersectWithStatement(affectedStatement,
                                  affectingStatements);
    bool truthiness = isNonEmptyIntersection1 && isNonEmptyIntersection2;
    return (isNegated && !hasSynonym) ? !truthiness : truthiness;
}

int Affects::getPriority() {
    int intersectSize1 = affectedStatement->getValueCount() * affectingStatement->getRelationshipCount();
    int intersectSize2 = affectingStatement->getValueCount() * affectedStatement->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
