#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/StatementLineNumber.h"
#include "query_processing_system/query/design_entities/statement_type/StatementWildcard.h"

class IVisitsStatement {
public:
    virtual void visit(const std::shared_ptr<StatementTypeSynonym>& statement) = 0;
    virtual void visit(const std::shared_ptr<StatementLineNumber>& statement) = 0;
    virtual void visit(const std::shared_ptr<StatementWildcard>& statement) = 0;
};
