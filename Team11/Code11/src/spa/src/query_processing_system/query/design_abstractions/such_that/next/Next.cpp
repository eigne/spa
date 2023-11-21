#include "Next.h"

Next::Next(const std::shared_ptr<IStatement>& previousStatement,
           const std::shared_ptr<IStatement>& nextStatement, bool hasStar,
           bool isNegated) : SuchThat(isNegated, hasStar ? 150 : 15) {
    this->previousStatement = previousStatement;
    this->nextStatement = nextStatement;
    this->hasStar = hasStar;
}

bool Next::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateNext(reader);
}

bool Next::evaluateNext(const std::shared_ptr<INextReader>& reader) {
    auto nextVisitor = std::make_shared<NextVisitor>(reader);
    bool hasSynonym = this->previousStatement->isSynonym() || this->nextStatement->isSynonym();
    nextVisitor->setImmediateState(!this->hasStar);

    nextVisitor->setNextQueryState(true);
    previousStatement->queryAbstractions(nextVisitor);
    auto nextStatements = nextVisitor->getStatementTwoRelationships();
    bool isNonEmptyIntersection1 = (isNegated && hasSynonym)
        ? nextStatement->differenceWithStatement(previousStatement,
            nextStatements)
        : nextStatement->intersectWithStatement(previousStatement,
            nextStatements);

    nextVisitor->setNextQueryState(false);
    nextStatement->queryAbstractions(nextVisitor);
    auto previousStatements = nextVisitor->getStatementOneRelationships();
    bool isNonEmptyIntersection2 = (isNegated && hasSynonym)
        ? previousStatement->differenceWithStatement(nextStatement,
            previousStatements)
        : previousStatement->intersectWithStatement(nextStatement,
            previousStatements);

    bool truthiness = isNonEmptyIntersection1 && isNonEmptyIntersection2;
    return (isNegated && !hasSynonym) ? !truthiness : truthiness;
}

int Next::getPriority() {
    int intersectSize1 = nextStatement->getValueCount() * previousStatement->getRelationshipCount();
    int intersectSize2 = previousStatement->getValueCount() * nextStatement->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
