#include "WithVisitor.h"

void WithVisitor::setNegated(bool negation) {
    isNegated = negation;
}

bool WithVisitor::visit(const std::shared_ptr<ConstantWithRef> &ref, const std::shared_ptr<WithRef> &other) {
    auto fakeEntity = std::make_shared<VariableName>(ref->value);
    if (isNegated) {
        return other->differenceWithEntity(fakeEntity, {{ref->value, {ref->value}}});
    } else {
        return other->intersectWithEntity(fakeEntity, {{ref->value, {ref->value}}});
    }
}

bool WithVisitor::visit(const std::shared_ptr<EntityAttributeRef> &ref, const std::shared_ptr<WithRef> &other) {
    auto entity = std::dynamic_pointer_cast<IEntity>(ref->attribute->getSynonym());
    if (isNegated) {
        return other->differenceWithEntity(entity, ref->getAttributeMap());
    } else {
        return other->intersectWithEntity(entity, ref->getAttributeMap());
    }
}

bool WithVisitor::visit(const std::shared_ptr<StatementAttributeRef> &ref, const std::shared_ptr<WithRef> &other) {
    auto statement = std::dynamic_pointer_cast<IStatement>(ref->attribute->getSynonym());
    if (isNegated) {
        return other->differenceWithStatement(statement, ref->getAttributeMap());
    } else {
        return other->intersectWithStatement(statement, ref->getAttributeMap());
    }
}
