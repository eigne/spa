#pragma once

#include <memory>

#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsProcedure.h"
#include \
    "query_processing_system/query_evaluator/design_entity_visitor/interfaces/relationships/EntityEntityRelationship.h"
#include "query_processing_system/query_evaluator/storage_reader/ICallsReader.h"

class CallsVisitor : public IVisitsProcedure, public EntityEntityRelationship {
private:
    bool onlyQueryImmediate;
    bool queryForCalled;
    std::shared_ptr<ICallsReader> callsReader;
    std::unordered_set<std::string> queryBasedOnState(const std::string& procedureName);
public:
    explicit CallsVisitor(const std::shared_ptr<ICallsReader>& callsReader);

    void visit(const std::shared_ptr<ProcedureSynonym>& procedure) override;
    void visit(const std::shared_ptr<ProcedureName>& procedure) override;
    void visit(const std::shared_ptr<ProcedureWildcard>& procedure) override;
    void setImmediateState(bool onlyImmediate);
    void setCalledQueryState(bool queryCalled);
};
