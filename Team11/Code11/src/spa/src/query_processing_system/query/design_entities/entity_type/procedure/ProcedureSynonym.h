#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/entity_type/EntityTypeSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/IProcedure.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/procedure_name/EntityWithProcedureName.h"

class ProcedureSynonym : public virtual EntityTypeSynonym, public IProcedure, public EntityWithProcedureName {
public:
    explicit ProcedureSynonym(const std::string& synonymIdentifier);
    void initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& sc) override;
    void queryAbstractions(const std::shared_ptr<IVisitsProcedure>& v) override;
};
