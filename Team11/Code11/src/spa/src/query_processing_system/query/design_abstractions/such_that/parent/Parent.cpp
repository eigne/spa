#include "Parent.h"

Parent::Parent(const std::shared_ptr<IStatement> &parentStatement,
               const std::shared_ptr<IStatement> &childStatement,
               bool hasStar, bool isNegated) : SuchThat(isNegated, hasStar ? 15 : 10) {
    this->parentStatement = parentStatement;
    this->childStatement = childStatement;
    this->hasStar = hasStar;
}

bool Parent::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateParent(reader);
}

bool Parent::evaluateParent(const std::shared_ptr<IParentReader>& reader) {
    auto parentVisitor = std::make_shared<ParentVisitor>(reader);
    bool hasSynonym = childStatement->isSynonym() || parentStatement->isSynonym();
    parentVisitor->setImmediateState(!hasStar);
    parentVisitor->setParentQueryState(true);
    childStatement->queryAbstractions(parentVisitor);
    auto parentStatements = parentVisitor->getStatementOneRelationships();
    bool isNonEmptyIntersection1 = (isNegated && hasSynonym)
        ? parentStatement->differenceWithStatement(childStatement,
            parentStatements)
        : parentStatement->intersectWithStatement(childStatement,
            parentStatements);

    parentVisitor->setParentQueryState(false);
    parentStatement->queryAbstractions(parentVisitor);
    auto childStatements = parentVisitor->getStatementTwoRelationships();
    bool isNonEmptyIntersection2 = (isNegated && hasSynonym)
        ? childStatement->differenceWithStatement(parentStatement,
            childStatements)
        : childStatement->intersectWithStatement(parentStatement,
            childStatements);
    bool truthiness = isNonEmptyIntersection1 && isNonEmptyIntersection2;
    return (isNegated && !hasSynonym) ? !truthiness : truthiness;
}

int Parent::getPriority() {
    int intersectSize1 = parentStatement->getValueCount() * childStatement->getRelationshipCount();
    int intersectSize2 = childStatement->getValueCount() * parentStatement->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
