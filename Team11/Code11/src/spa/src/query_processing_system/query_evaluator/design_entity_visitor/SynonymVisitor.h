#pragma once

#include "query_processing_system/query_evaluator/storage_reader/ISynonymReader.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/StatementWildcard.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/ProcedureWildcard.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableWildcard.h"

class SynonymVisitor : public IVisitsSynonym {
private:
    std::shared_ptr<ISynonymReader> synonymReader;
public:
    explicit SynonymVisitor(const std::shared_ptr<ISynonymReader>& synonymReader);

    void visit(const std::shared_ptr<ConstantSynonym>& entity) override;
    void visit(const std::shared_ptr<ProcedureSynonym>& entity) override;
    void visit(const std::shared_ptr<VariableSynonym>& entity) override;

    void visit(const std::shared_ptr<AssignSynonym>& statement) override;
    void visit(const std::shared_ptr<CallSynonym>& statement) override;
    void visit(const std::shared_ptr<IfSynonym>& statement) override;
    void visit(const std::shared_ptr<PrintSynonym>& statement) override;
    void visit(const std::shared_ptr<ReadSynonym>& statement) override;
    void visit(const std::shared_ptr<StatementSynonym>& statement) override;
    void visit(const std::shared_ptr<WhileSynonym>& statement) override;
};
