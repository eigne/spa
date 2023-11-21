#include "ModifiesStatement.h"

ModifiesStatement::ModifiesStatement(const std::shared_ptr<IStatement> &statementThatModifies,
                                     const std::shared_ptr<IVariable> &variableModified,
                                     bool isNegated) : SuchThat(isNegated, 11) {
    this->statementThatModifies = statementThatModifies;
    this->variableModified = variableModified;
}

bool ModifiesStatement::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateModifiesStatement(reader);
}

bool ModifiesStatement::evaluateModifiesStatement(const std::shared_ptr<IModifiesStatementReader>& reader) {
    auto modifiesStatementVisitor = std::make_shared<ModifiesStatementVisitor>(reader);
    bool hasSynonym = statementThatModifies->isSynonym() || variableModified->isSynonym();
    variableModified->queryAbstractions(modifiesStatementVisitor);
    auto statements = modifiesStatementVisitor->getStatementRelationships();
    bool isNonEmptyIntersection1 = (isNegated && hasSynonym)
        ? statementThatModifies->differenceWithEntity(
            variableModified,
            statements)
        : statementThatModifies->intersectWithEntity(
            variableModified,
            statements);

    statementThatModifies->queryAbstractions(modifiesStatementVisitor);
    auto variables = modifiesStatementVisitor->getEntityRelationships();
    bool isNonEmptyIntersection2 = (isNegated && hasSynonym)
        ? variableModified->differenceWithStatement(
            statementThatModifies,
            variables)
        : variableModified->intersectWithStatement(
            statementThatModifies,
            variables);

    bool truthiness = isNonEmptyIntersection1 && isNonEmptyIntersection2;
    return (isNegated && !hasSynonym) ? !truthiness : truthiness;
}

int ModifiesStatement::getPriority() {
    int intersectSize1 = variableModified->getValueCount() * statementThatModifies->getRelationshipCount();
    int intersectSize2 = statementThatModifies->getValueCount() * variableModified->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
