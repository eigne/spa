#include "AssignPattern.h"

AssignPattern::AssignPattern(const std::shared_ptr<AssignSynonym>& assignment,
                             const std::shared_ptr<IVariable>& variable,
                             const std::string& expression,
                             bool hasWildcard, bool isNegated) : Pattern(isNegated, 13) {
    this->assignment = assignment;
    this->variable = variable;
    this->postfixExpression = ExpressionParser::convertToPostfix(expression);
    this->hasWildcard = hasWildcard;
}

bool AssignPattern::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateAssignPattern(reader);
}

bool AssignPattern::evaluateAssignPattern(const std::shared_ptr<IAssignPatternReader>& reader) {
    auto assignPatternVisitor = std::make_shared<AssignPatternVisitor>(reader);
    assignPatternVisitor->setPatternState(hasWildcard, postfixExpression);
    assignment->queryAbstractions(assignPatternVisitor);
    auto variables = assignPatternVisitor->getEntityRelationships();
    bool isNonEmptyIntersection1 = isNegated
        ? variable->differenceWithStatement(assignment, variables)
        : variable->intersectWithStatement(assignment, variables);

    variable->queryAbstractions(assignPatternVisitor);
    auto statements = assignPatternVisitor->getStatementRelationships();
    bool isNonEmptyIntersection2 = isNegated
        ? assignment->differenceWithEntity(variable, statements)
        : assignment->intersectWithEntity(variable, statements);

    return isNonEmptyIntersection1 && isNonEmptyIntersection2;
}

int AssignPattern::getPriority() {
    int intersectSize1 = assignment->getValueCount() * variable->getRelationshipCount();
    int intersectSize2 = variable->getValueCount() * assignment->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
