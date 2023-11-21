#include "catch.hpp"

#include <memory>
#include "query_processing_system/query/query_selection/QuerySelection.h"

#include "query_processing_system/query/design_entities/statement_type/synonyms/ReadSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/PrintSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/CallSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementSynonym.h"

#include "query_processing_system/query/design_entities/entity_type/constant/ConstantSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/ProcedureSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableSynonym.h"

#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/StatementVariableNameAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/EntityVariableNameAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/statement_number/StatementStatementNumberAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/value/EntityValueAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/procedure_name/StatementProcedureNameAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/procedure_name/EntityProcedureNameAttribute.h"


TEST_CASE("Select Boolean") {
    QuerySelection sel;
    REQUIRE(sel.getAnswer(true) == std::list<std::string>({"TRUE"}));
    REQUIRE(sel.getAnswer(false) == std::list<std::string>({"FALSE"}));
}

TEST_CASE("Select Read Synonym") {
    std::list<std::string> ans;

    std::shared_ptr<ReadSynonym> s1 = std::make_shared<ReadSynonym>("s");
    s1->setStatementNumbers({1, 2, 3});
    s1->setVariableNameAttributes({{1, "a"}, {2, "b"}, {3, "c"}});

    QuerySelection sel1(s1);
    ans = sel1.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1", "2", "3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<StatementVariableNameAttribute> a1 = std::make_shared<StatementVariableNameAttribute>(s1);
    QuerySelection sel2(a1);
    ans = sel2.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a", "b", "c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<StatementStatementNumberAttribute> a2 = std::make_shared<StatementStatementNumberAttribute>(s1);
    QuerySelection sel3(a2);
    ans = sel3.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1", "2", "3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel4({s1, a1, a2});
    ans = sel4.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 a 1", "2 b 2", "3 c 3"}));
    REQUIRE(sel1.getAnswer(false).empty());
}

TEST_CASE("Select Print Synonym") {
    std::list<std::string> ans;

    std::shared_ptr<PrintSynonym> s1 = std::make_shared<PrintSynonym>("s");
    s1->setStatementNumbers({1, 2, 3});
    s1->setVariableNameAttributes({{1, "a"}, {2, "b"}, {3, "c"}});

    QuerySelection sel1(s1);
    ans = sel1.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1", "2", "3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<StatementVariableNameAttribute> a1 = std::make_shared<StatementVariableNameAttribute>(s1);
    QuerySelection sel2(a1);
    ans = sel2.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a", "b", "c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<StatementStatementNumberAttribute> a2 = std::make_shared<StatementStatementNumberAttribute>(s1);
    QuerySelection sel3(a2);
    ans = sel3.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1", "2", "3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel4({s1, a1, a2});
    ans = sel4.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 a 1", "2 b 2", "3 c 3"}));
    REQUIRE(sel1.getAnswer(false).empty());
}

TEST_CASE("Select Call Synonym") {
    std::list<std::string> ans;

    std::shared_ptr<CallSynonym> s1 = std::make_shared<CallSynonym>("s");
    s1->setStatementNumbers({1, 2, 3});
    s1->setProcedureNameAttributes({{1, "a"}, {2, "b"}, {3, "c"}});

    QuerySelection sel1(s1);
    ans = sel1.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1", "2", "3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<StatementProcedureNameAttribute> a1 = std::make_shared<StatementProcedureNameAttribute>(s1);
    QuerySelection sel2(a1);
    ans = sel2.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a", "b", "c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<StatementStatementNumberAttribute> a2 = std::make_shared<StatementStatementNumberAttribute>(s1);
    QuerySelection sel3(a2);
    ans = sel3.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1", "2", "3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel4({s1, a1, a2});
    ans = sel4.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 a 1", "2 b 2", "3 c 3"}));
    REQUIRE(sel1.getAnswer(false).empty());
}

TEST_CASE("Select Statement Synonym") {
    std::list<std::string> ans;

    std::shared_ptr<StatementSynonym> s1 = std::make_shared<StatementSynonym>("s");
    s1->setStatementNumbers({1, 2, 3});

    QuerySelection sel1(s1);
    ans = sel1.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1", "2", "3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<StatementStatementNumberAttribute> a1 = std::make_shared<StatementStatementNumberAttribute>(s1);
    QuerySelection sel2(a1);
    ans = sel2.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1", "2", "3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel3({s1, a1});
    ans = sel3.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 1", "2 2", "3 3"}));
    REQUIRE(sel1.getAnswer(false).empty());
}

TEST_CASE("Select Constant Synonym") {
    std::list<std::string> ans;

    std::shared_ptr<ConstantSynonym> s1 = std::make_shared<ConstantSynonym>("s");
    s1->setNames({"a", "b", "c"});
    s1->setValueAttributes({{"a", "a"}, {"b", "b"}, {"c", "c"}});

    QuerySelection sel1(s1);
    ans = sel1.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a", "b", "c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<EntityValueAttribute> a1 = std::make_shared<EntityValueAttribute>(s1);
    QuerySelection sel2(a1);
    ans = sel2.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a", "b", "c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel3({s1, a1});
    ans = sel3.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a a", "b b", "c c"}));
    REQUIRE(sel1.getAnswer(false).empty());
}

TEST_CASE("Select Variable Synonym") {
    std::list<std::string> ans;

    std::shared_ptr<VariableSynonym> s1 = std::make_shared<VariableSynonym>("s");
    s1->setNames({"a", "b", "c"});
    s1->setVariableNameAttributes({{"a", "a"}, {"b", "b"}, {"c", "c"}});

    QuerySelection sel1(s1);
    ans = sel1.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a", "b", "c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<EntityVariableNameAttribute> a1 = std::make_shared<EntityVariableNameAttribute>(s1);
    QuerySelection sel2(a1);
    ans = sel2.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a", "b", "c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel3({s1, a1});
    ans = sel3.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a a", "b b", "c c"}));
    REQUIRE(sel1.getAnswer(false).empty());
}

TEST_CASE("Select Procedure Synonym") {
    std::list<std::string> ans;

    std::shared_ptr<ProcedureSynonym> s1 = std::make_shared<ProcedureSynonym>("s");
    s1->setNames({"a", "b", "c"});
    s1->setProcedureNameAttributes({{"a", "a"}, {"b", "b"}, {"c", "c"}});

    QuerySelection sel1(s1);
    ans = sel1.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a", "b", "c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<EntityProcedureNameAttribute> a1 = std::make_shared<EntityProcedureNameAttribute>(s1);
    QuerySelection sel2(a1);
    ans = sel2.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a", "b", "c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel3({s1, a1});
    ans = sel3.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a a", "b b", "c c"}));
    REQUIRE(sel1.getAnswer(false).empty());
}

TEST_CASE("Select Intersected Synonyms") {
    std::list<std::string> ans;

    std::shared_ptr<VariableSynonym> s1 = std::make_shared<VariableSynonym>("s1");
    s1->setNames({"a", "b", "c"});
    s1->setVariableNameAttributes({{"a", "a"}, {"b", "b"}, {"c", "c"}});

    std::shared_ptr<ReadSynonym> s2 = std::make_shared<ReadSynonym>("s2");
    s2->setStatementNumbers({1, 2, 3});
    s2->setVariableNameAttributes({{1, "a"}, {2, "b"}, {3, "c"}});

    s1->intersectWithStatement(s2, {{"a", {1}}, {"b", {2}}, {"c", {3}}});
    s2->intersectWithEntity(s1, {{1, {"a"}}, {2, {"b"}}, {3, {"c"}}});

    QuerySelection sel1({s1, s2});
    ans = sel1.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a 1", "b 2", "c 3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    std::shared_ptr<EntityVariableNameAttribute> a1_1 = std::make_shared<EntityVariableNameAttribute>(s1);
    std::shared_ptr<StatementVariableNameAttribute> a2_1 = std::make_shared<StatementVariableNameAttribute>(s2);
    std::shared_ptr<StatementStatementNumberAttribute> a2_2 = std::make_shared<StatementStatementNumberAttribute>(s2);

    QuerySelection sel2({a1_1, a2_1});
    ans = sel2.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a a", "b b", "c c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel3({a1_1, a2_2});
    ans = sel3.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a 1", "b 2", "c 3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel4({a2_1, a2_2});
    ans = sel4.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a 1", "b 2", "c 3"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel5({s1, a2_1});
    ans = sel5.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a a", "b b", "c c"}));
    REQUIRE(sel1.getAnswer(false).empty());

    QuerySelection sel6({a1_1, s2});
    ans = sel6.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a 1", "b 2", "c 3"}));
    REQUIRE(sel1.getAnswer(false).empty());
}
