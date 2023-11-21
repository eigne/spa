#pragma once

#include "query_processing_system/query/design_abstractions/Clause.h"

class SuchThat : public Clause {
public:
    explicit SuchThat(bool isNegated, int clauseWeight) : Clause(isNegated, clauseWeight) {
    }
};
