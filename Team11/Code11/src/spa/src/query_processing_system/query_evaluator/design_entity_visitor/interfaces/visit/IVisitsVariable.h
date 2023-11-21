#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/entity_type/variable/VariableSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableName.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableWildcard.h"

class IVisitsVariable {
public:
    virtual void visit(const std::shared_ptr<VariableSynonym>& variable) = 0;
    virtual void visit(const std::shared_ptr<VariableName>& variable) = 0;
    virtual void visit(const std::shared_ptr<VariableWildcard>& variable) = 0;
};
