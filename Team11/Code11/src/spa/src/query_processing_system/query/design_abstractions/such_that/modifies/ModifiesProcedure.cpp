#include "ModifiesProcedure.h"

ModifiesProcedure::ModifiesProcedure(const std::shared_ptr<IProcedure> &procedureThatModifies,
                                     const std::shared_ptr<IVariable> &variableModified,
                                     bool isNegated) : SuchThat(isNegated, 30) {
    this->procedureThatModifies = procedureThatModifies;
    this->variableModified = variableModified;
}

bool ModifiesProcedure::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateModifiesProcedure(reader);
}

bool ModifiesProcedure::evaluateModifiesProcedure(const std::shared_ptr<IModifiesProcedureReader>& reader) {
    auto modifiesProcedureVisitor = std::make_shared<ModifiesProcedureVisitor>(reader);
    bool hasSynonym = procedureThatModifies->isSynonym() || variableModified->isSynonym();
    variableModified->queryAbstractions(modifiesProcedureVisitor);
    auto procedures = modifiesProcedureVisitor->getEntityOneRelationships();
    bool isNonEmptyIntersection1 = (isNegated && hasSynonym)
        ? procedureThatModifies->differenceWithEntity(
            variableModified,
            procedures)
        : procedureThatModifies->intersectWithEntity(
            variableModified,
            procedures);

    procedureThatModifies->queryAbstractions(modifiesProcedureVisitor);
    auto variables = modifiesProcedureVisitor->getEntityTwoRelationships();
    bool isNonEmptyIntersection2 = (isNegated && hasSynonym)
        ? variableModified->differenceWithEntity(
            procedureThatModifies,
            variables)
        : variableModified->intersectWithEntity(
            procedureThatModifies,
            variables);

    bool truthiness = isNonEmptyIntersection1 && isNonEmptyIntersection2;
    return (isNegated && !hasSynonym) ? !truthiness : truthiness;
}

int ModifiesProcedure::getPriority() {
    int intersectSize1 = variableModified->getValueCount() * procedureThatModifies->getRelationshipCount();
    int intersectSize2 = procedureThatModifies->getValueCount() * variableModified->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
