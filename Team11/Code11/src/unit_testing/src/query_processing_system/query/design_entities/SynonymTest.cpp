#include "catch.hpp"

#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/StatementLineNumber.h"
#include "query_processing_system/query/design_entities/statement_type/StatementWildcard.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableName.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableWildcard.h"

TEST_CASE("Intersect two statements no synonym") {
    StatementWildcard::allPossibleStatements = {1, 2};
    std::shared_ptr<IStatement> s1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<IStatement> s2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<IStatement> sw = std::make_shared<StatementWildcard>();

    REQUIRE(s1->intersectWithStatement(sw, {{1, {0}}}));
    REQUIRE(sw->intersectWithStatement(s1, {{2, {1}}}));
    REQUIRE(s1->intersectWithStatement(s2, {{1, {2}}}));
    REQUIRE_FALSE(s1->intersectWithStatement(sw, {{2, {1}}}));
    REQUIRE_FALSE(sw->intersectWithStatement(sw, {}));
}

TEST_CASE("Intersect two statements 1 synonym") {
    StatementWildcard::allPossibleStatements = {1, 2, 3};
    std::shared_ptr<StatementSynonym> s = std::make_shared<StatementSynonym>("s");
    std::shared_ptr<IStatement> s1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<IStatement> sw = std::make_shared<StatementWildcard>();

    s->setStatementNumbers({1, 2, 3});

    REQUIRE(s->intersectWithStatement(s1, {{2, {1}}, {3, {1}}}));
    REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    REQUIRE(s->intersectWithStatement(sw, {{2, {2}}, {3, {2}}}));
    REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    REQUIRE(s->intersectWithStatement(s1, {{2, {1}}, {3, {2}}}));
    REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    REQUIRE_FALSE(s->intersectWithStatement(sw, {{1, {2}}}));
    REQUIRE(s->getStatementNumbers().empty());

    REQUIRE(s1->intersectWithStatement(s, {{1, {2}}, {2, {1}}}));
    REQUIRE_FALSE(s1->intersectWithStatement(s, {{2, {1}}}));
    REQUIRE(sw->intersectWithStatement(s, {{1, {2}}, {2, {1}}}));
    REQUIRE_FALSE(sw->intersectWithStatement(s, {}));
}

TEST_CASE("Intersect two statements 2 synonym") {
    SECTION("Simple FollowsStar") {
        // test case: Follows*(s1, s2), Follows*(s2, s3)
        // s1 x s2: 1, 2+3    2, 3
        // s2 x s3: 1, 2+3    2, 3
        // s1: 1, s2: 2, s3: 3

        std::shared_ptr<StatementSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementSynonym> s2 = std::make_shared<StatementSynonym>("s2");
        std::shared_ptr<StatementSynonym> s3 = std::make_shared<StatementSynonym>("s3");

        s1->setStatementNumbers({1, 2, 3});
        s2->setStatementNumbers({1, 2, 3});
        s3->setStatementNumbers({1, 2, 3});

        REQUIRE(s1->intersectWithStatement(s2, {{1, {2, 3}}, {2, {3}}}));
        REQUIRE(s2->intersectWithStatement(s1, {{3, {1, 2}}, {2, {1}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2}));
        REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({2, 3}));

        REQUIRE(s2->intersectWithStatement(s3, {{1, {2, 3}}, {2, {3}}}));
        REQUIRE(s3->intersectWithStatement(s2, {{3, {2}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));
        REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({2}));
        REQUIRE(s3->getStatementNumbers() == std::unordered_set<int>({3}));
    }

    SECTION("Follows and FollowsStar") {
        // test case: Follows*(s1, s2), Follows(s2, s3)
        // s1 x s2: 1, 2+3+4    2, 3+4    3, 4
        // s2 x s3: 1, 2    2, 3    3, 4
        // s1: 1+2, s2: 2+3, s3: 3+4

        std::shared_ptr<StatementSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementSynonym> s2 = std::make_shared<StatementSynonym>("s2");
        std::shared_ptr<StatementSynonym> s3 = std::make_shared<StatementSynonym>("s3");

        s1->setStatementNumbers({1, 2, 3, 4});
        s2->setStatementNumbers({1, 2, 3, 4});
        s3->setStatementNumbers({1, 2, 3, 4});

        REQUIRE(s1->intersectWithStatement(s2, {{1, {2, 3, 4}}, {2, {3, 4}}, {3, {4}}}));
        REQUIRE(s2->intersectWithStatement(s1, {{4, {1, 2, 3}}, {3, {1, 2}}, {2, {1}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        REQUIRE(s2->intersectWithStatement(s3, {{1, {2}}, {2, {3}}, {3, {4}}}));
        REQUIRE(s3->intersectWithStatement(s2, {{4, {3}}, {3, {2}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2}));
        REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({2, 3}));
        REQUIRE(s3->getStatementNumbers() == std::unordered_set<int>({3, 4}));
    }

    SECTION("Instant wipe") {
        // test case: Follows*(s1, s2), Follows(s2, s3)
        // s1 x s2: 1, 2+3+4    2, 3+4    3, 4
        // s2 x s3: 1, 2
        // s1: 1+2, s2: 2+3, s3: 3+4

        std::shared_ptr<StatementSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementSynonym> s2 = std::make_shared<StatementSynonym>("s2");
        std::shared_ptr<StatementSynonym> s3 = std::make_shared<StatementSynonym>("s3");

        s1->setStatementNumbers({1, 2, 3, 4});
        s2->setStatementNumbers({1, 2, 3, 4});
        s3->setStatementNumbers({1, 2});

        REQUIRE(s1->intersectWithStatement(s2, {{1, {2, 3, 4}}, {2, {3, 4}}, {3, {4}}}));
        REQUIRE(s2->intersectWithStatement(s1, {{4, {1, 2, 3}}, {3, {1, 2}}, {2, {1}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        REQUIRE_FALSE(s2->intersectWithStatement(s3, {{1, {2}}}));
        REQUIRE_FALSE(s3->intersectWithStatement(s2, {}));
        REQUIRE(s1->getStatementNumbers().empty());
        REQUIRE(s2->getStatementNumbers().empty());
        REQUIRE(s3->getStatementNumbers().empty());
    }

    SECTION("Chain that ends badly") {
        // test case: Follows*(s1, s2), Follows*(s2, s3) Follows*(s3, s1)
        // s1 x s2: 1, 2+3+4    2, 3+4    3, 4

        std::shared_ptr<StatementSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementSynonym> s2 = std::make_shared<StatementSynonym>("s2");
        std::shared_ptr<StatementSynonym> s3 = std::make_shared<StatementSynonym>("s3");

        s1->setStatementNumbers({1, 2, 3, 4});
        s2->setStatementNumbers({1, 2, 3, 4});
        s3->setStatementNumbers({1, 2, 3, 4});

        REQUIRE(s1->intersectWithStatement(s2, {{1, {2, 3, 4}}, {2, {3, 4}}, {3, {4}}}));
        REQUIRE(s2->intersectWithStatement(s1, {{4, {1, 2, 3}}, {3, {1, 2}}, {2, {1}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        REQUIRE(s2->intersectWithStatement(s3, {{1, {2, 3, 4}}, {2, {3, 4}}, {3, {4}}}));
        REQUIRE(s3->intersectWithStatement(s2, {{4, {2, 3}}, {3, {2}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2}));
        REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({2, 3}));
        REQUIRE(s3->getStatementNumbers() == std::unordered_set<int>({3, 4}));

        REQUIRE_FALSE(s3->intersectWithStatement(s1, {{1, {2}}}));
        REQUIRE_FALSE(s1->intersectWithStatement(s3, {}));
        REQUIRE(s1->getStatementNumbers().empty());
        REQUIRE(s2->getStatementNumbers().empty());
        REQUIRE(s3->getStatementNumbers().empty());
    }
}

TEST_CASE("Intersect two entities no synonym") {
    VariableWildcard::allPossibleVariables = {"a", "b"};
    std::shared_ptr<IEntity> v1 = std::make_shared<VariableName>("a");
    std::shared_ptr<IEntity> v2 = std::make_shared<VariableName>("b");
    std::shared_ptr<IEntity> vw = std::make_shared<VariableWildcard>();

    REQUIRE(v1->intersectWithEntity(vw, {{"a", {""}}}));
    REQUIRE(vw->intersectWithEntity(v1, {{"b", {"a"}}}));
    REQUIRE(v1->intersectWithEntity(v2, {{"a", {"b"}}}));
    REQUIRE_FALSE(v1->intersectWithEntity(vw, {{"b", {"a"}}}));
    REQUIRE_FALSE(vw->intersectWithEntity(vw, {}));
}

TEST_CASE("Intersect two entities 1 synonym") {
    VariableWildcard::allPossibleVariables = {"a", "b", "c"};
    std::shared_ptr<EntityTypeSynonym> v = std::make_shared<VariableSynonym>("v");
    std::shared_ptr<IEntity> v1 = std::make_shared<VariableName>("a");
    std::shared_ptr<IEntity> vw = std::make_shared<VariableWildcard>();

    v->setNames({"a", "b", "c"});

    REQUIRE(v->intersectWithEntity(v1, {{"b", {"a"}}, {"c", {"a"}}}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    REQUIRE(v->intersectWithEntity(vw, {{"b", {"b"}}, {"c", {"b"}}}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    REQUIRE(v->intersectWithEntity(v1, {{"b", {"a"}}, {"c", {"b"}}}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    REQUIRE_FALSE(v->intersectWithEntity(vw, {{"a", {"b"}}}));
    REQUIRE(v->getNames().empty());

    REQUIRE(v1->intersectWithEntity(v, {{"a", {"b"}}, {"b", {"a"}}}));
    REQUIRE_FALSE(v1->intersectWithEntity(v, {{"b", {"a"}}}));
    REQUIRE(vw->intersectWithEntity(v, {{"a", {"b"}}, {"b", {"a"}}}));
    REQUIRE_FALSE(vw->intersectWithEntity(v, {}));
}

TEST_CASE("Intersect two entities 2 synonym") {
    // same test cases as statement
    SECTION("Simple FollowsStar") {
        std::shared_ptr<EntityTypeSynonym> v1 = std::make_shared<VariableSynonym>("v1");
        std::shared_ptr<EntityTypeSynonym> v2 = std::make_shared<VariableSynonym>("v2");
        std::shared_ptr<EntityTypeSynonym> v3 = std::make_shared<VariableSynonym>("v3");

        v1->setNames({"a", "b", "c"});
        v2->setNames({"a", "b", "c"});
        v3->setNames({"a", "b", "c"});

        REQUIRE(v1->intersectWithEntity(v2, {{"a", {"b", "c"}}, {"b", {"c"}}}));
        REQUIRE(v2->intersectWithEntity(v1, {{"c", {"a", "b"}}, {"b", {"a"}}}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b"}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b", "c"}));

        REQUIRE(v2->intersectWithEntity(v3, {{"a", {"b", "c"}}, {"b", {"c"}}}));
        REQUIRE(v3->intersectWithEntity(v2, {{"c", {"b"}}}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a"}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b"}));
        REQUIRE(v3->getNames() == std::unordered_set<std::string>({"c"}));
    }

    SECTION("Follows and FollowsStar") {
        std::shared_ptr<EntityTypeSynonym> v1 = std::make_shared<VariableSynonym>("v1");
        std::shared_ptr<EntityTypeSynonym> v2 = std::make_shared<VariableSynonym>("v2");
        std::shared_ptr<EntityTypeSynonym> v3 = std::make_shared<VariableSynonym>("v3");

        v1->setNames({"a", "b", "c", "d"});
        v2->setNames({"a", "b", "c", "d"});
        v3->setNames({"a", "b", "c", "d"});

        REQUIRE(v1->intersectWithEntity(v2, {{"a", {"b", "c", "d"}}, {"b", {"c", "d"}}, {"c", {"d"}}}));
        REQUIRE(v2->intersectWithEntity(v1, {{"d", {"a", "b", "c"}}, {"c", {"a", "b"}}, {"b", {"a"}}}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));

        REQUIRE(v2->intersectWithEntity(v3, {{"a", {"b"}}, {"b", {"c"}}, {"c", {"d"}}}));
        REQUIRE(v3->intersectWithEntity(v2, {{"d", {"c"}}, {"c", {"b"}}}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b"}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b", "c"}));
        REQUIRE(v3->getNames() == std::unordered_set<std::string>({"c", "d"}));
    }

    SECTION("Instant wipe") {
        std::shared_ptr<EntityTypeSynonym> v1 = std::make_shared<VariableSynonym>("v1");
        std::shared_ptr<EntityTypeSynonym> v2 = std::make_shared<VariableSynonym>("v2");
        std::shared_ptr<EntityTypeSynonym> v3 = std::make_shared<VariableSynonym>("v3");

        v1->setNames({"a", "b", "c", "d"});
        v2->setNames({"a", "b", "c", "d"});
        v3->setNames({"a", "b"});

        REQUIRE(v1->intersectWithEntity(v2, {{"a", {"b", "c", "d"}}, {"b", {"c", "d"}}, {"c", {"d"}}}));
        REQUIRE(v2->intersectWithEntity(v1, {{"d", {"a", "b", "c"}}, {"c", {"a", "b"}}, {"b", {"a"}}}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));

        REQUIRE_FALSE(v2->intersectWithEntity(v3, {{"a", {"b"}}}));
        REQUIRE_FALSE(v3->intersectWithEntity(v2, {}));
        REQUIRE(v1->getNames().empty());
        REQUIRE(v2->getNames().empty());
        REQUIRE(v3->getNames().empty());
    }
}

TEST_CASE("Intersect entity and statement no synonym") {
    VariableWildcard::allPossibleVariables = {"a", "b"};
    StatementWildcard::allPossibleStatements = {1, 2};
    std::shared_ptr<IStatement> s1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<IStatement> sw = std::make_shared<StatementWildcard>();
    std::shared_ptr<IEntity> v1 = std::make_shared<VariableName>("a");
    std::shared_ptr<IEntity> vw = std::make_shared<VariableWildcard>();

    REQUIRE(s1->intersectWithEntity(vw, {{1, {""}}}));
    REQUIRE(sw->intersectWithEntity(v1, {{2, {"a"}}}));
    REQUIRE_FALSE(s1->intersectWithEntity(v1, {{2, {"a"}}}));
    REQUIRE_FALSE(sw->intersectWithEntity(vw, {}));

    REQUIRE(v1->intersectWithStatement(sw, {{"a", {0}}}));
    REQUIRE(vw->intersectWithStatement(s1, {{"b", {1}}}));
    REQUIRE_FALSE(v1->intersectWithStatement(s1, {{"b", {1}}}));
    REQUIRE_FALSE(vw->intersectWithStatement(sw, {}));
}

TEST_CASE("Intersect entity and statement 1 synonym") {
    VariableWildcard::allPossibleVariables = {"a", "b", "c"};
    StatementWildcard::allPossibleStatements = {1, 2, 3};
    std::shared_ptr<EntityTypeSynonym> v = std::make_shared<VariableSynonym>("v");
    std::shared_ptr<IEntity> v1 = std::make_shared<VariableName>("a");
    std::shared_ptr<IEntity> vw = std::make_shared<VariableWildcard>();

    std::shared_ptr<StatementSynonym> s = std::make_shared<StatementSynonym>("s");
    std::shared_ptr<IStatement> s1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<IStatement> sw = std::make_shared<StatementWildcard>();

    s->setStatementNumbers({1, 2, 3});
    v->setNames({"a", "b", "c"});

    REQUIRE(s->intersectWithEntity(v1, {{2, {"a"}}, {3, {"a"}}}));
    REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    REQUIRE(s->intersectWithEntity(vw, {{2, {"b"}}, {3, {"b"}}}));
    REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    REQUIRE(s->intersectWithEntity(v1, {{2, {"a"}}, {3, {"b"}}}));
    REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    REQUIRE_FALSE(s->intersectWithEntity(vw, {{1, {"b"}}}));
    REQUIRE(s->getStatementNumbers().empty());

    REQUIRE(s1->intersectWithEntity(v, {{1, {"b"}}, {2, {"a"}}}));
    REQUIRE_FALSE(s1->intersectWithEntity(v, {{2, {"a"}}}));
    REQUIRE(sw->intersectWithEntity(v, {{1, {"b"}}, {2, {"a"}}}));
    REQUIRE_FALSE(sw->intersectWithEntity(v, {}));

    REQUIRE(v->intersectWithStatement(s1, {{"b", {1}}, {"c", {1}}}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    REQUIRE(v->intersectWithStatement(sw, {{"b", {2}}, {"c", {2}}}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    REQUIRE(v->intersectWithStatement(s1, {{"b", {1}}, {"c", {2}}}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    REQUIRE_FALSE(v->intersectWithStatement(sw, {{"a", {2}}}));
    REQUIRE(v->getNames().empty());

    REQUIRE(v1->intersectWithStatement(s, {{"a", {2}}, {"b", {1}}}));
    REQUIRE_FALSE(v1->intersectWithStatement(s, {{"b", {1}}}));
    REQUIRE(vw->intersectWithStatement(s, {{"a", {2}}, {"b", {1}}}));
    REQUIRE_FALSE(vw->intersectWithStatement(s, {}));
}

TEST_CASE("Intersect entity and statement 2 synonym") {
    // same test cases as statement
    SECTION("Simple FollowsStar") {
        std::shared_ptr<StatementSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<EntityTypeSynonym> v2 = std::make_shared<VariableSynonym>("v2");
        std::shared_ptr<EntityTypeSynonym> v3 = std::make_shared<VariableSynonym>("v3");

        s1->setStatementNumbers({1, 2, 3});
        v2->setNames({"a", "b", "c"});
        v3->setNames({"a", "b", "c"});

        REQUIRE(s1->intersectWithEntity(v2, {{1, {"b", "c"}}, {2, {"c"}}}));
        REQUIRE(v2->intersectWithStatement(s1, {{"c", {1, 2}}, {"b", {1}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b", "c"}));

        REQUIRE(v2->intersectWithEntity(v3, {{"a", {"b", "c"}}, {"b", {"c"}}}));
        REQUIRE(v3->intersectWithEntity(v2, {{"c", {"b"}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b"}));
        REQUIRE(v3->getNames() == std::unordered_set<std::string>({"c"}));
    }

    SECTION("Follows and FollowsStar") {
        std::shared_ptr<StatementSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<EntityTypeSynonym> v2 = std::make_shared<VariableSynonym>("v2");
        std::shared_ptr<EntityTypeSynonym> v3 = std::make_shared<VariableSynonym>("v3");

        s1->setStatementNumbers({1, 2, 3, 4});
        v2->setNames({"a", "b", "c", "d"});
        v3->setNames({"a", "b", "c", "d"});

        REQUIRE(s1->intersectWithEntity(v2, {{1, {"b", "c", "d"}}, {2, {"c", "d"}}, {3, {"d"}}}));
        REQUIRE(v2->intersectWithStatement(s1, {{"d", {1, 2, 3}}, {"c", {1, 2}}, {"b", {1}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));

        REQUIRE(v2->intersectWithEntity(v3, {{"a", {"b"}}, {"b", {"c"}}, {"c", {"d"}}}));
        REQUIRE(v3->intersectWithEntity(v2, {{"d", {"c"}}, {"c", {"b"}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b", "c"}));
        REQUIRE(v3->getNames() == std::unordered_set<std::string>({"c", "d"}));
    }

    SECTION("Instant wipe") {
        std::shared_ptr<StatementSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<EntityTypeSynonym> v2 = std::make_shared<VariableSynonym>("v2");
        std::shared_ptr<EntityTypeSynonym> v3 = std::make_shared<VariableSynonym>("v3");

        s1->setStatementNumbers({1, 2, 3, 4});
        v2->setNames({"a", "b", "c", "d"});
        v3->setNames({"a", "b"});

        REQUIRE(s1->intersectWithEntity(v2, {{1, {"b", "c", "d"}}, {2, {"c", "d"}}, {3, {"d"}}}));
        REQUIRE(v2->intersectWithStatement(s1, {{"d", {1, 2, 3}}, {"c", {1, 2}}, {"b", {1}}}));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(v2->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));

        REQUIRE_FALSE(v2->intersectWithEntity(v3, {{"a", {"b"}}}));
        REQUIRE_FALSE(v3->intersectWithEntity(v2, {}));
        REQUIRE(s1->getStatementNumbers().empty());
        REQUIRE(v2->getNames().empty());
        REQUIRE(v3->getNames().empty());
    }
}
