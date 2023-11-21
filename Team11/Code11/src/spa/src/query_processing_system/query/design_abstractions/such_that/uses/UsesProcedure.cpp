#include "UsesProcedure.h"

UsesProcedure::UsesProcedure(const std::shared_ptr<IProcedure> &procedureThatUses,
                             const std::shared_ptr<IVariable> &variableUsed,
                             bool isNegated) : SuchThat(isNegated, 100) {
    this->procedureThatUses = procedureThatUses;
    this->variableUsed = variableUsed;
}

bool UsesProcedure::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateUsesProcedure(reader);
}

bool UsesProcedure::evaluateUsesProcedure(const std::shared_ptr<IUsesProcedureReader>& reader) {
    auto usesProcedureVisitor = std::make_shared<UsesProcedureVisitor>(reader);
    bool hasSynonym = procedureThatUses->isSynonym() || variableUsed->isSynonym();
    variableUsed->queryAbstractions(usesProcedureVisitor);
    auto procedures = usesProcedureVisitor->getEntityOneRelationships();
    bool isNonEmptyIntersection1 = (isNegated && hasSynonym)
        ? procedureThatUses->differenceWithEntity(variableUsed,
            procedures)
        : procedureThatUses->intersectWithEntity(variableUsed,
            procedures);

    procedureThatUses->queryAbstractions(usesProcedureVisitor);
    auto variables = usesProcedureVisitor->getEntityTwoRelationships();
    bool isNonEmptyIntersection2 = (isNegated && hasSynonym)
        ? variableUsed->differenceWithEntity(procedureThatUses,
            variables)
        : variableUsed->intersectWithEntity(procedureThatUses,
            variables);
    bool truthiness = isNonEmptyIntersection1 && isNonEmptyIntersection2;
    return (isNegated && !hasSynonym) ? !truthiness : truthiness;
}

int UsesProcedure::getPriority() {
    int intersectSize1 = variableUsed->getValueCount() * procedureThatUses->getRelationshipCount();
    int intersectSize2 = procedureThatUses->getValueCount() * variableUsed->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
