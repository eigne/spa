#include "catch.hpp"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query/design_abstractions/with/With.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/ReadSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableSynonym.h"
#include "query_processing_system/query/design_entities/with_ref/ConstantWithRef.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/statement_number/StatementStatementNumberAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/StatementVariableNameAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/EntityVariableNameAttribute.h"


TEST_CASE("Two constants") {
    std::shared_ptr<ConstantWithRef> ref1 = std::make_shared<ConstantWithRef>("1");
    std::shared_ptr<ConstantWithRef> ref2 = std::make_shared<ConstantWithRef>("1");
    std::shared_ptr<ConstantWithRef> ref3 = std::make_shared<ConstantWithRef>("hello");
    std::shared_ptr<ConstantWithRef> ref4 = std::make_shared<ConstantWithRef>("hello");

    With with1(ref1, ref2, false);
    REQUIRE(with1.evaluateWith());
    With with2(ref1, ref2, true);
    REQUIRE_FALSE(with2.evaluateWith());

    With with3(ref3, ref4, false);
    REQUIRE(with3.evaluateWith());
    With with4(ref3, ref4, true);
    REQUIRE_FALSE(with4.evaluateWith());

    With with5(ref1, ref3, false);
    REQUIRE_FALSE(with5.evaluateWith());
    With with6(ref1, ref3, true);
    REQUIRE(with6.evaluateWith());
}

TEST_CASE("Constant and Attribute") {
    std::shared_ptr<ConstantWithRef> const1 = std::make_shared<ConstantWithRef>("1");
    std::shared_ptr<ConstantWithRef> const2 = std::make_shared<ConstantWithRef>("a");
    std::shared_ptr<ConstantWithRef> const3 = std::make_shared<ConstantWithRef>("b");

    std::shared_ptr<ReadSynonym> read  = std::make_shared<ReadSynonym>("r");
    std::shared_ptr<IStatementAttribute> stmtAttr = std::make_shared<StatementStatementNumberAttribute>(read);
    std::shared_ptr<StatementAttributeRef> attr1 = std::make_shared<StatementAttributeRef>(stmtAttr);

    std::shared_ptr<IStatementAttribute> varAttr = std::make_shared<StatementVariableNameAttribute>(read);
    std::shared_ptr<StatementAttributeRef> attr2 = std::make_shared<StatementAttributeRef>(varAttr);

    std::shared_ptr<VariableSynonym> variable = std::make_shared<VariableSynonym>("v");
    std::shared_ptr<IEntityAttribute> varNameAttr = std::make_shared<EntityVariableNameAttribute>(variable);
    std::shared_ptr<EntityAttributeRef> attr3 = std::make_shared<EntityAttributeRef>(varNameAttr);


    SECTION("Statement no Not") {
        read->setStatementNumbers({1, 2, 3, 4});
        read->setVariableNameAttributes({{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}});
        With with1(const1, attr1, false);
        REQUIRE(with1.evaluateWith());
        REQUIRE(read->getStatementNumbers() == std::unordered_set<int>({1}));
        REQUIRE(read->getVariableNameAttributes() == std::unordered_set<std::string>({"a"}));

        With with2(const2, attr2, false);
        REQUIRE(with2.evaluateWith());
        REQUIRE(read->getStatementNumbers() == std::unordered_set<int>({1}));
        REQUIRE(read->getVariableNameAttributes() == std::unordered_set<std::string>({"a"}));

        With with3(const3, attr2, false);
        REQUIRE_FALSE(with3.evaluateWith());
        REQUIRE(read->getStatementNumbers().empty());
        REQUIRE(read->getVariableNameAttributes().empty());
    }

    SECTION("Statement with Not") {
        read->setStatementNumbers({1, 2, 3, 4});
        read->setVariableNameAttributes({{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}});

        With with1(const1, attr1, true);
        REQUIRE(with1.evaluateWith());
        REQUIRE(read->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));
        REQUIRE(read->getVariableNameAttributes() == std::unordered_set<std::string>({"b", "c", "d"}));

        With with2(const3, attr2, true);
        REQUIRE(with2.evaluateWith());
        REQUIRE(read->getStatementNumbers() == std::unordered_set<int>({3, 4}));
        REQUIRE(read->getVariableNameAttributes() == std::unordered_set<std::string>({"c", "d"}));

        With with3(const2, attr2, false);
        REQUIRE_FALSE(with3.evaluateWith());
        REQUIRE(read->getStatementNumbers().empty());
        REQUIRE(read->getVariableNameAttributes().empty());
    }

    SECTION("Entity no Not") {
        variable->setNames({"a", "b", "c"});
        variable->setVariableNameAttributes({{"a", "a"}, {"b", "b"}, {"c", "c"}});

        With with1(const2, attr3, false);
        REQUIRE(with1.evaluateWith());
        REQUIRE(variable->getNames() == std::unordered_set<std::string>({"a"}));
        REQUIRE(variable->getVariableNameAttributes() == std::unordered_set<std::string>({"a"}));

        With with2(const3, attr3, false);
        REQUIRE_FALSE(with2.evaluateWith());
        REQUIRE(variable->getNames().empty());
        REQUIRE(variable->getVariableNameAttributes().empty());
    }

    SECTION("Entity with Not") {
        variable->setNames({"a", "b", "c"});
        variable->setVariableNameAttributes({{"a", "a"}, {"b", "b"}, {"c", "c"}});

        With with1(const2, attr3, true);
        REQUIRE(with1.evaluateWith());
        REQUIRE(variable->getNames() == std::unordered_set<std::string>({"b", "c"}));
        REQUIRE(variable->getVariableNameAttributes() == std::unordered_set<std::string>({"b", "c"}));

        With with2(const3, attr3, true);
        REQUIRE(with2.evaluateWith());
        REQUIRE(variable->getNames() == std::unordered_set<std::string>({"c"}));
        REQUIRE(variable->getVariableNameAttributes() == std::unordered_set<std::string>({"c"}));
    }
}

TEST_CASE("Two Attributes") {
    std::shared_ptr<ReadSynonym> read1  = std::make_shared<ReadSynonym>("r1");
    std::shared_ptr<IStatementAttribute> stmtAttr1 = std::make_shared<StatementStatementNumberAttribute>(read1);
    std::shared_ptr<StatementAttributeRef> read1Stmt = std::make_shared<StatementAttributeRef>(stmtAttr1);
    std::shared_ptr<IStatementAttribute> varAttr1 = std::make_shared<StatementVariableNameAttribute>(read1);
    std::shared_ptr<StatementAttributeRef> read1Var = std::make_shared<StatementAttributeRef>(varAttr1);

    std::shared_ptr<ReadSynonym> read2  = std::make_shared<ReadSynonym>("r2");
    std::shared_ptr<IStatementAttribute> stmtAttr2 = std::make_shared<StatementStatementNumberAttribute>(read2);
    std::shared_ptr<StatementAttributeRef> read2Stmt = std::make_shared<StatementAttributeRef>(stmtAttr2);
    std::shared_ptr<IStatementAttribute> varAttr2 = std::make_shared<StatementVariableNameAttribute>(read2);
    std::shared_ptr<StatementAttributeRef> read2Var = std::make_shared<StatementAttributeRef>(varAttr2);

    std::shared_ptr<VariableSynonym> variable = std::make_shared<VariableSynonym>("v");
    std::shared_ptr<IEntityAttribute> varNameAttr = std::make_shared<EntityVariableNameAttribute>(variable);
    std::shared_ptr<EntityAttributeRef> varVar = std::make_shared<EntityAttributeRef>(varNameAttr);

    QuerySelection selection = QuerySelection({read1, read2, variable});
    std::list<std::string> ans;

    SECTION("No nots") {
        read1->setStatementNumbers({1, 2, 3, 4});
        read1->setVariableNameAttributes({{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}});
        read2->setStatementNumbers({5, 6, 7, 8});
        read2->setVariableNameAttributes({{5, "d"}, {6, "c"}, {7, "b"}, {8, "a"}});
        variable->setNames({"a", "b", "c", "d"});
        variable->setVariableNameAttributes({{"a", "a"}, {"b", "b"}, {"c", "c"}, {"d", "d"}});

        With with1(read1Var, read2Var, false);
        REQUIRE(with1.evaluateWith());
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 8 a", "1 8 b", "1 8 c", "1 8 d",
                                               "2 7 a", "2 7 b", "2 7 c", "2 7 d",
                                               "3 6 a", "3 6 b", "3 6 c", "3 6 d",
                                               "4 5 a", "4 5 b", "4 5 c", "4 5 d"}));

        With with2 (read1Var, varVar, false);
        REQUIRE(with2.evaluateWith());
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 8 a", "2 7 b", "3 6 c", "4 5 d"}));

        With with3 (read2Var, varVar, false);
        REQUIRE(with3.evaluateWith());
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 8 a", "2 7 b", "3 6 c", "4 5 d"}));
    }

    SECTION("With not") {
        read1->setStatementNumbers({1, 2, 3, 4});
        read1->setVariableNameAttributes({{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}});
        read2->setStatementNumbers({5, 6, 7, 8});
        read2->setVariableNameAttributes({{5, "d"}, {6, "c"}, {7, "b"}, {8, "a"}});
        variable->setNames({"a", "b", "c", "d"});
        variable->setVariableNameAttributes({{"a", "a"}, {"b", "b"}, {"c", "c"}, {"d", "d"}});

        With with1(read1Var, read2Var, true);
        REQUIRE(with1.evaluateWith());
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 5 a", "1 5 b", "1 5 c", "1 5 d",
                                               "1 6 a", "1 6 b", "1 6 c", "1 6 d",
                                               "1 7 a", "1 7 b", "1 7 c", "1 7 d",
                                               "2 5 a", "2 5 b", "2 5 c", "2 5 d",
                                               "2 6 a", "2 6 b", "2 6 c", "2 6 d",
                                               "2 8 a", "2 8 b", "2 8 c", "2 8 d",
                                               "3 5 a", "3 5 b", "3 5 c", "3 5 d",
                                               "3 7 a", "3 7 b", "3 7 c", "3 7 d",
                                               "3 8 a", "3 8 b", "3 8 c", "3 8 d",
                                               "4 6 a", "4 6 b", "4 6 c", "4 6 d",
                                               "4 7 a", "4 7 b", "4 7 c", "4 7 d",
                                               "4 8 a", "4 8 b", "4 8 c", "4 8 d"}));

        With with2 (read1Var, varVar, true);
        REQUIRE(with2.evaluateWith());
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 5 b", "1 5 c", "1 5 d",
                                               "1 6 b", "1 6 c", "1 6 d",
                                               "1 7 b", "1 7 c", "1 7 d",
                                               "2 5 a", "2 5 c", "2 5 d",
                                               "2 6 a", "2 6 c", "2 6 d",
                                               "2 8 a", "2 8 c", "2 8 d",
                                               "3 5 a", "3 5 b", "3 5 d",
                                               "3 7 a", "3 7 b", "3 7 d",
                                               "3 8 a", "3 8 b", "3 8 d",
                                               "4 6 a", "4 6 b", "4 6 c",
                                               "4 7 a", "4 7 b", "4 7 c",
                                               "4 8 a", "4 8 b", "4 8 c"}));

        With with3 (read2Var, varVar, true);
        REQUIRE(with3.evaluateWith());
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 5 b", "1 5 c",
                                               "1 6 b", "1 6 d",
                                               "1 7 c", "1 7 d",
                                               "2 5 a", "2 5 c",
                                               "2 6 a", "2 6 d",
                                               "2 8 c", "2 8 d",
                                               "3 5 a", "3 5 b",
                                               "3 7 a", "3 7 d",
                                               "3 8 b", "3 8 d",
                                               "4 6 a", "4 6 b",
                                               "4 7 a", "4 7 c",
                                               "4 8 b", "4 8 c"}));
    }
}
