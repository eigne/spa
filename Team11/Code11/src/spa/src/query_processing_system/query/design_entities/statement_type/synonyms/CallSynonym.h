#pragma once

#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/procedure_name/StatementWithProcedureName.h"

class CallSynonym : public virtual StatementTypeSynonym, public StatementWithProcedureName {
public:
    explicit CallSynonym(const std::string& synonymIdentifier);
    void initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& sc) override;
};
