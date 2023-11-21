#include <unordered_set>
#include "catch.hpp"
#include "TestPKBUtility.h"
#include "program_knowledge_base/exceptions/PKBException.h"


TEST_CASE("PKBQPS unit tests") {
    auto PKBPair{createPKBPair()};
    auto sp{PKBPair.first};
    auto qps{PKBPair.second};

    SECTION("program_knowledge_base getAllReads test") {
        sp.insertRead(1, "x");
        sp.insertRead(2, "x");
        sp.insertRead(3, "y");
        REQUIRE(qps.getAllReads() == std::unordered_set<int>{1, 2, 3});
    }

    SECTION("program_knowledge_base getAllPrints test") {
        sp.insertPrint(1, "x");
        sp.insertPrint(2, "x");
        sp.insertPrint(3, "y");
        REQUIRE(qps.getAllPrints() == std::unordered_set<int>{1, 2, 3});
    }

    SECTION("program_knowledge_base getAllAssigns test") {
        sp.insertAssign(1, "x");
        sp.insertAssign(2, "x");
        sp.insertAssign(3, "y");
        REQUIRE(qps.getAllAssigns() == std::unordered_set<int>{1, 2, 3});
    }

    SECTION("program_knowledge_base getAllStatements test") {
        sp.insertRead(1, "x");
        sp.insertRead(2, "y");
        sp.insertAssign(3, "z");
        REQUIRE(qps.getAllStatements() == std::unordered_set<int>{1, 2, 3});
    }

    SECTION("program_knowledge_base getAllCalls test") {
        sp.insertCall(1, "readName");
        sp.insertCall(1, "readSurname");
        sp.insertCall(2, "changeName");
        REQUIRE(qps.getAllCalls() == std::unordered_set<int>{1, 2});
    }

    SECTION("program_knowledge_base getAllProcedures test") {
        sp.insertProcedure("procA");
        sp.insertProcedure("procB");
        sp.insertProcedure("procC");
        REQUIRE(qps.getAllProcedures() == std::unordered_set<std::string>{"procA", "procB", "procC"});
    }

    SECTION("program_knowledge_base getAllVariables test") {
        sp.insertVariable("a");
        sp.insertVariable("b");
        sp.insertVariable("c");
        REQUIRE(qps.getAllVariables() == std::unordered_set<std::string>{"a", "b", "c"});
    }

    SECTION("program_knowledge_base getAllConstants test") {
        sp.insertConstant("3");
        sp.insertConstant("4");
        sp.insertConstant("5");
        REQUIRE(qps.getAllConstants() == std::unordered_set<std::string>{"3", "4", "5"});
    }

    SECTION("program_knowledge_base getVariablesUsedInStatement test") {
        // Statement number 1
        sp.insertUses(1, "a");
        // Procedure named 1
        sp.insertUses("1", "b");
        REQUIRE(qps.getVariablesUsedInStatement(1) == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base getVariablesUsedInStatement multiple test") {
        // 1. x = a + b + c
        sp.insertUses(1, "a");
        sp.insertUses(1, "b");
        sp.insertUses(1, "c");
        // 2. print a
        sp.insertUses(2, "a");
        REQUIRE(qps.getVariablesUsedInStatement(1) == std::unordered_set<std::string>{"a", "b", "c"});
        REQUIRE(qps.getVariablesUsedInStatement(2) == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base getStatementsThatUseVariable test") {
        // Statement number 1
        sp.insertUses(1, "a");
        // Procedure named 2
        sp.insertUses("2", "a");
        REQUIRE(qps.getStatementsThatUseVariable("a") == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getStatementsThatUseVariable multiple test") {
        sp.insertUses(1, "a");
        sp.insertUses(2, "a");
        sp.insertUses(3, "a");
        sp.insertUses(3, "b");
        sp.insertUses(4, "b");
        REQUIRE(qps.getStatementsThatUseVariable("a") == std::unordered_set<int>{1, 2, 3});
        REQUIRE(qps.getStatementsThatUseVariable("b") == std::unordered_set<int>{3, 4});
    }

    SECTION("program_knowledge_base getStatementsThatUseAnyVariable test") {
        // Statement number
        sp.insertUses(1, "a");
        // Procedure named '2'
        sp.insertUses("2", "b");
        REQUIRE(qps.getStatementsThatUseAnyVariable() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getStatementsThatUseAnyVariable multiple test") {
        sp.insertUses(1, "a");
        sp.insertUses(1, "b");
        sp.insertUses(2, "a");
        sp.insertUses(3, "c");
        REQUIRE(qps.getStatementsThatUseAnyVariable() == std::unordered_set<int>{1, 2, 3});
    }

    SECTION("program_knowledge_base getVariablesUsedInProcedure test") {
        sp.insertUses("doSomething", "a");
        REQUIRE(qps.getVariablesUsedInProcedure("doSomething") == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base getVariablesUsedInProcedure incorrectly named procedure test") {
        sp.insertUses("1", "a");
        sp.insertUses(1, "b");
        REQUIRE(qps.getVariablesUsedInProcedure("1") == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base getVariablesUsedInProcedure multiple test") {
        sp.insertUses("doSomething", "a");
        sp.insertUses("doSomething", "b");
        sp.insertUses("doSomething", "c");
        sp.insertUses("doSomethingElse", "b");
        REQUIRE(qps.getVariablesUsedInProcedure("doSomething") == std::unordered_set<std::string>{"a", "b", "c"});
        REQUIRE(qps.getVariablesUsedInProcedure("doSomethingElse") == std::unordered_set<std::string>{"b"});
    }

    SECTION("program_knowledge_base getProceduresThatUseVariable test") {
        sp.insertUses("doSomething", "a");
        sp.insertUses(1, "a");
        REQUIRE(qps.getProceduresThatUseVariable("a") == std::unordered_set<std::string>{"doSomething"});
    }

    SECTION("program_knowledge_base getProceduresThatUseVariable multiple test") {
        sp.insertUses("doSomething", "a");
        sp.insertUses("doSomething", "b");
        sp.insertUses("doSomethingElse", "a");
        sp.insertUses(1, "a");
        REQUIRE(qps.getProceduresThatUseVariable("a") ==
                std::unordered_set<std::string>{"doSomething", "doSomethingElse"});
    }

    SECTION("program_knowledge_base getProceduresThatUseAnyVariable test") {
        sp.insertUses("doSomething", "a");
        sp.insertUses(1, "a");
        sp.insertUses(2, "b");
        REQUIRE(qps.getProceduresThatUseAnyVariable() == std::unordered_set<std::string>{"doSomething"});
    }

    SECTION("program_knowledge_base getProceduresThatUseAnyVariable multiple test") {
        sp.insertUses("doSomething", "a");
        sp.insertUses("doSomething", "b");
        sp.insertUses("doSomethingElse", "c");
        REQUIRE(qps.getProceduresThatUseAnyVariable() ==
                std::unordered_set<std::string>{"doSomething", "doSomethingElse"});
    }

    SECTION("program_knowledge_base getVariablesModifiedInStatement test") {
        sp.insertModifies(1, "x");
        sp.insertModifies("1", "y");
        REQUIRE(qps.getVariablesModifiedInStatement(1) == std::unordered_set<std::string>{"x"});
    }

    SECTION("program_knowledge_base getVariablesModifiedInStatement multiple test") {
        sp.insertModifies(1, "x");
        sp.insertModifies(1, "b");
        sp.insertModifies(1, "c");
        REQUIRE(qps.getVariablesModifiedInStatement(1) == std::unordered_set<std::string>{"x", "b", "c"});
    }

    SECTION("program_knowledge_base getVariablesModifiedInProcedure test") {
        sp.insertModifies("1", "a");
        sp.insertModifies(1, "a");
        REQUIRE(qps.getVariablesModifiedInProcedure("1") == std::unordered_set<std::string>{"a"});
    }

    SECTION("program_knowledge_base getVariablesModifiedInProcedure multiple test") {
        sp.insertModifies("doSomething", "a");
        sp.insertModifies("doSomething", "b");
        sp.insertModifies("doSomething", "c");
        sp.insertModifies("doSomethingElse", "d");
        sp.insertModifies(1, "e");
        REQUIRE(qps.getVariablesModifiedInProcedure("doSomething") == std::unordered_set<std::string>{"a", "b", "c"});
    }

    SECTION("program_knowledge_base getStatementsThatModifyVariable test") {
        sp.insertModifies(1, "x");
        sp.insertModifies("2", "x");
        REQUIRE(qps.getStatementsThatModifyVariable("x") == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getStatementsThatModifyVariable multiple test") {
        sp.insertModifies(1, "a");
        sp.insertModifies(1, "b");
        sp.insertModifies(2, "a");
        sp.insertModifies(3, "a");
        sp.insertModifies(4, "c");
        sp.insertModifies("doSomething", "a");
        REQUIRE(qps.getStatementsThatModifyVariable("a") == std::unordered_set<int>{1, 2, 3});
    }

    SECTION("program_knowledge_base getStatementsThatModifyAnyVariable test") {
        sp.insertModifies(1, "a");
        REQUIRE(qps.getStatementsThatModifyAnyVariable() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getStatementsThatModifyAnyVariable multiple test") {
        sp.insertModifies(1, "a");
        sp.insertModifies("1", "a");
        sp.insertModifies(2, "a");
        sp.insertModifies(2, "b");
        sp.insertModifies(3, "c");
        REQUIRE(qps.getStatementsThatModifyAnyVariable() == std::unordered_set<int>{1, 2, 3});
    }

    SECTION("program_knowledge_base getProceduresThatModifyVariable test") {
        sp.insertModifies("doSomething", "a");
        sp.insertModifies(1, "a");
        REQUIRE(qps.getProceduresThatModifyVariable("a") == std::unordered_set<std::string>{"doSomething"});
    }

    SECTION("program_knowledge_base getProceduresThatModifyVariable multiple test") {
        sp.insertModifies("doSomething", "a");
        sp.insertModifies("doSomething", "b");
        sp.insertModifies("doSomethingElse", "a");
        sp.insertModifies("doSomethingElseElse", "c");
        sp.insertModifies(1, "a");
        REQUIRE(qps.getProceduresThatModifyVariable("a") ==
                std::unordered_set<std::string>{"doSomething", "doSomethingElse"});
    }

    SECTION("program_knowledge_base getProceduresThatModifyAnyVariable test") {
        sp.insertModifies("doSomething", "a");
        sp.insertModifies(1, "a");
        REQUIRE(qps.getProceduresThatModifyAnyVariable() == std::unordered_set<std::string>{"doSomething"});
    }

    SECTION("program_knowledge_base getProceduresThatModifyAnyVariable multiple test") {
        sp.insertModifies("doSomething", "a");
        sp.insertModifies("doSomething", "b");
        sp.insertModifies("doSomethingElse", "c");
        sp.insertModifies(1, "a");
        sp.insertModifies(1, "d");
        REQUIRE(qps.getProceduresThatModifyAnyVariable() ==
                std::unordered_set<std::string>{"doSomething", "doSomethingElse"});
    }

    SECTION("program_knowledge_base getBefore test") {
        sp.insertFollows(1, 2);
        REQUIRE(qps.getBefore(2, true) == std::unordered_set<int>{1});
        REQUIRE(qps.getBefore(2, false) == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getBefore negative test") {
        REQUIRE(qps.getBefore(1, true).empty());
        REQUIRE(qps.getBefore(1, false).empty());
    }

    SECTION("program_knowledge_base getBefore multiple test") {
        sp.insertFollows(1, 2);
        sp.insertFollows(2, 3);
        sp.insertFollows(3, 4);

        // Follows*
        sp.insertFollowsStar(1, 3);
        sp.insertFollowsStar(1, 4);
        sp.insertFollowsStar(2, 4);

        REQUIRE(qps.getBefore(4, true) == std::unordered_set{3});
        REQUIRE(qps.getBefore(3, true) == std::unordered_set{2});
        REQUIRE(qps.getBefore(2, true) == std::unordered_set{1});
        REQUIRE(qps.getBefore(1, true).empty());
        REQUIRE(qps.getBefore(4, false) == std::unordered_set{3, 2, 1});
        REQUIRE(qps.getBefore(3, false) == std::unordered_set{2, 1});
        REQUIRE(qps.getBefore(2, false) == std::unordered_set{1});
        REQUIRE(qps.getBefore(1, false).empty());
    }

    SECTION("program_knowledge_base getAllWithFollows test") {
        sp.insertFollows(1, 2);
        REQUIRE(qps.getAllWithFollows() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getAllWithFollows negative test") {
        REQUIRE(qps.getAllWithFollows().empty());
    }

    SECTION("program_knowledge_base getAllWithFollows multiple test") {
        sp.insertFollows(1, 2);
        sp.insertFollows(2, 3);
        sp.insertFollows(3, 4);

        REQUIRE(qps.getAllWithFollows() == std::unordered_set<int>{1, 2, 3});
    }

    SECTION("program_knowledge_base getAllWithBefore test") {
        sp.insertFollows(1, 2);

        REQUIRE(qps.getAllWithBefore() == std::unordered_set<int>{2});
    }

    SECTION("program_knowledge_base getAllWithBefore negative test") {
        REQUIRE(qps.getAllWithBefore().empty());
    }

    SECTION("program_knowledge_base getAllWithBefore multiple test") {
        sp.insertFollows(1, 2);
        sp.insertFollows(2, 3);
        sp.insertFollows(3, 4);

        REQUIRE(qps.getAllWithBefore() == std::unordered_set<int>{2, 3, 4});
    }

    SECTION("program_knowledge_base getParent test") {
        sp.insertParents(1, 2);
        REQUIRE(qps.getParent(2, true) == std::unordered_set<int>{1});
        REQUIRE(qps.getParent(2, false) == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getParent negative test") {
        REQUIRE(qps.getParent(1, true).empty());
        REQUIRE(qps.getParent(1, false).empty());
    }

    SECTION("program_knowledge_base getParent multiple test") {
        sp.insertParents(1, 2);
        sp.insertParents(2, 3);
        sp.insertParents(3, 4);
        sp.insertParents(2, 5);
        sp.insertParents(1, 6);
        sp.insertParents(1, 7);
        sp.insertParents(1, 8);

        //Parents*
        sp.insertParentsStar(1, 3);
        sp.insertParentsStar(1, 4);
        sp.insertParentsStar(2, 4);
        sp.insertParentsStar(1, 5);

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
        REQUIRE(qps.getParent(1, true).empty());
        REQUIRE(qps.getParent(2, true) == std::unordered_set<int>{1});
        REQUIRE(qps.getParent(3, true) == std::unordered_set<int>{2});
        REQUIRE(qps.getParent(4, true) == std::unordered_set<int>{3});
        REQUIRE(qps.getParent(5, true) == std::unordered_set<int>{2});
        REQUIRE(qps.getParent(6, true) == std::unordered_set<int>{1});
        REQUIRE(qps.getParent(7, true) == std::unordered_set<int>{1});
        REQUIRE(qps.getParent(8, true) == std::unordered_set<int>{1});

        REQUIRE(qps.getParent(1, false).empty());
        REQUIRE(qps.getParent(2, false) == std::unordered_set<int>{1});
        REQUIRE(qps.getParent(3, false) == std::unordered_set<int>{2, 1});
        REQUIRE(qps.getParent(4, false) == std::unordered_set<int>{3, 2, 1});
        REQUIRE(qps.getParent(5, false) == std::unordered_set<int>{2, 1});
        REQUIRE(qps.getParent(7, false) == std::unordered_set<int>{1});
        REQUIRE(qps.getParent(8, false) == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getAllWithChild test") {
        sp.insertParents(1, 2);
        REQUIRE(qps.getAllWithChild() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getAllWithChild negative test") {
        REQUIRE(qps.getAllWithChild().empty());
    }

    SECTION("program_knowledge_base getAllWithChild multiple test") {
        sp.insertParents(1, 2);
        sp.insertParents(2, 3);
        sp.insertParents(3, 4);
        sp.insertParents(2, 5);
        sp.insertParents(1, 6);
        sp.insertParents(1, 7);
        sp.insertParents(1, 8);

        // Parents*
        sp.insertParents(1, 3);
        sp.insertParents(1, 4);
        sp.insertParents(2, 4);
        sp.insertParents(1, 5);

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

        REQUIRE(qps.getAllWithChild() == std::unordered_set<int>{1, 2, 3});
    }

    SECTION("program_knowledge_base getAllWithParent test") {
        sp.insertParents(1, 2);
        REQUIRE(qps.getAllWithParent() == std::unordered_set<int>{2});
    }

    SECTION("program_knowledge_base getAllWithParent negative test") {
        REQUIRE(qps.getAllWithParent().empty());
    }

    SECTION("program_knowledge_base getAllWithParent multiple test") {
        sp.insertParents(1, 2);
        sp.insertParents(2, 3);
        sp.insertParents(3, 4);
        sp.insertParents(2, 5);
        sp.insertParents(1, 6);
        sp.insertParents(1, 7);
        sp.insertParents(1, 8);
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

        REQUIRE(qps.getAllWithParent() == std::unordered_set<int>{4, 3, 5, 2, 6, 7, 8});
    }

    SECTION("program_knowledge_base getProceduresCalledBy test") {
        /*
         * main {
         *      sub1
         *      sub2
         * }
         * sub2 {
         *      sub3
         * }
         * sub3 {
         *      sub4
         * }
         */

        sp.insertCall(1, "sub1");
        sp.insertCall(2, "sub2");
        sp.insertCall(3, "sub3");
        sp.insertCall(4, "sub4");

        // Calls
        sp.insertCalls("main", "sub1");
        sp.insertCalls("main", "sub2");
        sp.insertCalls("sub2", "sub3");
        sp.insertCalls("sub3", "sub4");

        // Calls*
        sp.insertCallsStar("main", "sub3");
        sp.insertCallsStar("main", "sub4");
        sp.insertCallsStar("sub2", "sub4");

        REQUIRE(qps.getProceduresCalledBy("main", true) == std::unordered_set<std::string>{"sub1", "sub2"});
        REQUIRE(qps.getProceduresCalledBy("main", false) ==
                std::unordered_set<std::string>{"sub1", "sub2", "sub3", "sub4"});
        REQUIRE(qps.getProceduresCalledBy("sub1", true).empty());
        REQUIRE(qps.getProceduresCalledBy("sub1", false).empty());
        REQUIRE(qps.getProceduresCalledBy("sub2", true) == std::unordered_set<std::string>{"sub3"});
        REQUIRE(qps.getProceduresCalledBy("sub2", false) == std::unordered_set<std::string>{"sub3", "sub4"});
        REQUIRE(qps.getProceduresCalledBy("sub3", true) == std::unordered_set<std::string>{"sub4"});
        REQUIRE(qps.getProceduresCalledBy("sub3", false) == std::unordered_set<std::string>{"sub4"});
        REQUIRE(qps.getProceduresCalledBy("sub4", true).empty());
        REQUIRE(qps.getProceduresCalledBy("sub4", false).empty());
    }

    SECTION("program_knowledge_base getProceduresCalling test") {
        /*
         * main {
         *      sub1
         *      sub2
         * }
         * sub2 {
         *      sub3
         * }
         * sub3 {
         *      sub4
         * }
         */

        sp.insertCall(1, "sub1");
        sp.insertCall(2, "sub2");
        sp.insertCall(3, "sub3");
        sp.insertCall(4, "sub4");

        // Calls
        sp.insertCalls("main", "sub1");
        sp.insertCalls("main", "sub2");
        sp.insertCalls("sub2", "sub3");
        sp.insertCalls("sub3", "sub4");

        // Calls*
        sp.insertCallsStar("main", "sub3");
        sp.insertCallsStar("main", "sub4");
        sp.insertCallsStar("sub2", "sub4");

        REQUIRE(qps.getProceduresCalling("sub1", true) == std::unordered_set<std::string>{"main"});
        REQUIRE(qps.getProceduresCalling("sub1", false) == std::unordered_set<std::string>{"main"});
        REQUIRE(qps.getProceduresCalling("sub2", true) == std::unordered_set<std::string>{"main"});
        REQUIRE(qps.getProceduresCalling("sub2", false) == std::unordered_set<std::string>{"main"});
        REQUIRE(qps.getProceduresCalling("sub3", true) == std::unordered_set<std::string>{"sub2"});
        REQUIRE(qps.getProceduresCalling("sub3", false) == std::unordered_set<std::string>{"main", "sub2"});
        REQUIRE(qps.getProceduresCalling("sub4", true) == std::unordered_set<std::string>{"sub3"});
        REQUIRE(qps.getProceduresCalling("sub4", false) == std::unordered_set<std::string>{"main", "sub2", "sub3"});
    }

    SECTION("program_knowledge_base doCallsExist test") {
        sp.insertCall(1, "sub1");
        REQUIRE(qps.doCallsExist() == true);
    }

    SECTION("program_knowledge_base doCallsExist negative test") {
        REQUIRE(qps.doCallsExist() == false);

        sp.insertCall(1, "sub1");
        REQUIRE(qps.doCallsExist() == true);
    }

    SECTION("program_knowledge_base getProcedureCalledByCall test") {
        sp.insertCall(1, "sub1");
        REQUIRE(qps.getProcedureCalledByCall(1) == "sub1");
    }

    SECTION("program_knowledge_base getProcedureCalledByCall negative test") {
        REQUIRE_THROWS_AS(qps.getProcedureCalledByCall(1), PKBException);
    }

    SECTION("program_knowledge_base getProcedureCalledByCall multiple test") {
        sp.insertCall(1, "sub1");
        sp.insertCall(2, "sub2");
        sp.insertCall(3, "sub3");
        REQUIRE(qps.getProcedureCalledByCall(1) == "sub1");
        REQUIRE(qps.getProcedureCalledByCall(2) == "sub2");
        REQUIRE(qps.getProcedureCalledByCall(3) == "sub3");
    }

    SECTION("program_knowledge_base while test") {
        sp.insertWhile(1, std::vector<std::string>{"x"});
        sp.insertWhile(1, std::vector<std::string>{"y"});
        sp.insertWhile(2, std::vector<std::string>{"a"});

        REQUIRE(qps.getVariablesUsedInWhile(1) == std::unordered_set<std::string>{"x", "y"});
        REQUIRE(qps.getVariablesUsedInWhile(2) == std::unordered_set<std::string>{"a"});
        REQUIRE(qps.getWhilesThatUseAnyVariable() == std::unordered_set<int>{1, 2});
        REQUIRE(qps.getWhilesThatUseVariable("x") == std::unordered_set<int>{1});
        REQUIRE(qps.getWhilesThatUseVariable("y") == std::unordered_set<int>{1});
        REQUIRE(qps.getWhilesThatUseVariable("a") == std::unordered_set<int>{2});
        REQUIRE(qps.getVariablesUsedInWhile(3).empty());
    }

    SECTION("program_knowledge_base doesIfUseVariable test") {
        sp.insertIf(1, std::vector<std::string>{"x"});
        sp.insertIf(1, std::vector<std::string>{"y"});
        sp.insertIf(2, std::vector<std::string>{"a"});

        REQUIRE(qps.getVariablesUsedInIf(1) == std::unordered_set<std::string>{"x", "y"});
        REQUIRE(qps.getVariablesUsedInIf(2) == std::unordered_set<std::string>{"a"});
        REQUIRE(qps.getIfsThatUseAnyVariable() == std::unordered_set<int>{1, 2});
        REQUIRE(qps.getIfsThatUseVariable("x") == std::unordered_set<int>{1});
        REQUIRE(qps.getIfsThatUseVariable("y") == std::unordered_set<int>{1});
        REQUIRE(qps.getIfsThatUseVariable("a") == std::unordered_set<int>{2});
        REQUIRE(qps.getVariablesUsedInIf(3).empty());
    }

    SECTION("program_knowledge_base hasMatchingPattern test") {
        sp.insertPattern(1, std::vector<std::string>{"3", "4", "2", "*", "1", "5", "-", "2", "3"});
        REQUIRE(qps.hasMatchingPattern(1, std::vector<std::string>{"*", "1"}, false));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{"*", "1"}, false));
    }

    SECTION("program_knowledge_base hasMatchingPattern negative test") {
        REQUIRE(!qps.hasMatchingPattern(1, std::vector<std::string>{"*", "1"}, false));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{"*", "1"}, true));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{}, true));
        REQUIRE(!qps.hasMatchingPattern(2, std::vector<std::string>{}, false));
    }

    SECTION("program_knowledge_base hasMatchingPattern multiple test") {
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

    SECTION("program_knowledge_base getProceduresThatCall test") {
        sp.insertCalls("main", "sub1");
        sp.insertCalls("main", "sub2");
        sp.insertCalls("sub2", "sub3");
        sp.insertCalls("sub3", "sub4");

        REQUIRE(qps.getProceduresThatCall() == std::unordered_set<std::string>{"main", "sub2", "sub3"});
    }

    SECTION("program_knowledge_base getProceduresThatCall with Calls* test") {
        sp.insertCalls("main", "sub1");
        sp.insertCalls("main", "sub2");
        sp.insertCalls("sub2", "sub3");
        sp.insertCalls("sub3", "sub4");

        sp.insertCallsStar("main", "sub3");
        sp.insertCallsStar("main", "sub4");
        sp.insertCallsStar("sub2", "sub4");

        REQUIRE(qps.getProceduresThatCall() == std::unordered_set<std::string>{"main", "sub2", "sub3"});
    }

    SECTION("program_knowledge_base getProceduresThatCall negative test") {
        REQUIRE(qps.getProceduresThatCall().empty());
    }

    SECTION("program_knowledge_base getAllCalledProcedures test") {
        sp.insertCalls("main", "sub1");
        sp.insertCalls("main", "sub2");
        sp.insertCalls("sub2", "sub3");
        sp.insertCalls("sub3", "sub4");

        REQUIRE(qps.getAllCalledProcedures() == std::unordered_set<std::string>{"sub1", "sub2", "sub3", "sub4"});
    }

    SECTION("program_knowledge_base getAllCalledProcedures with Calls* test") {
        sp.insertCalls("main", "sub1");
        sp.insertCalls("main", "sub2");
        sp.insertCalls("sub2", "sub3");
        sp.insertCalls("sub3", "sub4");

        REQUIRE(qps.getAllCalledProcedures() == std::unordered_set<std::string>{"sub1", "sub2", "sub3", "sub4"});
    }

    SECTION("program_knowledge_base getNext test") {
        sp.insertNext(1, 2);
        REQUIRE(qps.getNext(1) == std::unordered_set<int>{2});
        REQUIRE(qps.getNext(1) == std::unordered_set<int>{2});
        REQUIRE(qps.getNext(2).empty());
        REQUIRE(qps.getNext(2).empty());
    }

    SECTION("program_knowledge_base getNext negative test") {
        REQUIRE(qps.getNext(1).empty());
    }

    SECTION("program_knowledge_base getNext invalid test") {
        sp.insertNext(1, 2);
        REQUIRE(qps.getNext(2).empty());
        REQUIRE(qps.getNext(3).empty());
    }

    SECTION("program_knowledge_base getNext multiple test") {
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

    SECTION("program_knowledge_base getAllWithNext test") {
        sp.insertNext(1, 2);
        REQUIRE(qps.getAllWithNext() == std::unordered_set<int>{1});
    }

    SECTION("program_knowledge_base getAllWithNext negative test") {
        REQUIRE(qps.getAllWithNext().empty());
    }

    SECTION("program_knowledge_base getAllWithNext multiple test") {
        sp.insertNext(1, 2);
        sp.insertNext(2, 3);
        sp.insertNext(3, 4);
        sp.insertNext(3, 5);
        sp.insertNext(4, 5);

        REQUIRE(qps.getAllWithNext() == std::unordered_set<int>{1, 2, 3, 4});
    }

    SECTION("program_knowledge_base getAllWithPrevious test") {
        sp.insertNext(1, 2);
        REQUIRE(qps.getAllWithPrevious() == std::unordered_set<int>{2});
    }

    SECTION("program_knowledge_base getAllWithPrevious negative test") {
        REQUIRE(qps.getAllWithPrevious().empty());
    }

    SECTION("program_knowledge_base getAllWithPrevious multiple test") {
        sp.insertNext(1, 2);
        sp.insertNext(2, 3);
        sp.insertNext(3, 4);
        sp.insertNext(3, 5);
        sp.insertNext(4, 5);

        REQUIRE(qps.getAllWithPrevious() == std::unordered_set<int>{2, 3, 4, 5});
    }

    SECTION("program_knowledge_base getPrevious test") {
        sp.insertNext(1, 2);
        REQUIRE(qps.getPrevious(1).empty());
        REQUIRE(qps.getNext(1) == std::unordered_set<int>{2});
        REQUIRE(qps.getNext(2).empty());
        REQUIRE(qps.getNext(2).empty());
    }

    SECTION("program_knowledge_base getPrevious negative test") {
        REQUIRE(qps.getPrevious(1).empty());
    }

    SECTION("program_knowledge_base getPrevious invalid test") {
        sp.insertNext(1, 2);
        REQUIRE(qps.getPrevious(1).empty());
        REQUIRE(qps.getPrevious(3).empty());
    }

    SECTION("program_knowledge_base getPrevious multiple test") {
        sp.insertNext(1, 2);
        sp.insertNext(2, 3);
        sp.insertNext(3, 4);
        sp.insertNext(3, 5);
        sp.insertNext(4, 5);

        REQUIRE(qps.getPrevious(1).empty());
        REQUIRE(qps.getPrevious(2) == std::unordered_set<int>{1});
        REQUIRE(qps.getPrevious(3) == std::unordered_set<int>{2});
        REQUIRE(qps.getPrevious(4) == std::unordered_set<int>{3});
        REQUIRE(qps.getPrevious(5) == std::unordered_set<int>{4, 3});
    }
}