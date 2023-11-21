#include "Query.h"

Query::Query() {
    this->synonyms = {};
    this->clauses = {};
}

Query::Query(const std::vector<std::shared_ptr<ISynonym> >& synonyms, const QuerySelection& selected) {
    this->synonyms = synonyms;
    this->selected = selected;
    clauses = {};
}

Query::Query(const std::vector<std::shared_ptr<ISynonym> >& synonyms, const QuerySelection& selected,
             const std::shared_ptr<SuchThat>& suchThat) {
    this->synonyms = synonyms;
    this->selected = selected;
    clauses = {suchThat};
}

Query::Query(const std::vector<std::shared_ptr<ISynonym> >& synonyms, const QuerySelection& selected,
             const std::shared_ptr<Pattern>& pattern) {
    this->synonyms = synonyms;
    this->selected = selected;
    clauses = {pattern};
}

Query::Query(const std::vector<std::shared_ptr<ISynonym> >& synonyms, const QuerySelection& selected,
             const std::shared_ptr<SuchThat>& suchThat, const std::shared_ptr<Pattern>& pattern) {
    this->synonyms = synonyms;
    this->selected = selected;
    // order matters: pattern should be solved first
    clauses = {pattern, suchThat};
}

void Query::addSynonym(const std::shared_ptr<ISynonym>& synonym) {
    this->synonyms.push_back(synonym);
}

void Query::setSelection(QuerySelection &selection) {
    this->selected = selection;
}

void Query::addClause(const std::shared_ptr<Clause>& clause) {
    this->clauses.push_back(clause);
}

std::vector<std::shared_ptr<ISynonym> > Query::getSynonyms() {
    return synonyms;
}

bool Query::hasClauses() {
    return !clauses.empty();
}

std::shared_ptr<Clause> Query::getNextClause() {
    std::vector<std::shared_ptr<Clause>> nextClauses;
    std::shared_ptr<Clause> minClause = nullptr;
    for (const auto& clause : clauses) {
        if (minClause == nullptr) {
            minClause = clause;
        } else if (clause->getPriority() < minClause->getPriority()) {
            nextClauses.push_back(minClause);
            minClause = clause;
        } else {
            nextClauses.push_back(clause);
        }
    }
    clauses = nextClauses;
    return minClause;
}

std::vector<std::shared_ptr<Clause> > Query::getClauses() {
    return clauses;
}

QuerySelection Query::getSelected() {
    return selected;
}
