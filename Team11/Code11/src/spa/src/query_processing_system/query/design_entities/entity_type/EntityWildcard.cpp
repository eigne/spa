#include "EntityWildcard.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"

bool EntityWildcard::isSynonym() {
    return false;
}

int EntityWildcard::getValueCount() {
    return getNames().size();
}

int EntityWildcard::getRelationshipCount() {
    return 1;
}
