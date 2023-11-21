#include "ProcedureSynonym.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsProcedure.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"

ProcedureSynonym::ProcedureSynonym(const std::string& synonymIdentifier) {
    this->synonymIdentifier = synonymIdentifier;
}

void ProcedureSynonym::initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& v) {
    v->visit(std::dynamic_pointer_cast<ProcedureSynonym>(shared_from_this()));
}

void ProcedureSynonym::queryAbstractions(const std::shared_ptr<IVisitsProcedure>& v) {
    v->visit(std::dynamic_pointer_cast<ProcedureSynonym>(shared_from_this()));
}
