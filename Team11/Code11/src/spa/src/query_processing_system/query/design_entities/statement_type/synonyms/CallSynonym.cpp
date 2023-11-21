#include "CallSynonym.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"

CallSynonym::CallSynonym(const std::string& synonymIdentifier) {
    this->synonymIdentifier = synonymIdentifier;
    possibleStatementNumbers = {};
}

void CallSynonym::initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& v) {
    v->visit(std::dynamic_pointer_cast<CallSynonym>(shared_from_this()));
}
