#include "ReadSynonym.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"

ReadSynonym::ReadSynonym(const std::string& synonymIdentifier) {
    this->synonymIdentifier = synonymIdentifier;
    possibleStatementNumbers = {};
}

void ReadSynonym::initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& v) {
    v->visit(std::dynamic_pointer_cast<ReadSynonym>(shared_from_this()));
}
