#pragma once

#include "query_processing_system/query/design_entities/with_ref/ConstantWithRef.h"
#include "query_processing_system/query/design_entities/with_ref/EntityAttributeRef.h"
#include "query_processing_system/query/design_entities/with_ref/StatementAttributeRef.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableName.h"

class WithVisitor {
private:
    bool isNegated;
public:
    void setNegated(bool negation);
    bool visit(const std::shared_ptr<ConstantWithRef>& ref, const std::shared_ptr<WithRef>& other);
    bool visit(const std::shared_ptr<EntityAttributeRef>& ref, const std::shared_ptr<WithRef>& other);
    bool visit(const std::shared_ptr<StatementAttributeRef>& ref, const std::shared_ptr<WithRef>& other);
};
