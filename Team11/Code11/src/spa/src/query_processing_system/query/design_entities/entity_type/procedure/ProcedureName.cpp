#include "ProcedureName.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsProcedure.h"

void ProcedureName::queryAbstractions(const std::shared_ptr<IVisitsProcedure>& v) {
    v->visit(shared_from_this());
}
