#pragma once

#include <memory>
#include <string>
#include "query_processing_system/query/design_abstractions/Clause.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/AssignSynonym.h"
#include "commons/ExpressionParser.h"


class Pattern : public Clause {
public:
    explicit Pattern(bool isNegated, int clauseWeight) : Clause(isNegated, clauseWeight) {
    }
};
