#include "PrintSynonym.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsSynonym.h"

PrintSynonym::PrintSynonym(const std::string& synonymIdentifier) {
    this->synonymIdentifier = synonymIdentifier;
    possibleStatementNumbers = {};
}

void PrintSynonym::initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& v) {
    v->visit(std::dynamic_pointer_cast<PrintSynonym>(shared_from_this()));
}
