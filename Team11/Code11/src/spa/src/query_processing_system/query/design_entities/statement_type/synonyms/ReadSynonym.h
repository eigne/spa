#pragma once

#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/StatementWithVariableName.h"

class ReadSynonym : public virtual StatementTypeSynonym, public StatementWithVariableName {
public:
    explicit ReadSynonym(const std::string& synonymIdentifier);
    void initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& sc) override;
};
