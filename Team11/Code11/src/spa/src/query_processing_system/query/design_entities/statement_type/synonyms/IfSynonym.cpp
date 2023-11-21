#include "IfSynonym.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"

IfSynonym::IfSynonym(const std::string& synonymIdentifier) {
    this->synonymIdentifier = synonymIdentifier;
    possibleStatementNumbers = {};
}

void IfSynonym::initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& v) {
    v->visit(std::dynamic_pointer_cast<IfSynonym>(shared_from_this()));
}
