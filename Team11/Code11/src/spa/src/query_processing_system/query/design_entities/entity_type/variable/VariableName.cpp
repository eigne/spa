#include "VariableName.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsVariable.h"

void VariableName::queryAbstractions(const std::shared_ptr<IVisitsVariable>& v) {
    v->visit(shared_from_this());
}
