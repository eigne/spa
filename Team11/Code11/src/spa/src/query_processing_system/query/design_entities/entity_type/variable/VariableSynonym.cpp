#include "VariableSynonym.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsVariable.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"

VariableSynonym::VariableSynonym(const std::string& synonymIdentifier) {
    this->synonymIdentifier = synonymIdentifier;
}

void VariableSynonym::initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& v) {
    v->visit(std::dynamic_pointer_cast<VariableSynonym>(shared_from_this()));
}

void VariableSynonym::queryAbstractions(const std::shared_ptr<IVisitsVariable>& v) {
    v->visit(std::dynamic_pointer_cast<VariableSynonym>(shared_from_this()));
}
