#include "With.h"
#include <utility>

With::With(std::shared_ptr<WithRef> ref1, std::shared_ptr<WithRef> ref2, bool isNegated) : Clause(isNegated, 10) {
    this->ref1 = std::move(ref1);
    this->ref2 = std::move(ref2);
}

bool With::evaluate(const std::shared_ptr<IStorageReader> &reader) {
    return evaluateWith();
}

bool With::evaluateWith() {
    WithVisitor visitor{};
    visitor.setNegated(isNegated);
    bool intersection1 = ref1->visit(visitor, ref2);
    bool intersection2 = ref2->visit(visitor, ref1);
    return intersection1 && intersection2;
}

int With::getPriority() {
    int intersectSize1 = ref1->getValueCount() * ref2->getRelationshipCount();
    int intersectSize2 = ref2->getValueCount() * ref1->getRelationshipCount();
    return clauseWeight * std::max(intersectSize1, intersectSize2);
}
