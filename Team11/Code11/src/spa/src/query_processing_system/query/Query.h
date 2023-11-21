#pragma once

#include <vector>
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query/design_entities/ISynonym.h"
#include "query_processing_system/query/design_abstractions/Clause.h"
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_abstractions/pattern/Pattern.h"

class Query {
private:
    std::vector<std::shared_ptr<ISynonym> > synonyms;
    QuerySelection selected;
    std::vector<std::shared_ptr<Clause> > clauses;

public:
    Query();
// for debugging?
    Query(const std::vector<std::shared_ptr<ISynonym> >& synonyms, const QuerySelection& selected);
    Query(const std::vector<std::shared_ptr<ISynonym> >& synonyms, const QuerySelection& selected,
          const std::shared_ptr<SuchThat>& suchThat);
    Query(const std::vector<std::shared_ptr<ISynonym> >& synonyms, const QuerySelection& selected,
          const std::shared_ptr<Pattern>& pattern);
    Query(const std::vector<std::shared_ptr<ISynonym> >& synonyms, const QuerySelection& selected,
          const std::shared_ptr<SuchThat>& suchThat, const std::shared_ptr<Pattern>& pattern);

    void addSynonym(const std::shared_ptr<ISynonym>& synonym);
    void setSelection(QuerySelection& selection);
    void addClause(const std::shared_ptr<Clause>& clause);

    std::vector<std::shared_ptr<ISynonym>> getSynonyms();
    std::vector<std::shared_ptr<Clause>> getClauses();
    bool hasClauses();
    std::shared_ptr<Clause> getNextClause();
    QuerySelection getSelected();
};
