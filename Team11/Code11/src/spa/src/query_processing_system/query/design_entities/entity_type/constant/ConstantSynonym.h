#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/entity_type/EntityTypeSynonym.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/value/EntityWithValue.h"

class ConstantSynonym : public virtual EntityTypeSynonym, public EntityWithValue {
public:
    explicit ConstantSynonym(const std::string& synonymIdentifier);
    void initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& sc) override;
};
