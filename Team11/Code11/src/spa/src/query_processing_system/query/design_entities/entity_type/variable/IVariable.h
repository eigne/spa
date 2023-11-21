#pragma once

#include "query_processing_system/query/design_entities/entity_type/IEntity.h"

class IVisitsVariable;

class IVariable : public virtual IEntity {
public:
    virtual void queryAbstractions(const std::shared_ptr<IVisitsVariable>& v) = 0;
};
