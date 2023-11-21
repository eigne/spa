#include "catch.hpp"
#include <memory>
#include "query_processing_system/query/Query.h"

#include "query_processing_system/query/design_abstractions/Clause.h"
#include "query_processing_system/query/design_abstractions/pattern/AssignPattern.h"
#include "query_processing_system/query/design_abstractions/such_that/uses/UsesStatement.h"

#include "query_processing_system/query/design_entities/ISynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/AssignSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableSynonym.h"

TEST_CASE("No Clauses") {
    std::shared_ptr<AssignSynonym> a = std::make_shared<AssignSynonym>("a");
    std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");

    std::shared_ptr<ISynonym> aSyn = std::static_pointer_cast<ISynonym>(a);
    std::shared_ptr<ISynonym> vSyn = std::static_pointer_cast<ISynonym>(v);

    QuerySelection sel_a = QuerySelection(aSyn);
    QuerySelection sel_v = QuerySelection(vSyn);

    Query q1({a}, sel_a);
    REQUIRE(q1.getSynonyms() == std::vector<std::shared_ptr<ISynonym>>({aSyn}));
    REQUIRE(q1.getSelected() == sel_a);
    REQUIRE(q1.getClauses().empty());

    Query q2({a, v}, sel_v);
    REQUIRE(q2.getSynonyms() == std::vector<std::shared_ptr<ISynonym>>({aSyn, vSyn}));
    REQUIRE(q2.getSelected() == sel_v);
    REQUIRE(q2.getClauses().empty());
}

TEST_CASE("One Clause") {
    std::shared_ptr<AssignSynonym> a = std::make_shared<AssignSynonym>("a");
    std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
    std::shared_ptr<Pattern> p = std::make_shared<AssignPattern>(a, v, "", true, false);
    std::shared_ptr<UsesStatement> u = std::make_shared<UsesStatement>(a, v, false);

    auto synnames = std::make_shared<std::set<std::string>>();
    synnames->insert("a");
    synnames->insert("v");
    p->synonym = synnames;
    u->synonym = synnames;

    std::shared_ptr<ISynonym> aSyn = std::static_pointer_cast<ISynonym>(a);
    std::shared_ptr<ISynonym> vSyn = std::static_pointer_cast<ISynonym>(v);
    std::shared_ptr<Clause> pClause = std::static_pointer_cast<Clause>(p);
    std::shared_ptr<Clause> uClause = std::static_pointer_cast<Clause>(u);

    QuerySelection sel_a = QuerySelection(aSyn);
    QuerySelection sel_v = QuerySelection(vSyn);

    Query q1({a, v}, sel_a, p);
    REQUIRE(q1.getSynonyms() == std::vector<std::shared_ptr<ISynonym>>({aSyn, vSyn}));
    REQUIRE(q1.getSelected() == sel_a);
    REQUIRE(q1.getClauses() == std::vector<std::shared_ptr<Clause>>({pClause}));

    Query q2({a, v}, sel_v, u);
    REQUIRE(q2.getSynonyms() == std::vector<std::shared_ptr<ISynonym>>({aSyn, vSyn}));
    REQUIRE(q2.getSelected() == sel_v);
    REQUIRE(q2.getClauses() == std::vector<std::shared_ptr<Clause>>({uClause}));
}

TEST_CASE("Two Clause") {
    std::shared_ptr<AssignSynonym> a = std::make_shared<AssignSynonym>("a");
    std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
    std::shared_ptr<Pattern> p = std::make_shared<AssignPattern>(a, v, "", true, false);
    auto synnames = std::make_shared<std::set<std::string>>();
    synnames->insert("a");
    synnames->insert("v");
    p->synonym = synnames;
    std::shared_ptr<UsesStatement> u = std::make_shared<UsesStatement>(a, v, false);
    u->synonym = synnames;

    std::shared_ptr<ISynonym> aSyn = std::static_pointer_cast<ISynonym>(a);
    std::shared_ptr<ISynonym> vSyn = std::static_pointer_cast<ISynonym>(v);
    std::shared_ptr<Clause> pClause = std::static_pointer_cast<Clause>(p);
    std::shared_ptr<Clause> uClause = std::static_pointer_cast<Clause>(u);

    QuerySelection sel_a = QuerySelection(aSyn);

    Query q1({a, v}, sel_a, u, p);
    auto f = q1.getClauses();
    REQUIRE(q1.getSynonyms() == std::vector<std::shared_ptr<ISynonym>>({aSyn, vSyn}));
    REQUIRE(q1.getSelected() == sel_a);
    REQUIRE(q1.getClauses() == std::vector<std::shared_ptr<Clause>>({pClause, uClause}));
}

TEST_CASE("Clause order") {

}
