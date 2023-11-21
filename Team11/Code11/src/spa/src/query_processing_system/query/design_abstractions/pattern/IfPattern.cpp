#include "IfPattern.h"

IfPattern::IfPattern(const std::shared_ptr<IfSynonym>& ifSynonym, const std::shared_ptr<IVariable>& variable,
                     bool isNegated) : Pattern(isNegated, 10) {
    this->ifSynonym = ifSynonym;
    this->variable = variable;
}

bool IfPattern::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateIfPattern(reader);
}

bool IfPattern::evaluateIfPattern(const std::shared_ptr<IIfPatternReader>& reader) {
    auto ifPatternVisitor = std::make_shared<IfPatternVisitor>(reader);
    ifSynonym->queryAbstractions(ifPatternVisitor);
    auto variables = ifPatternVisitor->getEntityRelationships();
    bool isNonEmptyIntersection1 = isNegated
        ? variable->differenceWithStatement(ifSynonym, variables)
        : variable->intersectWithStatement(ifSynonym, variables);

    variable->queryAbstractions(ifPatternVisitor);
    auto statements = ifPatternVisitor->getStatementRelationships();
    bool isNonEmptyIntersection2 = isNegated
        ? ifSynonym->differenceWithEntity(variable, statements)
        : ifSynonym->intersectWithEntity(variable, statements);

    return isNonEmptyIntersection1 && isNonEmptyIntersection2;
}

int IfPattern::getPriority() {
    int intersectSize1 = ifSynonym->getValueCount() * variable->getRelationshipCount();
    int intersectSize2 = variable->getValueCount() * ifSynonym->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
