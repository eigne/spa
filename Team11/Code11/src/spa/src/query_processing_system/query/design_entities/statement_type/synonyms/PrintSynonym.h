#pragma once

#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/StatementWithVariableName.h"

class PrintSynonym : public virtual StatementTypeSynonym, public StatementWithVariableName {
public:
    explicit PrintSynonym(const std::string& synonymIdentifier);
    void initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& sc) override;
};
