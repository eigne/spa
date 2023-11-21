#include "WhilePattern.h"

WhilePattern::WhilePattern(const std::shared_ptr<WhileSynonym>& whileSynonym,
                           const std::shared_ptr<IVariable>& variable,
                           bool isNegated) : Pattern(isNegated, 10) {
    this->whileSynonym = whileSynonym;
    this->variable = variable;
}

bool WhilePattern::evaluate(const std::shared_ptr<IStorageReader>& reader) {
    return evaluateWhilePattern(reader);
}

bool WhilePattern::evaluateWhilePattern(const std::shared_ptr<IWhilePatternReader>& reader) {
    auto whilePatternVisitor = std::make_shared<WhilePatternVisitor>(reader);
    whileSynonym->queryAbstractions(whilePatternVisitor);
    auto variables = whilePatternVisitor->getEntityRelationships();
    bool isNonEmptyIntersection1 = isNegated
        ? variable->differenceWithStatement(whileSynonym, variables)
        : variable->intersectWithStatement(whileSynonym, variables);

    variable->queryAbstractions(whilePatternVisitor);
    auto statements = whilePatternVisitor->getStatementRelationships();
    bool isNonEmptyIntersection2 = isNegated
        ? whileSynonym->differenceWithEntity(variable, statements)
        : whileSynonym->intersectWithEntity(variable, statements);

    return isNonEmptyIntersection1 && isNonEmptyIntersection2;
}

int WhilePattern::getPriority() {
    int intersectSize1 = whileSynonym->getValueCount() * variable->getRelationshipCount();
    int intersectSize2 = variable->getValueCount() * whileSynonym->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
