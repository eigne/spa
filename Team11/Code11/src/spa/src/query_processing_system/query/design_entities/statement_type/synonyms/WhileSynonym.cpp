#include "WhileSynonym.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"

WhileSynonym::WhileSynonym(const std::string& synonymIdentifier) {
    this->synonymIdentifier = synonymIdentifier;
    possibleStatementNumbers = {};
}

void WhileSynonym::initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& v) {
    v->visit(std::dynamic_pointer_cast<WhileSynonym>(shared_from_this()));
}
