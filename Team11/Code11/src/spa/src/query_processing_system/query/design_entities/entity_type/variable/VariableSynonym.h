#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/entity_type/EntityTypeSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/EntityWithVariableName.h"

class VariableSynonym : public virtual EntityTypeSynonym, public IVariable, public EntityWithVariableName {
public:
    explicit VariableSynonym(const std::string& synonymIdentifier);
    void initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& sc) override;
    void queryAbstractions(const std::shared_ptr<IVisitsVariable>& v) override;
};
