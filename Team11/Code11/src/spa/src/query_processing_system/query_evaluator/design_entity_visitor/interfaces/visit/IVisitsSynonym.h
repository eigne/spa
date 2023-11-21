#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/entity_type/constant/ConstantSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/ProcedureSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableSynonym.h"

#include "query_processing_system/query/design_entities/statement_type/synonyms/AssignSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/CallSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/IfSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/PrintSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/ReadSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/WhileSynonym.h"


class IVisitsSynonym {
public:
    virtual void visit(const std::shared_ptr<ConstantSynonym>& entity) = 0;
    virtual void visit(const std::shared_ptr<ProcedureSynonym>& entity) = 0;
    virtual void visit(const std::shared_ptr<VariableSynonym>& entity) = 0;

    virtual void visit(const std::shared_ptr<AssignSynonym>& statement) = 0;
    virtual void visit(const std::shared_ptr<CallSynonym>& statement) = 0;
    virtual void visit(const std::shared_ptr<IfSynonym>& statement) = 0;
    virtual void visit(const std::shared_ptr<PrintSynonym>& statement) = 0;
    virtual void visit(const std::shared_ptr<ReadSynonym>& statement) = 0;
    virtual void visit(const std::shared_ptr<StatementSynonym>& statement) = 0;
    virtual void visit(const std::shared_ptr<WhileSynonym>& statement) = 0;
};
