#include "Calls.h"

Calls::Calls(const std::shared_ptr<IProcedure>& callingProcedure,
             const std::shared_ptr<IProcedure>& calledProcedure,
             bool hasStar, bool isNegated) : SuchThat(isNegated, hasStar ? 15 : 10) {
    this->callingProcedure = callingProcedure;
    this->calledProcedure = calledProcedure;
    this->hasStar = hasStar;
}

bool Calls::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateCall(reader);
}

bool Calls::evaluateCall(const std::shared_ptr<ICallsReader>& reader) {
    auto callsVisitor = std::make_shared<CallsVisitor>(reader);
    bool hasSynonym = calledProcedure->isSynonym() || callingProcedure->isSynonym();
    callsVisitor->setImmediateState(!hasStar);
    callsVisitor->setCalledQueryState(true);
    callingProcedure->queryAbstractions(callsVisitor);
    auto calledProcedures = callsVisitor->getEntityTwoRelationships();
    bool isNonEmptyIntersection1 = (isNegated && hasSynonym)
        ? calledProcedure->differenceWithEntity(callingProcedure,
            calledProcedures)
        : calledProcedure->intersectWithEntity(callingProcedure,
            calledProcedures);

    callsVisitor->setCalledQueryState(false);
    calledProcedure->queryAbstractions(callsVisitor);
    auto callingProcedures = callsVisitor->getEntityOneRelationships();
    bool isNonEmptyIntersection2 = (isNegated && hasSynonym)
        ? callingProcedure->differenceWithEntity(calledProcedure,
            callingProcedures)
        : callingProcedure->intersectWithEntity(calledProcedure,
            callingProcedures);

    bool truthiness = isNonEmptyIntersection1 && isNonEmptyIntersection2;
    return (isNegated && !hasSynonym) ? !truthiness : truthiness;
}

int Calls::getPriority() {
    int intersectSize1 = callingProcedure->getValueCount() * calledProcedure->getRelationshipCount();
    int intersectSize2 = calledProcedure->getValueCount() * callingProcedure->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
