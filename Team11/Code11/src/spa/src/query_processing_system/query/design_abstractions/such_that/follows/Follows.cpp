#include "Follows.h"

Follows::Follows(const std::shared_ptr<IStatement>& beforeStatement,
                 const std::shared_ptr<IStatement>& afterStatement,
                 bool hasStar, bool isNegated) : SuchThat(isNegated, hasStar ? 80 : 10) {
    this->beforeStatement = beforeStatement;
    this->afterStatement = afterStatement;
    this->hasStar = hasStar;
}

bool Follows::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateFollows(reader);
}

bool Follows::evaluateFollows(const std::shared_ptr<IFollowsReader>& reader) {
    auto followsVisitor = std::make_shared<FollowsVisitor>(reader);
    bool hasSynonym = beforeStatement->isSynonym() || afterStatement->isSynonym();
    followsVisitor->setImmediateState(!hasStar);
    followsVisitor->setFollowsQueryState(true);
    beforeStatement->queryAbstractions(followsVisitor);
    auto followingStatements = followsVisitor->getStatementTwoRelationships();
    bool isNonEmptyIntersection1 = (isNegated && hasSynonym)
        ? afterStatement->differenceWithStatement(beforeStatement,
            followingStatements)
        : afterStatement->intersectWithStatement(beforeStatement,
            followingStatements);

    followsVisitor->setFollowsQueryState(false);
    afterStatement->queryAbstractions(followsVisitor);
    auto beforeStatements = followsVisitor->getStatementOneRelationships();
    bool isNonEmptyIntersection2 = (isNegated && hasSynonym)
        ? beforeStatement->differenceWithStatement(afterStatement,
            beforeStatements)
        : beforeStatement->intersectWithStatement(afterStatement,
            beforeStatements);

    bool truthiness = isNonEmptyIntersection1 && isNonEmptyIntersection2;
    return (isNegated && !hasSynonym) ? !truthiness : truthiness;
}

int Follows::getPriority() {
    int intersectSize1 = beforeStatement->getValueCount() * afterStatement->getRelationshipCount();
    int intersectSize2 = afterStatement->getValueCount() * beforeStatement->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
