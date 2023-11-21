#include <unordered_set>
#include "program_knowledge_base/PKB.h"
#include "catch.hpp"
#include "TestPKBUtility.h"


TEST_CASE ("PKBSP unit tests") {
    auto PKBPair{createPKBPair()};
    auto sp{PKBPair.first};
    auto qps{PKBPair.second};

    SECTION("program_knowledge_base insertRead test") {
        sp.insertRead(1, "x");
        REQUIRE(qps.getAllReads() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base insertRead negative test") {
        REQUIRE(qps.getAllReads().empty());
    }

    SECTION("program_knowledge_base insertPrint test") {
        sp.insertPrint(1, "x");
        REQUIRE(qps.getAllPrints() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base insertPrint negative test") {
        REQUIRE(qps.getAllPrints().empty());
    }

    SECTION("program_knowledge_base insertAssign test") {
        sp.insertAssign(1, "x");
        REQUIRE(qps.getAllAssigns() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base insertAssign negative test") {
        REQUIRE(qps.getAllAssigns().empty());
    }

    SECTION("program_knowledge_base insertWhile test") {
        sp.insertWhile(1, std::vector<std::string>{"x"});
        REQUIRE(qps.getAllWhiles() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base insertWhile negative test") {
        REQUIRE(qps.getAllWhiles().empty());
    }

    SECTION("program_knowledge_base insertIf test") {
        sp.insertIf(1, std::vector<std::string>{"x"});
        REQUIRE(qps.getAllIfs() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base insertIf negative test") {
        REQUIRE(qps.getAllIfs().empty());
    }

    SECTION("program_knowledge_base insertProcedure test") {
        PKB pkb;
        pkb.insertProcedure("doSomething");
        REQUIRE(pkb.getAllProcedures() == std::unordered_set{std::string{"doSomething"}});
    }

    SECTION("program_knowledge_base insertProcedure negative test") {
        REQUIRE(qps.getAllProcedures().empty());
    }

    SECTION("program_knowledge_base insertCalls test") {
        PKB pkb;
        pkb.insertCall(1, "doSomething");
        REQUIRE(pkb.getAllCalls() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base insertCalls negative test") {
        REQUIRE(qps.getAllCalls().empty());
    }

    SECTION("program_knowledge_base insertCalls multiple test") {
        sp.insertCall(1, "doSomething");
        sp.insertCall(2, "doSomething");
        sp.insertCall(3, "doSomethingElse");
        REQUIRE(qps.getAllCalls() == std::unordered_set<int>{1, 2, 3});
        REQUIRE(qps.getProcedureCalledByCall(1) == "doSomething");
        REQUIRE(qps.getProcedureCalledByCall(2) == "doSomething");
        REQUIRE(qps.getProcedureCalledByCall(3) == "doSomethingElse");
    }

    SECTION("program_knowledge_base insertVariable test") {
        sp.insertVariable("name");
        REQUIRE(qps.getAllVariables() == std::unordered_set<std::string>{"name"});
    }

    SECTION("program_knowledge_base insertVariable negative test") {
        REQUIRE(qps.getAllVariables().empty());
    }

    SECTION("program_knowledge_base insertVariable multiple test") {
        sp.insertVariable("name");
        sp.insertVariable("surname");
        sp.insertVariable("name");
        REQUIRE(qps.getAllVariables() == std::unordered_set<std::string>{"name", "surname"});
    }

    SECTION("program_knowledge_base insertConstant test") {
        sp.insertConstant("1");
        REQUIRE(qps.getAllConstants() == std::unordered_set<std::string>{"1"});
    }

    SECTION("program_knowledge_base insertConstant negative test") {
        REQUIRE(qps.getAllConstants().empty());
    }

    SECTION("program_knowledge_base insertConstant multiple test") {
        sp.insertConstant("1");
        sp.insertConstant("2");
        sp.insertConstant("1");
        REQUIRE(qps.getAllConstants() == std::unordered_set<std::string>{"1", "2"});
    }

    SECTION("program_knowledge_base insertUses test") {
        sp.insertUses(1, "a");
        REQUIRE(qps.getVariablesUsedInStatement(1) == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base insertUses negative test") {
        REQUIRE(qps.getVariablesUsedInStatement(1).empty());
    }

    SECTION("program_knowledge_base insertUses multiple test") {
        // 1. x = a + b + c
        sp.insertUses(1, "a");
        sp.insertUses(1, "b");
        sp.insertUses(1, "c");
        // 2. print a
        sp.insertUses(2, "a");
        REQUIRE(qps.getVariablesUsedInStatement(1) == std::unordered_set<std::string>{"a", "b", "c"});
        REQUIRE(qps.getVariablesUsedInStatement(2) == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base insertUses procedure test") {
        sp.insertUses("doSomething", "a");
        REQUIRE(qps.getVariablesUsedInProcedure("doSomething") == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base insertUses procedure negative test") {
        REQUIRE(qps.getVariablesUsedInProcedure("doSomething").empty());
    }

    SECTION("program_knowledge_base insertUses procedure multiple test") {
        sp.insertUses("doSomething", "a");
        sp.insertUses("doSomething", "b");
        sp.insertUses("doSomething", "c");
        sp.insertUses("doSomething", "d");
        sp.insertUses("doSomethingElse", "a");
        REQUIRE(qps.getVariablesUsedInProcedure("doSomething") == std::unordered_set<std::string>{"a", "b", "c", "d"});
        REQUIRE(qps.getVariablesUsedInProcedure("doSomethingElse") == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base insertModifies test") {
        sp.insertModifies(1, "a");
        REQUIRE(qps.getVariablesModifiedInStatement(1) == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base insertModifies negative test") {
        REQUIRE(qps.getVariablesModifiedInStatement(1).empty());
    }

    SECTION("program_knowledge_base insertModifies multiple test") {
        sp.insertModifies(1, "a");
        sp.insertModifies(1, "b");
        sp.insertModifies(1, "c");
        sp.insertModifies(2, "a");
        REQUIRE(qps.getVariablesModifiedInStatement(1) == std::unordered_set<std::string>{"a", "b", "c"});
        REQUIRE(qps.getVariablesModifiedInStatement(2) == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base insertModifies procedure test") {
        sp.insertModifies("doSomething", "a");
        REQUIRE(qps.getVariablesModifiedInProcedure("doSomething") == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base insertModifies procedure negative test") {
        REQUIRE(qps.getVariablesModifiedInProcedure("doSomething").empty());
    }

    SECTION("program_knowledge_base insertFollows test") {
        sp.insertFollows(1, 2);
        REQUIRE(qps.getFollows(1, true) == std::unordered_set<int>{2});
        REQUIRE(qps.getFollows(1, false) == std::unordered_set<int>{2});
    }

    SECTION("program_knowledge_base insertFollows negative test") {
        REQUIRE(qps.getFollows(1, false).empty());
        REQUIRE(qps.getFollows(1, true).empty());
    }

    SECTION("program_knowledge_base insertFollows multiple test") {
        sp.insertFollows(1, 2);
        sp.insertFollows(2, 3);
        sp.insertFollows(3, 4);

        // Follows*
        sp.insertFollowsStar(1, 3);
        sp.insertFollowsStar(1, 4);
        sp.insertFollowsStar(2, 4);

        REQUIRE(qps.getFollows(1, true) == std::unordered_set<int>{2});
        REQUIRE(qps.getFollows(2, true) == std::unordered_set<int>{3});
        REQUIRE(qps.getFollows(3, true) == std::unordered_set<int>{4});
        REQUIRE(qps.getFollows(1, false) == std::unordered_set<int>{2, 3, 4});
        REQUIRE(qps.getFollows(2, false) == std::unordered_set<int>{3, 4});
        REQUIRE(qps.getFollows(3, false) == std::unordered_set<int>{4});
    }

    SECTION("program_knowledge_base insertParents test") {
        sp.insertParents(1, 2);
        REQUIRE(qps.getChild(1, true) == std::unordered_set<int>{2});
        REQUIRE(qps.getChild(1, false) == std::unordered_set<int>{2});
    }

    SECTION("program_knowledge_base insertParents negative test") {
        REQUIRE(qps.getChild(1, true).empty());
        REQUIRE(qps.getChild(1, false).empty());
    }

    SECTION("program_knowledge_base insertParents multiple test") {
        /**
         * 1. {
         *      2. {
         *           3. {
         *                4.
         *           5.
         *      6.
         *      7.
         *      8.
         *
         *
         */
        sp.insertParents(1, 2);
        sp.insertParents(2, 3);
        sp.insertParents(3, 4);
        sp.insertParents(2, 5);
        sp.insertParents(1, 6);
        sp.insertParents(1, 7);
        sp.insertParents(1, 8);

        // Parents*
        sp.insertParentsStar(1, 3);
        sp.insertParentsStar(1, 4);
        sp.insertParentsStar(2, 4);
        sp.insertParentsStar(1, 5);

        REQUIRE(qps.getChild(1, true) == std::unordered_set<int>{2, 6, 7, 8});
        REQUIRE(qps.getChild(2, true) == std::unordered_set<int>{3, 5});
        REQUIRE(qps.getChild(3, true) == std::unordered_set<int>{4});
        REQUIRE(qps.getChild(4, true).empty());
        REQUIRE(qps.getChild(5, true).empty());
        REQUIRE(qps.getChild(6, true).empty());
        REQUIRE(qps.getChild(7, true).empty());
        REQUIRE(qps.getChild(8, true).empty());

        REQUIRE(qps.getChild(1, false) == std::unordered_set<int>{2, 6, 7, 8, 3, 5, 4});
        REQUIRE(qps.getChild(2, false) == std::unordered_set<int>{3, 5, 4});
        REQUIRE(qps.getChild(3, false) == std::unordered_set<int>{4});
        REQUIRE(qps.getChild(4, false).empty());
        REQUIRE(qps.getChild(5, false).empty());
        REQUIRE(qps.getChild(6, false).empty());
        REQUIRE(qps.getChild(7, false).empty());
        REQUIRE(qps.getChild(8, false).empty());
    }

    SECTION("program_knowledge_base insertPattern test") {
        sp.insertPattern(1, std::vector<std::string>{"3", "4", "2", "*", "1", "5", "-", "2", "3"});
        REQUIRE(qps.hasMatchingPattern(1, std::vector<std::string>{"*", "1"}, false));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{"*", "1"}, false));
    }

    SECTION("program_knowledge_base insertPattern negative test") {
        REQUIRE(!qps.hasMatchingPattern(1, std::vector<std::string>{"*", "1"}, false));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{"*", "1"}, true));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{}, true));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{}, false));
    }

    SECTION("program_knowledge_base insertPattern multiple test") {
        sp.insertPattern(1, std::vector<std::string>{"3", "4", "2", "*", "1", "5", "-", "2", "3"});
        sp.insertPattern(2, std::vector<std::string>{"4", "3", "1", "*", "1", "5", "-", "2", "3"});
        REQUIRE(qps.hasMatchingPattern(1, std::vector<std::string>{"*", "1"}, false));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{"*", "1"}, true));
        REQUIRE(qps.hasMatchingPattern(1, std::vector<std::string>{"3", "4", "2", "*", "1", "5", "-", "2", "3"},
                                       false));
        REQUIRE(qps.hasMatchingPattern(1, std::vector<std::string>{"3", "4", "2", "*", "1", "5", "-", "2", "3"}, true));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{}, true));
        REQUIRE(qps.hasMatchingPattern(2, std::vector<std::string>{}, false));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{"4", "3", "1", "*", "1", "5", "-", "2", "3", "4"},
                                        false));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{"4", "3", "1", "*", "1", "5", "-", "2", "3", "4"},
                                        true));
        REQUIRE(qps.hasMatchingPattern(2, std::vector<std::string>{"3", "1", "*", "1", "5", "-", "2", "3"}, false));
        REQUIRE(qps.hasMatchingPattern(2, std::vector<std::string>{"4", "3", "1", "*", "1", "5", "-", "2"}, false));
    }

    SECTION("program_knowledge_base containsProcedure test") {
        sp.insertProcedure("main");
        REQUIRE(sp.containsProcedure("main"));
    }

    SECTION("program_knowledge_base containsProcedure negative test") {
        REQUIRE_FALSE(sp.containsProcedure("main"));
    }

    SECTION("program_knowledge_base containsProcedure invalid test") {
        sp.insertProcedure("main");
        REQUIRE_FALSE(sp.containsProcedure("sub1"));
    }

    SECTION("program_knowledge_base getCallsMap test") {
        sp.insertCalls("A", "B");
        sp.insertCalls("A", "C");
        sp.insertCalls("B", "C");
        sp.insertCalls("C", "D");

        auto calls{sp.getCallsMap()};
        REQUIRE_FALSE(calls.empty());
        REQUIRE(calls["A"] == std::unordered_set<std::string>{"B", "C"});
        REQUIRE(calls["B"] == std::unordered_set<std::string>{"C"});
        REQUIRE(calls["C"] == std::unordered_set<std::string>{"D"});
        REQUIRE(calls["D"].empty());
    }

    SECTION("program_knowledge_base getCallsMap negative test") {
        auto calls{sp.getCallsMap()};
        REQUIRE(calls.empty());
    }

    SECTION("program_knowledge_base getCallsReversedMap test") {
        sp.insertCalls("A", "B");
        sp.insertCalls("A", "C");
        sp.insertCalls("B", "C");
        sp.insertCalls("C", "D");

        auto calls{sp.getCallsReversedMap()};
        REQUIRE_FALSE(calls.empty());
        REQUIRE(calls["A"].empty());
        REQUIRE(calls["B"] == std::unordered_set<std::string>{"A"});
        REQUIRE(calls["C"] == std::unordered_set<std::string>{"B", "A"});
        REQUIRE(calls["D"] == std::unordered_set<std::string>{"C"});
    }

    SECTION("program_knowledge_base getCallsReversedMap negative test") {
        auto calls{sp.getCallsReversedMap()};
        REQUIRE(calls.empty());
    }

    SECTION("program_knowledge_base getCallsStarMap test") {
        sp.insertCalls("A", "B");
        sp.insertCalls("A", "C");
        sp.insertCalls("B", "C");
        sp.insertCalls("C", "D");

        sp.insertCallsStar("A", "D");
        sp.insertCallsStar("B", "D");

        auto calls{sp.getCallsStarMap()};
        REQUIRE_FALSE(calls.empty());
        REQUIRE(calls["A"] == std::unordered_set<std::string>{"B", "C", "D"});
        REQUIRE(calls["B"] == std::unordered_set<std::string>{"C", "D"});
        REQUIRE(calls["C"] == std::unordered_set<std::string>{"D"});
        REQUIRE(calls["D"].empty());
    }

    SECTION("program_knowledge_base getCallsStarMap negative test") {
        auto calls{sp.getCallsStarMap()};
        REQUIRE(calls.empty());
    }

    SECTION("program_knowledge_base getCallsStarReversedMap test") {
        sp.insertCalls("A", "B");
        sp.insertCalls("A", "C");
        sp.insertCalls("B", "C");
        sp.insertCalls("C", "D");

        sp.insertCallsStar("A", "D");
        sp.insertCallsStar("B", "D");

        auto calls{sp.getCallsStarReversedMap()};
        REQUIRE_FALSE(calls.empty());
        REQUIRE(calls["A"].empty());
        REQUIRE(calls["B"] == std::unordered_set<std::string>{"A"});
        REQUIRE(calls["C"] == std::unordered_set<std::string>{"B", "A"});
        REQUIRE(calls["D"] == std::unordered_set<std::string>{"C", "B", "A"});
    }

    SECTION("program_knowledge_base getCallsStarReversedMap negative test") {
        auto calls{sp.getCallsStarReversedMap()};
        REQUIRE(calls.empty());
    }

    SECTION("program_knowledge_base getUsesProceduresMap test") {
        sp.insertUses("main", "sub1");
        sp.insertUses("main", "sub2");
        sp.insertUses("sub1", "sub2");
        sp.insertUses("sub2", "sub3");

        auto proceduresUsesMap{sp.getUsesProceduresMap()};
        REQUIRE_FALSE(proceduresUsesMap.empty());
        REQUIRE(proceduresUsesMap["main"] == std::unordered_set<std::string>{"sub1", "sub2"});
        REQUIRE(proceduresUsesMap["main"] == std::unordered_set<std::string>{"sub1", "sub2"});
        REQUIRE(proceduresUsesMap["sub1"] == std::unordered_set<std::string>{"sub2"});
        REQUIRE(proceduresUsesMap["sub2"] == std::unordered_set<std::string>{"sub3"});
        REQUIRE(proceduresUsesMap["sub3"].empty());
    }

    SECTION("program_knowledge_base getUsesProceduresMap negative test") {
        auto proceduresUsesMap{sp.getUsesProceduresMap()};
        REQUIRE(proceduresUsesMap.empty());
    }

    SECTION("program_knowledge_base getModifiesProcedureMap test") {
        sp.insertModifies("main", "sub1");
        sp.insertModifies("main", "sub2");
        sp.insertModifies("sub1", "sub2");
        sp.insertModifies("sub2", "sub3");

        auto proceduresModifiesMap{sp.getModifiesProceduresMap()};
        REQUIRE_FALSE(proceduresModifiesMap.empty());
        REQUIRE(proceduresModifiesMap["main"] == std::unordered_set<std::string>{"sub1", "sub2"});
        REQUIRE(proceduresModifiesMap["main"] == std::unordered_set<std::string>{"sub1", "sub2"});
        REQUIRE(proceduresModifiesMap["sub1"] == std::unordered_set<std::string>{"sub2"});
        REQUIRE(proceduresModifiesMap["sub2"] == std::unordered_set<std::string>{"sub3"});
        REQUIRE(proceduresModifiesMap["sub3"].empty());
    }

    SECTION("program_knowledge_base getModifiesProceduresMap negative test") {
        auto proceduresModifiesMap{sp.getModifiesProceduresMap()};
        REQUIRE(proceduresModifiesMap.empty());
    }

    SECTION("program_knowledge_base insertNext test") {
        sp.insertNext(1, 2);
        REQUIRE(qps.getNext(1) == std::unordered_set<int>{2});
        REQUIRE(qps.getNext(1) == std::unordered_set<int>{2});
        REQUIRE(qps.getNext(2).empty());
        REQUIRE(qps.getNext(2).empty());
    }

    SECTION("program_knowledge_base insertNext negative test") {
        REQUIRE(qps.getNext(1).empty());
        REQUIRE(qps.getNext(2).empty());
    }

    SECTION("program_knowledge_base insertNext multiple test") {
        sp.insertNext(1, 2);
        sp.insertNext(2, 3);
        sp.insertNext(3, 4);
        sp.insertNext(3, 5);
        sp.insertNext(4, 5);

        REQUIRE(qps.getNext(1) == std::unordered_set<int>{2});
        REQUIRE(qps.getNext(2) == std::unordered_set<int>{3});
        REQUIRE(qps.getNext(3) == std::unordered_set<int>{4, 5});
        REQUIRE(qps.getNext(4) == std::unordered_set<int>{5});
        REQUIRE(qps.getNext(5).empty());
    }
}