#pragma once

#include <memory>
#include <utility>
#include "query_processing_system/query/design_entities/entity_type/EntityName.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/IProcedure.h"

class ProcedureName : public IProcedure, public EntityName, public std::enable_shared_from_this<ProcedureName> {
public:
    explicit ProcedureName(std::string name) : EntityName(std::move(name)) {
    }
    void queryAbstractions(const std::shared_ptr<IVisitsProcedure>& v) override;
};
