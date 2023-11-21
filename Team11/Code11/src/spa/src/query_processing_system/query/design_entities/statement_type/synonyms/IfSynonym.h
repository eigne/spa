#pragma once

#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"

class IfSynonym : public StatementTypeSynonym {
public:
    explicit IfSynonym(const std::string& synonymIdentifier);
    void initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& sc) override;
};
