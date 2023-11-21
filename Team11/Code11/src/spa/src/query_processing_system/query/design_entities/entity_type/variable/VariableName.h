#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/entity_type/EntityName.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"

class VariableName : public IVariable, public EntityName, public std::enable_shared_from_this<VariableName> {
public:
    explicit VariableName(std::string name) : EntityName(std::move(name)) {
    }
    void queryAbstractions(const std::shared_ptr<IVisitsVariable>& v) override;
};
