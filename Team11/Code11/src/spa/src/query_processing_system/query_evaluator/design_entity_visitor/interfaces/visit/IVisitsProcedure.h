#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/entity_type/procedure/ProcedureSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/ProcedureName.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/ProcedureWildcard.h"

class IVisitsProcedure {
public:
    virtual void visit(const std::shared_ptr<ProcedureSynonym>& variable) = 0;
    virtual void visit(const std::shared_ptr<ProcedureName>& variable) = 0;
    virtual void visit(const std::shared_ptr<ProcedureWildcard>& variable) = 0;
};
