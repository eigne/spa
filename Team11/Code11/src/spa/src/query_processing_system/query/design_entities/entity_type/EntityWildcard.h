#pragma once

#include <string>
#include "query_processing_system/query/design_entities/entity_type/IEntity.h"

class EntityWildcard : public virtual IEntity {
public:
    bool isSynonym() override;
    int getRelationshipCount() override;
    int getValueCount() override;
};
