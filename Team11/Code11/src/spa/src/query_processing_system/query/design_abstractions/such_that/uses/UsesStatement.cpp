#include "UsesStatement.h"

UsesStatement::UsesStatement(const std::shared_ptr<IStatement> &statementThatUses,
                             const std::shared_ptr<IVariable> &variableUsed,
                             bool isNegated) : SuchThat(isNegated, 13) {
    this->statementThatUses = statementThatUses;
    this->variableUsed = variableUsed;
}

bool UsesStatement::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateUsesStatement(reader);
}

bool UsesStatement::evaluateUsesStatement(const std::shared_ptr<IUsesStatementReader>& reader) {
    auto usesStatementVisitor = std::make_shared<UsesStatementVisitor>(reader);
    bool hasSynonym = statementThatUses->isSynonym() || variableUsed->isSynonym();
    variableUsed->queryAbstractions(usesStatementVisitor);
    auto statements = usesStatementVisitor->getStatementRelationships();
    bool isNonEmptyIntersection1 = (isNegated && hasSynonym)
        ? statementThatUses->differenceWithEntity(variableUsed,
            statements)
        : statementThatUses->intersectWithEntity(variableUsed,
            statements);

    statementThatUses->queryAbstractions(usesStatementVisitor);
    auto variables = usesStatementVisitor->getEntityRelationships();
    bool isNonEmptyIntersection2 = (isNegated && hasSynonym)
        ? variableUsed->differenceWithStatement(
            statementThatUses,
            variables)
        : variableUsed->intersectWithStatement(
            statementThatUses,
            variables);

    bool truthiness = isNonEmptyIntersection1 && isNonEmptyIntersection2;
    return (isNegated && !hasSynonym) ? !truthiness : truthiness;
}

int UsesStatement::getPriority() {
    int intersectSize1 = variableUsed->getValueCount() * statementThatUses->getRelationshipCount();
    int intersectSize2 = statementThatUses->getValueCount() * variableUsed->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
