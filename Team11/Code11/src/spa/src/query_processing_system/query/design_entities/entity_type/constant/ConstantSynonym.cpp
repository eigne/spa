#include "ConstantSynonym.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"

ConstantSynonym::ConstantSynonym(const std::string& synonymIdentifier) {
    this->synonymIdentifier = synonymIdentifier;
}

void ConstantSynonym::initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& v) {
    v->visit(std::dynamic_pointer_cast<ConstantSynonym>(shared_from_this()));
}
