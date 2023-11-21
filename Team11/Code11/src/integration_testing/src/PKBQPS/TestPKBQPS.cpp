#include "catch.hpp"
#include "query_processing_system/QPS.h"
#include "program_knowledge_base/PKBFactory.h"

// Written by Jonas for unit tests, but I've copied it here as it uses real query inputs.
bool testQuery(const std::string& query, std::list<std::string> expected, const std::shared_ptr<PKBQPS>& pkbQPS) {
    std::list<std::string> results;
    QPS{}.evaluateQuery(query, results, pkbQPS);
    results.sort();
    expected.sort();
    return results == expected;
}

TEST_CASE("No clause QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    pkbSP->insertAssign(1, "a");
    pkbSP->insertVariable("a");

    REQUIRE(testQuery("stmt s; Select s", {"1"}, pkbQPS));
    REQUIRE(testQuery("assign a; Select a", {"1"}, pkbQPS));
    REQUIRE(testQuery("variable v; Select v", {"a"}, pkbQPS));
}

TEST_CASE("One UsesS QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    for (int i = 1; i < 5; i++) {
        pkbSP->insertAssign(i, std::string(i, 'a'));
        pkbSP->insertUses(i, std::string(i, 'b'));
        pkbSP->insertVariable(std::string(i, 'b'));
    }

    for (int i = 5; i < 9; i++) {
        pkbSP->insertPrint(i, std::string(i - 4, 'b'));
        pkbSP->insertUses(i, std::string(i - 4, 'b'));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("assign a; Select a such that Uses(a, \"a\")", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(a, \"b\")", {"1"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Uses(s, \"b\")", {"1", "5"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(a, _)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("variable v; Select v such that Uses(2, v)", {"bb"}, pkbQPS));
        REQUIRE(testQuery("variable v; Select v such that Uses(10, v)", {}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery("assign a1, a2; Select a1 such that Uses(a1, a2)", {"SemanticError"}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select a such that Uses(a, v)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("print p; variable v; Select p such that Uses(p, v)", {"5", "6", "7", "8"}, pkbQPS));
        REQUIRE(testQuery("stmt s; variable v; Select s such that Uses(s, v)", {"1", "2", "3", "4", "5", "6", "7", "8"}, pkbQPS));
    }
}

TEST_CASE("One Modifies QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    for (int i = 1; i < 5; i++) {
        pkbSP->insertAssign(i, std::string(i, 'a'));
        pkbSP->insertModifies(i, std::string(i, 'a'));
        pkbSP->insertVariable(std::string(i, 'a'));
    }

    for (int i = 5; i < 9; i++) {
        pkbSP->insertRead(i, std::string(i - 4, 'a'));
        pkbSP->insertModifies(i, std::string(i - 4, 'a'));
    }

    pkbSP->insertProcedure("main");
    pkbSP->insertModifies("main", "a");

    pkbSP->insertProcedure("sub1");
    pkbSP->insertModifies("sub1", "aa");
    pkbSP->insertModifies("sub1", "aaa");

    SECTION("One synonym") {
        REQUIRE(testQuery("assign a; Select a such that Modifies(a, \"b\")", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Modifies(a, \"a\")", {"1"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Modifies(s, \"a\")", {"1", "5"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Modifies(a, _)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("variable v; Select v such that Modifies(2, v)", {"aa"}, pkbQPS));
        REQUIRE(testQuery("variable v; Select v such that Modifies(10, v)", {}, pkbQPS));

        REQUIRE(testQuery(R"(procedure p; Select p such that Modifies(p, "a"))", {"main"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Modifies(p, "aa"))", {"sub1"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Modifies(p, "aaa"))", {"sub1"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Modifies(p, "aaaa"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Modifies(p, _))", {"main", "sub1"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery("assign a1, a2; Select a1 such that Modifies(a1, a2)", {"SemanticError"}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select a such that Modifies(a, v)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("read r; variable v; Select r such that Modifies(r, v)", {"5", "6", "7", "8"}, pkbQPS));
        REQUIRE(testQuery("stmt s; variable v; Select s such that Modifies(s, v)", {"1", "2", "3", "4", "5", "6", "7", "8"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; variable v; Select p such that Modifies(p, _))", {"main", "sub1"}, pkbQPS));
    }
}

TEST_CASE("One Follows QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    pkbSP->insertRead(5, std::string(5, 'a'));
    pkbSP->insertModifies(5, std::string(5, 'a'));

    SECTION("No follows") {
        REQUIRE(testQuery("read r; Select r such that Follows(_, _)", {}, pkbQPS));
    }

    for (int i = 1; i < 5; i++) {
        pkbSP->insertAssign(i, std::string(i, 'a'));
        pkbSP->insertFollows(i, i + 1);
    }

    SECTION("No synonym") {
        REQUIRE(testQuery("assign a; Select a such that Follows(1, 2)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows(1, 3)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows(1, 1)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows(2, 1)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows(_, 1)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows(5, _)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows(1, _)", {"1", "2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Follows(_, _)", {"5"}, pkbQPS));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("stmt s; variable v; Select s such that Follows(v, _)", {"SemanticError"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows(s, _)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows(1, s)", {"2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows(_, s)", {"2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows(s, 3)", {"2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows(4, s)", {"5"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Follows(r, _)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows(_, a)", {"2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Follows(3, r)", {}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Follows(4, r)", {"5"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery("assign a; Select a such that Follows(a, a)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s1, s2; Select s1 such that Follows(s1, s2)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("assign a1, a2; Select a1 such that Follows(a1, a2)", {"1", "2", "3"}, pkbQPS));
        REQUIRE(testQuery("assign a; read r; Select a such that Follows(a, r)", {"4"}, pkbQPS));
        REQUIRE(testQuery("assign a; read r; Select a such that Follows(r, a)", {}, pkbQPS));
    }
}

TEST_CASE("One Follows* QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    pkbSP->insertRead(5, std::string(5, 'a'));
    pkbSP->insertModifies(5, std::string(5, 'a'));

    SECTION("No follows") {
        testQuery("read r; Select r such that Follows*(_, _)", {}, pkbQPS);
    }

    for (int i = 1; i < 5; i++) {
        pkbSP->insertAssign(i, std::string(i, 'a'));
        pkbSP->insertFollows(i, i + 1);
    }

    pkbSP->insertFollowsStar(1,3);
    pkbSP->insertFollowsStar(1,4);
    pkbSP->insertFollowsStar(1,5);
    pkbSP->insertFollowsStar(2,4);
    pkbSP->insertFollowsStar(2,5);
    pkbSP->insertFollowsStar(3,5);

    SECTION("No synonym") {
        REQUIRE(testQuery("assign a; Select a such that Follows*(1, 2)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows*(1, 3)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows*(1, 1)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows*(2, 1)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows*(_, 1)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows*(5, _)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows*(1, _)", {"1", "2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Follows*(_, _)", {"5"}, pkbQPS));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("stmt s; variable v; Select s such that Follows*(v, _)", {"SemanticError"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows*(s, _)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows*(1, s)", {"2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows*(_, s)", {"2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows*(s, 3)", {"1", "2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Follows*(2, s)", {"3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Follows*(r, _)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Follows*(_, a)", {"2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Follows*(3, r)", {"5"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Follows*(4, r)", {"5"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery("assign a; Select a such that Follows*(a, a)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s1, s2; Select s1 such that Follows*(s1, s2)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("assign a1, a2; Select a1 such that Follows*(a1, a2)", {"1", "2", "3"}, pkbQPS));
        REQUIRE(testQuery("assign a; read r; Select a such that Follows*(a, r)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("assign a; read r; Select a such that Follows*(r, a)", {}, pkbQPS));
    }
}

TEST_CASE("One Parent QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    /*
     * 1. while (a == 1) {
     * 2.     if (a == 1) {
     * 3.        read aaa;
     *        } else {
     * 4.        read aaa;
     *       }
     * 5.    read aaaaa;
     *    }
     */

    SECTION("No parents") {
        REQUIRE(testQuery("read r; Select r such that Parent(_, _)", {}, pkbQPS));
    }

    pkbSP->insertWhile(1, {"a", "1"});
    pkbSP->insertIf(2, {"a", "1"});
    pkbSP->insertParents(1, 2);
    pkbSP->insertRead(3, "aaa");
    pkbSP->insertModifies(3, "aaa");
    pkbSP->insertParents(2, 3);
    pkbSP->insertRead(4, "aaa");
    pkbSP->insertModifies(4, "aaa");
    pkbSP->insertParents(2, 4);
    pkbSP->insertParentsStar(1, 4);
    pkbSP->insertModifies(4, "aaa");
    pkbSP->insertParents(2, 4);
    pkbSP->insertParentsStar(1, 4);
    pkbSP->insertParentsStar(1, 3);
    pkbSP->insertRead(5, "aaaaa");
    pkbSP->insertModifies(5, "aaaaa");
    pkbSP->insertParents(1, 5);

    SECTION("No synonym") {
        REQUIRE(testQuery("stmt s; Select s such that Parent(1, 2)", {"1", "2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent(1, 3)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent(1, 1)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent(2, 1)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent(_, 6)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent(4, _)", {}, pkbQPS));
        REQUIRE(testQuery("while w; Select w such that Parent(1, _)", {"1"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Parent(_, _)", {"3", "4", "5"}, pkbQPS));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("stmt s; Select s such that Parent(s, _)", {"1", "2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent(_, s)", {"2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent(s, 3)", {"2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent(4, s)", {}, pkbQPS));
        REQUIRE(testQuery("if i; Select i such that Parent(_, i)", {"2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent(1, s)", {"2", "5"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Parent(_, r)", {"3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Parent(r, _)", {}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery("stmt s; Select s such that Parent(s, s)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s1, s2; Select s1 such that Parent(s1, s2)", {"1", "2"}, pkbQPS));
        REQUIRE(testQuery("stmt s1, s2; Select s2 such that Parent(s1, s2)", {"2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("stmt s; if i; Select s such that Parent(i, s)", {"3", "4"}, pkbQPS));
        REQUIRE(testQuery("stmt s; read r; Select r such that Parent(r, s)", {}, pkbQPS));
    }
}

TEST_CASE("One Parent* QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    /*
     * 1. while (a == 1) {
     * 2.     if (a == 1) {
     * 3.        read aaa;
     *        } else {
     * 4.        read aaa;
     *       }
     *    }
     */
    SECTION("No parents") {
        REQUIRE(testQuery("read r; Select r such that Parent*(_, _)", {}, pkbQPS));
    }

    pkbSP->insertWhile(1, {"a", "1"});
    pkbSP->insertIf(2, {"a", "1"});
    pkbSP->insertParents(1, 2);
    pkbSP->insertRead(3, "aaa");
    pkbSP->insertModifies(3, "aaa");
    pkbSP->insertParents(2, 3);
    pkbSP->insertRead(4, "aaa");
    pkbSP->insertModifies(4, "aaa");
    pkbSP->insertParents(2, 4);
    pkbSP->insertParentsStar(1, 4);
    pkbSP->insertParentsStar(1, 3);

    SECTION("No synonym") {
        REQUIRE(testQuery("stmt s; Select s such that Parent*(1, 2)", {"1", "2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("if i; Select i such that Parent*(1, 3)", {"2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent*(1, 1)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent*(2, 1)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent*(_, 5)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent*(4, _)", {}, pkbQPS));
        REQUIRE(testQuery("while w; Select w such that Parent*(1, _)", {"1"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Parent*(_, _)", {"3", "4"}, pkbQPS));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("stmt s; Select s such that Parent*(s, _)", {"1", "2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent*(_, s)", {"2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent*(s, 3)", {"1", "2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent*(4, s)", {}, pkbQPS));
        REQUIRE(testQuery("if i; Select i such that Parent*(_, i)", {"2"}, pkbQPS));
        REQUIRE(testQuery("stmt s; Select s such that Parent*(1, s)", {"2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Parent*(_, r)", {"3", "4"}, pkbQPS));
        REQUIRE(testQuery("read r; Select r such that Parent*(r, _)", {}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery("stmt s; Select s such that Parent*(s, s)", {}, pkbQPS));
        REQUIRE(testQuery("stmt s1, s2; Select s1 such that Parent*(s1, s2)", {"1", "2"}, pkbQPS));
        REQUIRE(testQuery("stmt s1, s2; Select s2 such that Parent*(s1, s2)", {"2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("stmt s; while w; Select s such that Parent*(w, s)", {"2", "3", "4"}, pkbQPS));
        REQUIRE(testQuery("stmt s; read r; Select r such that Parent*(r, s)", {}, pkbQPS));
    }
}

TEST_CASE("One Calls QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    /*
     * procedure main {
     * 1. while (x == 1) {
     * 2.    read z;
     * }
     * 3.    call sub1;
     * 4.    call sub2;
     * }
     *
     * procedure sub1 {
     * 5.    call sub2;
     * 6.    call sub3;
     * }
     *
     * procedure sub2 {
     * 7.    read a;
     * }
     *
     * procedure sub3 {
     * 8.    call sub4;
     * }
     *
     * procedure sub4 {
     * 9.    read b;
     * }
     *
     * procedure notCalled {
     * 10.   read c;
     * }
     *
     */
    SECTION("No calls") {
        REQUIRE(testQuery("read r; Select r such that Calls(_, _)", {}, pkbQPS));
    }

    pkbSP->insertProcedure("main");
    pkbSP->insertProcedure("sub1");
    pkbSP->insertProcedure("sub2");
    pkbSP->insertProcedure("sub3");
    pkbSP->insertProcedure("sub4");
    pkbSP->insertProcedure("notCalled");
    pkbSP->insertWhile(1, std::vector<std::string>{"x", "1"});

    pkbSP->insertRead(2, "z");
    pkbSP->insertRead(5, "a");
    pkbSP->insertRead(7, "a");
    pkbSP->insertRead(9, "b");
    pkbSP->insertRead(10, "c");

    // Adding these because SynonymVisitor::visit(const std::shared_ptr<ReadSynonym>& readSynonym) segfaults
    // if reads don't have an accompanying Modifies
    pkbSP->insertModifies(2, "z");
    pkbSP->insertModifies(5, "a");
    pkbSP->insertModifies(7, "a");
    pkbSP->insertModifies(9, "b");
    pkbSP->insertModifies(10, "c");

    pkbSP->insertCall(1, "sub1");
    pkbSP->insertCall(2, "sub2");
    pkbSP->insertCall(3, "sub2");
    pkbSP->insertCall(4, "sub3");
    pkbSP->insertCall(6, "sub4");
    pkbSP->insertCall(8, "sub4");

    pkbSP->insertCalls("main", "sub1");
    pkbSP->insertCalls("main", "sub2");
    pkbSP->insertCalls("sub1", "sub2");
    pkbSP->insertCalls("sub1", "sub3");
    pkbSP->insertCalls("sub3", "sub4");

    pkbSP->insertCallsStar("main", "sub2");
    pkbSP->insertCallsStar("main", "sub3");
    pkbSP->insertCallsStar("main", "sub4");
    pkbSP->insertCallsStar("sub1", "sub4");

    SECTION("No synonym") {
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls("main", "sub1"))", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls("main", "sub3"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls("main", "main"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls("sub1", "main"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls(_, "notCalled"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls("sub4", _))", {}, pkbQPS));
        REQUIRE(testQuery(R"(while w; Select w such that Calls("main", _))", {"1"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; Select r such that Calls(_, _))", {"2", "5", "7", "9", "10"}, pkbQPS));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("procedure p; Select p such that Calls(p, _)", {"main", "sub1", "sub3"}, pkbQPS));
        REQUIRE(testQuery("procedure p; Select p such that Calls(_, p)", {"sub1", "sub2", "sub3", "sub4"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls(p, "sub2"))", {"main", "sub1"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls("main", p))", {"sub1", "sub2"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls(_, p))", {"sub1", "sub2", "sub3", "sub4"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls(p, _))", {"main", "sub1", "sub3"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls(p, p))", {}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p1, p2; Select p1 such that Calls(p1, p2))", {"main", "sub1", "sub3"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p1, p2; Select p2 such that Calls(p1, p2))", {"sub1", "sub2", "sub3", "sub4"}, pkbQPS));
    }
}

TEST_CASE("One x QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    /*
     * procedure main {
     * 1. while (x == 1) {
     * 2.    read z;
     * }
     * 3.    call sub1;
     * 4.    call sub2;
     * }
     *
     * procedure sub1 {
     * 5.    call sub2;
     * 6.    call sub3;
     * }
     *
     * procedure sub2 {
     * 7.    read a;
     * }
     *
     * procedure sub3 {
     * 8.    call sub4;
     * }
     *
     * procedure sub4 {
     * 9.    read b;
     * }
     *
     * procedure notCalled {
     * 10.   read c;
     * }
     *
     */
    SECTION("No calls") {
        testQuery("read r; Select r such that Calls(_, _)", {}, pkbQPS);
    }

    pkbSP->insertProcedure("main");
    pkbSP->insertProcedure("sub1");
    pkbSP->insertProcedure("sub2");
    pkbSP->insertProcedure("sub3");
    pkbSP->insertProcedure("sub4");
    pkbSP->insertProcedure("notCalled");
    pkbSP->insertWhile(1, std::vector<std::string>{"x", "1"});

    pkbSP->insertRead(2, "z");
    pkbSP->insertRead(5, "a");
    pkbSP->insertRead(7, "a");
    pkbSP->insertRead(9, "b");
    pkbSP->insertRead(10, "c");

    // Adding these because SynonymVisitor::visit(const std::shared_ptr<ReadSynonym>& readSynonym) segfaults
    // if reads don't have an accompanying Modifies
    pkbSP->insertModifies(2, "z");
    pkbSP->insertModifies(5, "a");
    pkbSP->insertModifies(7, "a");
    pkbSP->insertModifies(9, "b");
    pkbSP->insertModifies(10, "c");

    pkbSP->insertCall(1, "sub1");
    pkbSP->insertCall(2, "sub2");
    pkbSP->insertCall(3, "sub2");
    pkbSP->insertCall(4, "sub3");
    pkbSP->insertCall(6, "sub4");
    pkbSP->insertCall(8, "sub4");

    pkbSP->insertCalls("main", "sub1");
    pkbSP->insertCalls("main", "sub2");
    pkbSP->insertCalls("sub1", "sub2");
    pkbSP->insertCalls("sub1", "sub3");
    pkbSP->insertCalls("sub3", "sub4");

    pkbSP->insertCallsStar("main", "sub2");
    pkbSP->insertCallsStar("main", "sub3");
    pkbSP->insertCallsStar("main", "sub4");
    pkbSP->insertCallsStar("sub1", "sub4");

    SECTION("No synonym") {
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("main", "sub1"))", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("main", "sub3"))", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("main", "main"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("sub1", "main"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*(_, "notCalled"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("sub4", _))", {}, pkbQPS));
        REQUIRE(testQuery(R"(while w; Select w such that Calls*("main", _))", {"1"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; Select r such that Calls*(_, _))", {"2", "5", "7", "9", "10"}, pkbQPS));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("procedure p; Select p such that Calls*(p, _)", {"main", "sub1", "sub3"}, pkbQPS));
        REQUIRE(testQuery("procedure p; Select p such that Calls*(_, p)", {"sub1", "sub2", "sub3", "sub4"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls*(p, "sub2"))", {"main", "sub1"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls*(p, "sub4"))", {"main", "sub1", "sub3"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls*("main", p))", {"sub1", "sub2", "sub3", "sub4"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls*(p, p))", {}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p1, p2; Select p1 such that Calls*(p1, p2))", {"main", "sub1", "sub3"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p1, p2; Select p2 such that Calls*(p1, p2))", {"sub1", "sub2", "sub3", "sub4"}, pkbQPS));
    }
}

TEST_CASE("One Calls* QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    /*
     * procedure main {
     * 1. while (x == 1) {
     * 2.    read z;
     * }
     * 3.    call sub1;
     * 4.    call sub2;
     * }
     *
     * procedure sub1 {
     * 5.    call sub2;
     * 6.    call sub3;
     * }
     *
     * procedure sub2 {
     * 7.    read a;
     * }
     *
     * procedure sub3 {
     * 8.    call sub4;
     * }
     *
     * procedure sub4 {
     * 9.    read b;
     * }
     *
     * procedure notCalled {
     * 10.   read c;
     * }
     *
     */
    SECTION("No calls") {
        testQuery("read r; Select r such that Calls(_, _)", {}, pkbQPS);
    }

    pkbSP->insertProcedure("main");
    pkbSP->insertProcedure("sub1");
    pkbSP->insertProcedure("sub2");
    pkbSP->insertProcedure("sub3");
    pkbSP->insertProcedure("sub4");
    pkbSP->insertProcedure("notCalled");
    pkbSP->insertWhile(1, std::vector<std::string>{"x", "1"});

    pkbSP->insertRead(2, "z");
    pkbSP->insertRead(5, "a");
    pkbSP->insertRead(7, "a");
    pkbSP->insertRead(9, "b");
    pkbSP->insertRead(10, "c");

    // Adding these because SynonymVisitor::visit(const std::shared_ptr<ReadSynonym>& readSynonym) segfaults
    // if reads don't have an accompanying Modifies
    pkbSP->insertModifies(2, "z");
    pkbSP->insertModifies(5, "a");
    pkbSP->insertModifies(7, "a");
    pkbSP->insertModifies(9, "b");
    pkbSP->insertModifies(10, "c");

    pkbSP->insertCall(1, "sub1");
    pkbSP->insertCall(2, "sub2");
    pkbSP->insertCall(3, "sub2");
    pkbSP->insertCall(4, "sub3");
    pkbSP->insertCall(6, "sub4");
    pkbSP->insertCall(8, "sub4");

    pkbSP->insertCalls("main", "sub1");
    pkbSP->insertCalls("main", "sub2");
    pkbSP->insertCalls("sub1", "sub2");
    pkbSP->insertCalls("sub1", "sub3");
    pkbSP->insertCalls("sub3", "sub4");

    pkbSP->insertCallsStar("main", "sub2");
    pkbSP->insertCallsStar("main", "sub3");
    pkbSP->insertCallsStar("main", "sub4");
    pkbSP->insertCallsStar("sub1", "sub4");

    SECTION("No synonym") {
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("main", "sub1"))", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("main", "sub3"))", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("main", "main"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("sub1", "main"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*(_, "notCalled"))", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Calls*("sub4", _))", {}, pkbQPS));
        REQUIRE(testQuery(R"(while w; Select w such that Calls*("main", _))", {"1"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; Select r such that Calls*(_, _))", {"2", "5", "7", "9", "10"}, pkbQPS));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("procedure p; Select p such that Calls*(p, _)", {"main", "sub1", "sub3"}, pkbQPS));
        REQUIRE(testQuery("procedure p; Select p such that Calls*(_, p)", {"sub1", "sub2", "sub3", "sub4"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls*(p, "sub2"))", {"main", "sub1"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls*(p, "sub4"))", {"main", "sub1", "sub3"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls*("main", p))", {"sub1", "sub2", "sub3", "sub4"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery(R"(procedure p; Select p such that Calls*(p, p))", {}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p1, p2; Select p1 such that Calls*(p1, p2))", {"main", "sub1", "sub3"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p1, p2; Select p2 such that Calls*(p1, p2))", {"sub1", "sub2", "sub3", "sub4"}, pkbQPS));
    }
}

TEST_CASE("One If QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    /*
     * procedure main {
     * 1. if (x == y) then {
     * 2.         y = 1;
     *     } else {
     * 3.         x = 1;
     *     }
     *  }
     */

    pkbSP->insertProcedure("main");
    pkbSP->insertIf(1, {"x", "y"});
    pkbSP->insertModifies(1, "x");
    pkbSP->insertModifies(1, "y");
    pkbSP->insertModifies(2, "y");
    pkbSP->insertModifies(3, "x");
    pkbSP->insertUses(1, "x");
    pkbSP->insertUses(1, "y");
    pkbSP->insertAssign(1, "y");
    pkbSP->insertAssign(3, "x");
    pkbSP->insertConstant("1");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertVariable("x");
    pkbSP->insertVariable("y");

    SECTION("One synonym") {
        REQUIRE(testQuery(R"(if ifs; Select ifs pattern ifs(_,_,_))", {"1"}, pkbQPS));
        REQUIRE(testQuery(R"(if ifs; Select ifs pattern ifs("x",_,_))", {"1"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery(R"(if ifs; variable v; Select <ifs, v> pattern ifs(v,_,_))", {"1 x", "1 y"}, pkbQPS));
    }
}

TEST_CASE("One Pattern QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    std::string operators[5] = {"+", "*", "-", "/", "%"};
    for (int i = 1; i < 6; i++) {
        pkbSP->insertAssign(i, std::string(i, 'a'));
        pkbSP->insertModifies(i, std::string(i, 'a'));
        std::string expression = "1" + operators[i - 1] + "2" + operators[i % 5] + "3" + operators[(i + 1) % 5] + "4";
        pkbSP->insertPattern(i, ExpressionParser::convertToPostfix(expression));
        pkbSP->insertVariable(std::string(i, 'a'));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("assign a; Select a pattern a(_, _)", {"1", "2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a pattern a(\"a\", _)", {"1"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a pattern a(_, \"1 + 2\")", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a pattern a(_, _\"1 + 2\"_)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a pattern a(_, _\"1 * 2\"_)", {"2"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a pattern a(\"a\", _\"1 * 2\"_)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a pattern a(\"aa\", _\"1 * 2\"_)", {"2"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery("assign a; variable v; Select a pattern a(v, _)", {"1", "2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select v pattern a(v, _)", {"a", "aa", "aaa", "aaaa", "aaaaa"}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select a pattern a(v, \"1 * 2\")", {}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select v pattern a(v, \"1 * 2\")", {}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select a pattern a(v, _\"1 * 2\"_)", {"2"}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select v pattern a(v, _\"1 * 2\"_)", {"aa"}, pkbQPS));
    }
}

TEST_CASE("One While QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    /*
     * procedure main {
     * 1. while (x == y) {
     * 2.         y = 1;
     *     }
     *  }
     */

    pkbSP->insertProcedure("main");
    pkbSP->insertWhile(1, {"x", "y"});
    pkbSP->insertModifies(1, "y");
    pkbSP->insertModifies(2, "y");
    pkbSP->insertUses(1, "x");
    pkbSP->insertUses(1, "y");
    pkbSP->insertAssign(1, "y");
    pkbSP->insertConstant("1");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertVariable("x");
    pkbSP->insertVariable("y");

    SECTION("One synonym") {
        REQUIRE(testQuery(R"(while whiles; Select whiles pattern whiles(_,_))", {"1"}, pkbQPS));
        REQUIRE(testQuery(R"(while whiles; Select whiles pattern whiles("x",_))", {"1"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery(R"(while whiles; variable v; Select <whiles, v> pattern whiles(v,_))", {"1 x", "1 y"}, pkbQPS));
    }
}

TEST_CASE("One Next QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    /**
     * procedure main {
     * 1.    x = 0;
     * 2.    while (x > 1) {
     * 3.        if (x > 2) then {
     * 4.            read y;
     * 5.            call one;
     *           } else {
     * 6.            read z;
     * 7.            call two;
     *           }
     *       }
     *    }
     *
     *  procedure one {
     *  8.    read a;
     *  9.    call two;
     *  }
     *
     *  procedure two {
     *  10.    read b;
     *  }
     *
     */

    pkbSP->insertProcedure("main");
    pkbSP->insertAssign(1, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(1,{"0", });
    pkbSP->insertConstant("0");
    pkbSP->insertWhile(2, {"x"});
    pkbSP->insertConstant("1");
    pkbSP->insertVariable("x");
    pkbSP->insertIf(3, {"x"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(4, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertCall(5, "one");
    pkbSP->insertRead(6, "z");
    pkbSP->insertVariable("z");
    pkbSP->insertCall(7, "two");
    pkbSP->insertProcedure("one");
    pkbSP->insertRead(8, "a");
    pkbSP->insertVariable("a");
    pkbSP->insertCall(9, "two");
    pkbSP->insertProcedure("two");
    pkbSP->insertRead(10, "b");
    pkbSP->insertVariable("b");
    pkbSP->insertModifies(1, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(1, 2);
    pkbSP->insertFollowsStar(1, 2);
    pkbSP->insertNext(1, 2);
    pkbSP->insertUses(2, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertParents(2, 3);
    pkbSP->insertParentsStar(2, 3);
    pkbSP->insertNext(2, 3);
    pkbSP->insertUses(3, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(2, "x");
    pkbSP->insertParents(3, 4);
    pkbSP->insertParentsStar(2, 4);
    pkbSP->insertParentsStar(3, 4);
    pkbSP->insertNext(3, 4);
    pkbSP->insertModifies(4, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(2, "y");
    pkbSP->insertModifies(3, "y");
    pkbSP->insertFollows(4, 5);
    pkbSP->insertFollowsStar(4, 5);
    pkbSP->insertParents(3, 5);
    pkbSP->insertParentsStar(2, 5);
    pkbSP->insertParentsStar(3, 5);
    pkbSP->insertNext(4, 5);
    pkbSP->insertCalls("main", "one");
    pkbSP->insertParents(3, 6);
    pkbSP->insertParentsStar(2, 6);
    pkbSP->insertParentsStar(3, 6);
    pkbSP->insertNext(3, 6);
    pkbSP->insertModifies(6, "z");
    pkbSP->insertModifies("main", "z");
    pkbSP->insertModifies(2, "z");
    pkbSP->insertModifies(3, "z");
    pkbSP->insertFollows(6, 7);
    pkbSP->insertFollowsStar(6, 7);
    pkbSP->insertParents(3, 7);
    pkbSP->insertParentsStar(2, 7);
    pkbSP->insertParentsStar(3, 7);
    pkbSP->insertNext(6, 7);
    pkbSP->insertCalls("main", "two");
    pkbSP->insertNext(7, 2);
    pkbSP->insertNext(5, 2);
    pkbSP->insertNext(7, 2);
    pkbSP->insertModifies(8, "a");
    pkbSP->insertModifies("one", "a");
    pkbSP->insertFollows(8, 9);
    pkbSP->insertFollowsStar(8, 9);
    pkbSP->insertNext(8, 9);
    pkbSP->insertCalls("one", "two");
    pkbSP->insertModifies(10, "b");
    pkbSP->insertModifies("two", "b");
    pkbSP->insertModifies(9, "b");
    pkbSP->insertModifies(3, "b");
    pkbSP->insertModifies(2, "b");
    pkbSP->insertModifies(7, "b");
    pkbSP->insertCallsStar("one", "two");
    pkbSP->insertModifies("one", "b");
    pkbSP->insertCallsStar("main", "two");
    pkbSP->insertModifies("main", "b");
    pkbSP->insertModifies(3, "b");
    pkbSP->insertModifies(2, "b");
    pkbSP->insertModifies(5, "b");
    pkbSP->insertModifies(3, "a");
    pkbSP->insertModifies(2, "a");
    pkbSP->insertModifies(5, "a");
    pkbSP->insertCallsStar("main", "one");
    pkbSP->insertCallsStar("main", "two");
    pkbSP->insertModifies("main", "b");
    pkbSP->insertModifies("main", "a");

    SECTION("No synonym") {
        REQUIRE(testQuery("Select BOOLEAN such that Next (1, 2)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next(1, 2)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next* (1, 2)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next*(1, 2)", {"TRUE"}, pkbQPS));

        REQUIRE(testQuery("Select BOOLEAN such that Next (2, 3)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next*(2, 3)", {"TRUE"}, pkbQPS));

        REQUIRE(testQuery("Select BOOLEAN such that Next (3, 4)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next*(3, 4)", {"TRUE"}, pkbQPS));

        REQUIRE(testQuery("Select BOOLEAN such that Next (4, 5)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next*(4, 5)", {"TRUE"}, pkbQPS));

        REQUIRE(testQuery("Select BOOLEAN such that Next (5, 6)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next*(5, 6)", {"TRUE"}, pkbQPS));

        REQUIRE(testQuery("Select BOOLEAN such that Next (6, 7)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next*(6, 7)", {"TRUE"}, pkbQPS));

        REQUIRE(testQuery("Select BOOLEAN such that Next (7, 8)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next*(7, 8)", {"FALSE"}, pkbQPS));

        REQUIRE(testQuery("Select BOOLEAN such that Next (_, 8)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next*(_, 8)", {"FALSE"}, pkbQPS));

        REQUIRE(testQuery("Select BOOLEAN such that Next (_, _)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Next*(_, _)", {"TRUE"}, pkbQPS));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery(R"(stmt s; Select s such that Next (3, s))", {"4", "6"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Next*(3, s))", {"2", "3", "4", "5", "6", "7"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Next (1, s))", {"2"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s such that Next*(1, s))", {"2", "3", "4", "5", "6", "7"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery(R"(stmt s1, s2; Select <s1, s2> such that Next(s1, s2))", {"1 2", "2 3", "3 4", "3 6", "4 5", "5 2", "6 7", "7 2", "8 9"}, pkbQPS));
    }
}

TEST_CASE("One Such That One Pattern QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    std::string operators[5] = {"+", "*", "-", "/", "%"};
    for (int i = 1; i < 6; i++) {
        pkbSP->insertAssign(i, std::string(i, 'a'));
        pkbSP->insertModifies(i, std::string(i, 'a'));
        std::string expression = "1" + operators[i - 1] + "2" + operators[i % 5] + "3" + operators[(i + 1) % 5] + std::string(6 - i, 'a');
        pkbSP->insertPattern(i, ExpressionParser::convertToPostfix(expression));
        pkbSP->insertVariable(std::string(i, 'a'));
        pkbSP->insertVariable(std::string(i, 'b'));
        pkbSP->insertUses(i, std::string(6 - i, 'a'));
    }

    SECTION("One synonym") {
        REQUIRE(testQuery("assign a; Select a such that Uses(a, _) pattern a(_, _)", {"1", "2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(a, _) pattern a(_, _\"1\"_)", {"1", "2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(a, _) pattern a(\"a\", _)", {"1"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(a, \"aa\") pattern a(_, _)", {"4"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(a, _) pattern a(_, _\"1 + 2\"_)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(a, _) pattern a(_, _\"1 * 2\"_)", {"2"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(a, \"aaaa\") pattern a(_, _\"1 * 2\"_)", {"2"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(a, \"aaa\") pattern a(_, _\"1 * 2\"_)", {}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(1, _) pattern a(_, _\"2 / 3\"_)", {"3"}, pkbQPS));
        REQUIRE(testQuery("assign a; Select a such that Uses(1, _) pattern a(\"aaa\", _\"aaa\"_)", {"3"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery("assign a; variable v; Select a such that Uses(a, v) pattern a(v, _)", {"3"}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select v such that Uses(a, v) pattern a(v, _)", {"aaa"}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select a such that Uses(a, v) pattern a(v, _\"1 * 2\"_)", {}, pkbQPS));
        REQUIRE(testQuery("assign a1, a2; Select a1 such that Uses(a1, _) pattern a2(_, _\"1 * 2\"_)",
                  {"1", "2", "3", "4", "5"}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v; Select v such that Uses(a, _) pattern a(v, _)",
                  {"a", "aa", "aaa", "aaaa", "aaaaa"}, pkbQPS));
    }

    SECTION("Three synonym") {
        REQUIRE(testQuery("assign a; variable v1, v2; Select v1 such that Uses(a, v1) pattern a(v2, _)",
                  {"a", "aa", "aaa", "aaaa", "aaaaa"}, pkbQPS));
        REQUIRE(testQuery("assign a; variable v1, v2; Select v1 such that Uses(a, v1) pattern a(v2, _\"1 * 2\"_)",
                  {"aaaa"}, pkbQPS));
        REQUIRE(testQuery("assign a1, a2; variable v; Select a1 such that Uses(a1, v) pattern a2(v, _\"1 * 2\"_)",
                  {"4"}, pkbQPS));
    }
}

TEST_CASE("QPS Errors") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    SECTION("Syntax") {
        REQUIRE(testQuery("stmt s Select s", {"SyntaxError"}, pkbQPS));
        REQUIRE(testQuery("stmt s; select s", {"SyntaxError"}, pkbQPS));
        REQUIRE(testQuery("Stmt s; Select s", {"SyntaxError"}, pkbQPS));
    }

    SECTION("Semantic") {
        REQUIRE(testQuery("Select s", {"SemanticError"}, pkbQPS));

        REQUIRE(testQuery("stmt s, s; Select s", {"SemanticError"}, pkbQPS));
        REQUIRE(testQuery("stmt s; stmt s; Select s", {"SemanticError"}, pkbQPS));

        REQUIRE(testQuery("stmt s; variable v; Select s such that Follows(s, v)", {"SemanticError"}, pkbQPS));
        REQUIRE(testQuery("stmt s; variable v; Select s such that Follows*(v, s)", {"SemanticError"}, pkbQPS));
        REQUIRE(testQuery("stmt s; variable v; Select s such that Parent(v, s)", {"SemanticError"}, pkbQPS));
        REQUIRE(testQuery("stmt s; variable v; Select s such that Parent*(s, v)", {"SemanticError"}, pkbQPS));

        REQUIRE(testQuery("stmt s; Select s such that Uses(s, s)", {"SemanticError"}, pkbQPS));
        REQUIRE(testQuery("variable v; Select v such that Uses(v, v)", {"SemanticError"}, pkbQPS));
    }

    SECTION("Edge cases") {
        REQUIRE(testQuery("stmt stmt; Select stmt", {}, pkbQPS));
        REQUIRE(testQuery("print Parent; Select Parent such that Parent(Parent, Parent)", {}, pkbQPS));
    }
}

TEST_CASE("Affects QPS") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;


    pkbSP->insertProcedure("main");
    pkbSP->insertAssign(1, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(1, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertAssign(2, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(2, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(3, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(3, {"3"});
    pkbSP->insertConstant("3");
    pkbSP->insertAssign(4, "z");
    pkbSP->insertVariable("z");
    pkbSP->insertPattern(4, {"x", "4", "+"});
    pkbSP->insertConstant("4");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(5, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(6, "a");
    pkbSP->insertVariable("a");
    pkbSP->insertPattern(6, {"x", "5", "+"});
    pkbSP->insertConstant("5");
    pkbSP->insertVariable("x");
    pkbSP->insertCall(7, "modifyX");
    pkbSP->insertAssign(8, "b");
    pkbSP->insertVariable("b");
    pkbSP->insertPattern(8, {"x", "6", "+"});
    pkbSP->insertConstant("6");
    pkbSP->insertVariable("x");
    pkbSP->insertCall(9, "doNotModifyX");
    pkbSP->insertAssign(10, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertPattern(10, {"x", "7", "+"});
    pkbSP->insertConstant("7");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(11, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(11, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertIf(12, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(13, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(13, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(14, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertRead(15, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertIf(16, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(17, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(17, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(18, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertCall(19, "modifyX");
    pkbSP->insertIf(20, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(21, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(21, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(22, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertCall(23, "doNotModifyX");
    pkbSP->insertIf(24, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(25, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(25, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(26, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertAssign(27, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(27, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertIf(28, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(29, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(29, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(30, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertAssign(31, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(31, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertWhile(32, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(33, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(33, {"2"});
    pkbSP->insertConstant("2");
    pkbSP->insertAssign(34, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(34, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(35, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(35, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertWhile(36, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(37, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(37, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(38, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(38, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(39, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(39, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertIf(40, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(41, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(41, {"2"});
    pkbSP->insertConstant("2");
    pkbSP->insertRead(42, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertAssign(43, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(43, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(44, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(44, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertIf(45, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(46, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(46, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(47, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertAssign(48, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(48, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertProcedure("modifyX");
    pkbSP->insertAssign(49, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(49, {"2"});
    pkbSP->insertConstant("2");
    pkbSP->insertProcedure("doNotModifyX");
    pkbSP->insertAssign(50, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(50, {"x"});
    pkbSP->insertVariable("x");
    pkbSP->insertModifies(1, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(1, 2);
    pkbSP->insertFollowsStar(1, 2);
    pkbSP->insertNext(1, 2);
    pkbSP->insertModifies(2, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(2, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(2, 3);
    pkbSP->insertFollowsStar(1, 3);
    pkbSP->insertFollowsStar(2, 3);
    pkbSP->insertNext(2, 3);
    pkbSP->insertModifies(3, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(3, 4);
    pkbSP->insertFollowsStar(1, 4);
    pkbSP->insertFollowsStar(2, 4);
    pkbSP->insertFollowsStar(3, 4);
    pkbSP->insertNext(3, 4);
    pkbSP->insertModifies(4, "z");
    pkbSP->insertModifies("main", "z");
    pkbSP->insertUses(4, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(4, 5);
    pkbSP->insertFollowsStar(1, 5);
    pkbSP->insertFollowsStar(2, 5);
    pkbSP->insertFollowsStar(3, 5);
    pkbSP->insertFollowsStar(4, 5);
    pkbSP->insertNext(4, 5);
    pkbSP->insertModifies(5, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(5, 6);
    pkbSP->insertFollowsStar(1, 6);
    pkbSP->insertFollowsStar(2, 6);
    pkbSP->insertFollowsStar(3, 6);
    pkbSP->insertFollowsStar(4, 6);
    pkbSP->insertFollowsStar(5, 6);
    pkbSP->insertNext(5, 6);
    pkbSP->insertModifies(6, "a");
    pkbSP->insertModifies("main", "a");
    pkbSP->insertUses(6, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(6, 7);
    pkbSP->insertFollowsStar(1, 7);
    pkbSP->insertFollowsStar(2, 7);
    pkbSP->insertFollowsStar(3, 7);
    pkbSP->insertFollowsStar(4, 7);
    pkbSP->insertFollowsStar(5, 7);
    pkbSP->insertFollowsStar(6, 7);
    pkbSP->insertNext(6, 7);
    pkbSP->insertCalls("main", "modifyX");
    pkbSP->insertFollows(7, 8);
    pkbSP->insertFollowsStar(1, 8);
    pkbSP->insertFollowsStar(2, 8);
    pkbSP->insertFollowsStar(3, 8);
    pkbSP->insertFollowsStar(4, 8);
    pkbSP->insertFollowsStar(5, 8);
    pkbSP->insertFollowsStar(6, 8);
    pkbSP->insertFollowsStar(7, 8);
    pkbSP->insertNext(7, 8);
    pkbSP->insertModifies(8, "b");
    pkbSP->insertModifies("main", "b");
    pkbSP->insertUses(8, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(8, 9);
    pkbSP->insertFollowsStar(1, 9);
    pkbSP->insertFollowsStar(2, 9);
    pkbSP->insertFollowsStar(3, 9);
    pkbSP->insertFollowsStar(4, 9);
    pkbSP->insertFollowsStar(5, 9);
    pkbSP->insertFollowsStar(6, 9);
    pkbSP->insertFollowsStar(7, 9);
    pkbSP->insertFollowsStar(8, 9);
    pkbSP->insertNext(8, 9);
    pkbSP->insertCalls("main", "doNotModifyX");
    pkbSP->insertFollows(9, 10);
    pkbSP->insertFollowsStar(1, 10);
    pkbSP->insertFollowsStar(2, 10);
    pkbSP->insertFollowsStar(3, 10);
    pkbSP->insertFollowsStar(4, 10);
    pkbSP->insertFollowsStar(5, 10);
    pkbSP->insertFollowsStar(6, 10);
    pkbSP->insertFollowsStar(7, 10);
    pkbSP->insertFollowsStar(8, 10);
    pkbSP->insertFollowsStar(9, 10);
    pkbSP->insertNext(9, 10);
    pkbSP->insertModifies(10, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertUses(10, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(10, 11);
    pkbSP->insertFollowsStar(1, 11);
    pkbSP->insertFollowsStar(2, 11);
    pkbSP->insertFollowsStar(3, 11);
    pkbSP->insertFollowsStar(4, 11);
    pkbSP->insertFollowsStar(5, 11);
    pkbSP->insertFollowsStar(6, 11);
    pkbSP->insertFollowsStar(7, 11);
    pkbSP->insertFollowsStar(8, 11);
    pkbSP->insertFollowsStar(9, 11);
    pkbSP->insertFollowsStar(10, 11);
    pkbSP->insertNext(10, 11);
    pkbSP->insertModifies(11, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(11, 12);
    pkbSP->insertFollowsStar(1, 12);
    pkbSP->insertFollowsStar(2, 12);
    pkbSP->insertFollowsStar(3, 12);
    pkbSP->insertFollowsStar(4, 12);
    pkbSP->insertFollowsStar(5, 12);
    pkbSP->insertFollowsStar(6, 12);
    pkbSP->insertFollowsStar(7, 12);
    pkbSP->insertFollowsStar(8, 12);
    pkbSP->insertFollowsStar(9, 12);
    pkbSP->insertFollowsStar(10, 12);
    pkbSP->insertFollowsStar(11, 12);
    pkbSP->insertNext(11, 12);
    pkbSP->insertUses(12, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(12, 13);
    pkbSP->insertParentsStar(12, 13);
    pkbSP->insertNext(12, 13);
    pkbSP->insertModifies(13, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(12, "y");
    pkbSP->insertUses(13, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(12, "x");
    pkbSP->insertParents(12, 14);
    pkbSP->insertParentsStar(12, 14);
    pkbSP->insertNext(12, 14);
    pkbSP->insertModifies(14, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(12, "c");
    pkbSP->insertFollows(12, 15);
    pkbSP->insertFollowsStar(1, 15);
    pkbSP->insertFollowsStar(2, 15);
    pkbSP->insertFollowsStar(3, 15);
    pkbSP->insertFollowsStar(4, 15);
    pkbSP->insertFollowsStar(5, 15);
    pkbSP->insertFollowsStar(6, 15);
    pkbSP->insertFollowsStar(7, 15);
    pkbSP->insertFollowsStar(8, 15);
    pkbSP->insertFollowsStar(9, 15);
    pkbSP->insertFollowsStar(10, 15);
    pkbSP->insertFollowsStar(11, 15);
    pkbSP->insertFollowsStar(12, 15);
    pkbSP->insertNext(14, 15);
    pkbSP->insertNext(13, 15);
    pkbSP->insertNext(14, 15);
    pkbSP->insertModifies(15, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(15, 16);
    pkbSP->insertFollowsStar(1, 16);
    pkbSP->insertFollowsStar(2, 16);
    pkbSP->insertFollowsStar(3, 16);
    pkbSP->insertFollowsStar(4, 16);
    pkbSP->insertFollowsStar(5, 16);
    pkbSP->insertFollowsStar(6, 16);
    pkbSP->insertFollowsStar(7, 16);
    pkbSP->insertFollowsStar(8, 16);
    pkbSP->insertFollowsStar(9, 16);
    pkbSP->insertFollowsStar(10, 16);
    pkbSP->insertFollowsStar(11, 16);
    pkbSP->insertFollowsStar(12, 16);
    pkbSP->insertFollowsStar(15, 16);
    pkbSP->insertNext(15, 16);
    pkbSP->insertUses(16, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(16, 17);
    pkbSP->insertParentsStar(16, 17);
    pkbSP->insertNext(16, 17);
    pkbSP->insertModifies(17, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(16, "y");
    pkbSP->insertUses(17, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(16, "x");
    pkbSP->insertParents(16, 18);
    pkbSP->insertParentsStar(16, 18);
    pkbSP->insertNext(16, 18);
    pkbSP->insertModifies(18, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(16, "c");
    pkbSP->insertFollows(16, 19);
    pkbSP->insertFollowsStar(1, 19);
    pkbSP->insertFollowsStar(2, 19);
    pkbSP->insertFollowsStar(3, 19);
    pkbSP->insertFollowsStar(4, 19);
    pkbSP->insertFollowsStar(5, 19);
    pkbSP->insertFollowsStar(6, 19);
    pkbSP->insertFollowsStar(7, 19);
    pkbSP->insertFollowsStar(8, 19);
    pkbSP->insertFollowsStar(9, 19);
    pkbSP->insertFollowsStar(10, 19);
    pkbSP->insertFollowsStar(11, 19);
    pkbSP->insertFollowsStar(12, 19);
    pkbSP->insertFollowsStar(15, 19);
    pkbSP->insertFollowsStar(16, 19);
    pkbSP->insertNext(18, 19);
    pkbSP->insertNext(17, 19);
    pkbSP->insertNext(18, 19);
    pkbSP->insertCalls("main", "modifyX");
    pkbSP->insertFollows(19, 20);
    pkbSP->insertFollowsStar(1, 20);
    pkbSP->insertFollowsStar(2, 20);
    pkbSP->insertFollowsStar(3, 20);
    pkbSP->insertFollowsStar(4, 20);
    pkbSP->insertFollowsStar(5, 20);
    pkbSP->insertFollowsStar(6, 20);
    pkbSP->insertFollowsStar(7, 20);
    pkbSP->insertFollowsStar(8, 20);
    pkbSP->insertFollowsStar(9, 20);
    pkbSP->insertFollowsStar(10, 20);
    pkbSP->insertFollowsStar(11, 20);
    pkbSP->insertFollowsStar(12, 20);
    pkbSP->insertFollowsStar(15, 20);
    pkbSP->insertFollowsStar(16, 20);
    pkbSP->insertFollowsStar(19, 20);
    pkbSP->insertNext(19, 20);
    pkbSP->insertUses(20, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(20, 21);
    pkbSP->insertParentsStar(20, 21);
    pkbSP->insertNext(20, 21);
    pkbSP->insertModifies(21, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(20, "y");
    pkbSP->insertUses(21, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(20, "x");
    pkbSP->insertParents(20, 22);
    pkbSP->insertParentsStar(20, 22);
    pkbSP->insertNext(20, 22);
    pkbSP->insertModifies(22, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(20, "c");
    pkbSP->insertFollows(20, 23);
    pkbSP->insertFollowsStar(1, 23);
    pkbSP->insertFollowsStar(2, 23);
    pkbSP->insertFollowsStar(3, 23);
    pkbSP->insertFollowsStar(4, 23);
    pkbSP->insertFollowsStar(5, 23);
    pkbSP->insertFollowsStar(6, 23);
    pkbSP->insertFollowsStar(7, 23);
    pkbSP->insertFollowsStar(8, 23);
    pkbSP->insertFollowsStar(9, 23);
    pkbSP->insertFollowsStar(10, 23);
    pkbSP->insertFollowsStar(11, 23);
    pkbSP->insertFollowsStar(12, 23);
    pkbSP->insertFollowsStar(15, 23);
    pkbSP->insertFollowsStar(16, 23);
    pkbSP->insertFollowsStar(19, 23);
    pkbSP->insertFollowsStar(20, 23);
    pkbSP->insertNext(22, 23);
    pkbSP->insertNext(21, 23);
    pkbSP->insertNext(22, 23);
    pkbSP->insertCalls("main", "doNotModifyX");
    pkbSP->insertFollows(23, 24);
    pkbSP->insertFollowsStar(1, 24);
    pkbSP->insertFollowsStar(2, 24);
    pkbSP->insertFollowsStar(3, 24);
    pkbSP->insertFollowsStar(4, 24);
    pkbSP->insertFollowsStar(5, 24);
    pkbSP->insertFollowsStar(6, 24);
    pkbSP->insertFollowsStar(7, 24);
    pkbSP->insertFollowsStar(8, 24);
    pkbSP->insertFollowsStar(9, 24);
    pkbSP->insertFollowsStar(10, 24);
    pkbSP->insertFollowsStar(11, 24);
    pkbSP->insertFollowsStar(12, 24);
    pkbSP->insertFollowsStar(15, 24);
    pkbSP->insertFollowsStar(16, 24);
    pkbSP->insertFollowsStar(19, 24);
    pkbSP->insertFollowsStar(20, 24);
    pkbSP->insertFollowsStar(23, 24);
    pkbSP->insertNext(23, 24);
    pkbSP->insertUses(24, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(24, 25);
    pkbSP->insertParentsStar(24, 25);
    pkbSP->insertNext(24, 25);
    pkbSP->insertModifies(25, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(24, "y");
    pkbSP->insertUses(25, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(24, "x");
    pkbSP->insertParents(24, 26);
    pkbSP->insertParentsStar(24, 26);
    pkbSP->insertNext(24, 26);
    pkbSP->insertModifies(26, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(24, "c");
    pkbSP->insertFollows(24, 27);
    pkbSP->insertFollowsStar(1, 27);
    pkbSP->insertFollowsStar(2, 27);
    pkbSP->insertFollowsStar(3, 27);
    pkbSP->insertFollowsStar(4, 27);
    pkbSP->insertFollowsStar(5, 27);
    pkbSP->insertFollowsStar(6, 27);
    pkbSP->insertFollowsStar(7, 27);
    pkbSP->insertFollowsStar(8, 27);
    pkbSP->insertFollowsStar(9, 27);
    pkbSP->insertFollowsStar(10, 27);
    pkbSP->insertFollowsStar(11, 27);
    pkbSP->insertFollowsStar(12, 27);
    pkbSP->insertFollowsStar(15, 27);
    pkbSP->insertFollowsStar(16, 27);
    pkbSP->insertFollowsStar(19, 27);
    pkbSP->insertFollowsStar(20, 27);
    pkbSP->insertFollowsStar(23, 27);
    pkbSP->insertFollowsStar(24, 27);
    pkbSP->insertNext(26, 27);
    pkbSP->insertNext(25, 27);
    pkbSP->insertNext(26, 27);
    pkbSP->insertModifies(27, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(27, 28);
    pkbSP->insertFollowsStar(1, 28);
    pkbSP->insertFollowsStar(2, 28);
    pkbSP->insertFollowsStar(3, 28);
    pkbSP->insertFollowsStar(4, 28);
    pkbSP->insertFollowsStar(5, 28);
    pkbSP->insertFollowsStar(6, 28);
    pkbSP->insertFollowsStar(7, 28);
    pkbSP->insertFollowsStar(8, 28);
    pkbSP->insertFollowsStar(9, 28);
    pkbSP->insertFollowsStar(10, 28);
    pkbSP->insertFollowsStar(11, 28);
    pkbSP->insertFollowsStar(12, 28);
    pkbSP->insertFollowsStar(15, 28);
    pkbSP->insertFollowsStar(16, 28);
    pkbSP->insertFollowsStar(19, 28);
    pkbSP->insertFollowsStar(20, 28);
    pkbSP->insertFollowsStar(23, 28);
    pkbSP->insertFollowsStar(24, 28);
    pkbSP->insertFollowsStar(27, 28);
    pkbSP->insertNext(27, 28);
    pkbSP->insertUses(28, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(28, 29);
    pkbSP->insertParentsStar(28, 29);
    pkbSP->insertNext(28, 29);
    pkbSP->insertModifies(29, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(28, "y");
    pkbSP->insertUses(29, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(28, "x");
    pkbSP->insertParents(28, 30);
    pkbSP->insertParentsStar(28, 30);
    pkbSP->insertNext(28, 30);
    pkbSP->insertModifies(30, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(28, "c");
    pkbSP->insertFollows(28, 31);
    pkbSP->insertFollowsStar(1, 31);
    pkbSP->insertFollowsStar(2, 31);
    pkbSP->insertFollowsStar(3, 31);
    pkbSP->insertFollowsStar(4, 31);
    pkbSP->insertFollowsStar(5, 31);
    pkbSP->insertFollowsStar(6, 31);
    pkbSP->insertFollowsStar(7, 31);
    pkbSP->insertFollowsStar(8, 31);
    pkbSP->insertFollowsStar(9, 31);
    pkbSP->insertFollowsStar(10, 31);
    pkbSP->insertFollowsStar(11, 31);
    pkbSP->insertFollowsStar(12, 31);
    pkbSP->insertFollowsStar(15, 31);
    pkbSP->insertFollowsStar(16, 31);
    pkbSP->insertFollowsStar(19, 31);
    pkbSP->insertFollowsStar(20, 31);
    pkbSP->insertFollowsStar(23, 31);
    pkbSP->insertFollowsStar(24, 31);
    pkbSP->insertFollowsStar(27, 31);
    pkbSP->insertFollowsStar(28, 31);
    pkbSP->insertNext(30, 31);
    pkbSP->insertNext(29, 31);
    pkbSP->insertNext(30, 31);
    pkbSP->insertModifies(31, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(31, 32);
    pkbSP->insertFollowsStar(1, 32);
    pkbSP->insertFollowsStar(2, 32);
    pkbSP->insertFollowsStar(3, 32);
    pkbSP->insertFollowsStar(4, 32);
    pkbSP->insertFollowsStar(5, 32);
    pkbSP->insertFollowsStar(6, 32);
    pkbSP->insertFollowsStar(7, 32);
    pkbSP->insertFollowsStar(8, 32);
    pkbSP->insertFollowsStar(9, 32);
    pkbSP->insertFollowsStar(10, 32);
    pkbSP->insertFollowsStar(11, 32);
    pkbSP->insertFollowsStar(12, 32);
    pkbSP->insertFollowsStar(15, 32);
    pkbSP->insertFollowsStar(16, 32);
    pkbSP->insertFollowsStar(19, 32);
    pkbSP->insertFollowsStar(20, 32);
    pkbSP->insertFollowsStar(23, 32);
    pkbSP->insertFollowsStar(24, 32);
    pkbSP->insertFollowsStar(27, 32);
    pkbSP->insertFollowsStar(28, 32);
    pkbSP->insertFollowsStar(31, 32);
    pkbSP->insertNext(31, 32);
    pkbSP->insertUses(32, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(32, 33);
    pkbSP->insertParentsStar(32, 33);
    pkbSP->insertNext(32, 33);
    pkbSP->insertModifies(33, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertModifies(32, "x");
    pkbSP->insertNext(33, 32);
    pkbSP->insertFollows(32, 34);
    pkbSP->insertFollowsStar(1, 34);
    pkbSP->insertFollowsStar(2, 34);
    pkbSP->insertFollowsStar(3, 34);
    pkbSP->insertFollowsStar(4, 34);
    pkbSP->insertFollowsStar(5, 34);
    pkbSP->insertFollowsStar(6, 34);
    pkbSP->insertFollowsStar(7, 34);
    pkbSP->insertFollowsStar(8, 34);
    pkbSP->insertFollowsStar(9, 34);
    pkbSP->insertFollowsStar(10, 34);
    pkbSP->insertFollowsStar(11, 34);
    pkbSP->insertFollowsStar(12, 34);
    pkbSP->insertFollowsStar(15, 34);
    pkbSP->insertFollowsStar(16, 34);
    pkbSP->insertFollowsStar(19, 34);
    pkbSP->insertFollowsStar(20, 34);
    pkbSP->insertFollowsStar(23, 34);
    pkbSP->insertFollowsStar(24, 34);
    pkbSP->insertFollowsStar(27, 34);
    pkbSP->insertFollowsStar(28, 34);
    pkbSP->insertFollowsStar(31, 34);
    pkbSP->insertFollowsStar(32, 34);
    pkbSP->insertNext(32, 34);
    pkbSP->insertModifies(34, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(34, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(34, 35);
    pkbSP->insertFollowsStar(1, 35);
    pkbSP->insertFollowsStar(2, 35);
    pkbSP->insertFollowsStar(3, 35);
    pkbSP->insertFollowsStar(4, 35);
    pkbSP->insertFollowsStar(5, 35);
    pkbSP->insertFollowsStar(6, 35);
    pkbSP->insertFollowsStar(7, 35);
    pkbSP->insertFollowsStar(8, 35);
    pkbSP->insertFollowsStar(9, 35);
    pkbSP->insertFollowsStar(10, 35);
    pkbSP->insertFollowsStar(11, 35);
    pkbSP->insertFollowsStar(12, 35);
    pkbSP->insertFollowsStar(15, 35);
    pkbSP->insertFollowsStar(16, 35);
    pkbSP->insertFollowsStar(19, 35);
    pkbSP->insertFollowsStar(20, 35);
    pkbSP->insertFollowsStar(23, 35);
    pkbSP->insertFollowsStar(24, 35);
    pkbSP->insertFollowsStar(27, 35);
    pkbSP->insertFollowsStar(28, 35);
    pkbSP->insertFollowsStar(31, 35);
    pkbSP->insertFollowsStar(32, 35);
    pkbSP->insertFollowsStar(34, 35);
    pkbSP->insertNext(34, 35);
    pkbSP->insertModifies(35, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(35, 36);
    pkbSP->insertFollowsStar(1, 36);
    pkbSP->insertFollowsStar(2, 36);
    pkbSP->insertFollowsStar(3, 36);
    pkbSP->insertFollowsStar(4, 36);
    pkbSP->insertFollowsStar(5, 36);
    pkbSP->insertFollowsStar(6, 36);
    pkbSP->insertFollowsStar(7, 36);
    pkbSP->insertFollowsStar(8, 36);
    pkbSP->insertFollowsStar(9, 36);
    pkbSP->insertFollowsStar(10, 36);
    pkbSP->insertFollowsStar(11, 36);
    pkbSP->insertFollowsStar(12, 36);
    pkbSP->insertFollowsStar(15, 36);
    pkbSP->insertFollowsStar(16, 36);
    pkbSP->insertFollowsStar(19, 36);
    pkbSP->insertFollowsStar(20, 36);
    pkbSP->insertFollowsStar(23, 36);
    pkbSP->insertFollowsStar(24, 36);
    pkbSP->insertFollowsStar(27, 36);
    pkbSP->insertFollowsStar(28, 36);
    pkbSP->insertFollowsStar(31, 36);
    pkbSP->insertFollowsStar(32, 36);
    pkbSP->insertFollowsStar(34, 36);
    pkbSP->insertFollowsStar(35, 36);
    pkbSP->insertNext(35, 36);
    pkbSP->insertUses(36, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(36, 37);
    pkbSP->insertParentsStar(36, 37);
    pkbSP->insertNext(36, 37);
    pkbSP->insertModifies(37, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(36, "y");
    pkbSP->insertUses(37, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(36, "x");
    pkbSP->insertNext(37, 36);
    pkbSP->insertFollows(36, 38);
    pkbSP->insertFollowsStar(1, 38);
    pkbSP->insertFollowsStar(2, 38);
    pkbSP->insertFollowsStar(3, 38);
    pkbSP->insertFollowsStar(4, 38);
    pkbSP->insertFollowsStar(5, 38);
    pkbSP->insertFollowsStar(6, 38);
    pkbSP->insertFollowsStar(7, 38);
    pkbSP->insertFollowsStar(8, 38);
    pkbSP->insertFollowsStar(9, 38);
    pkbSP->insertFollowsStar(10, 38);
    pkbSP->insertFollowsStar(11, 38);
    pkbSP->insertFollowsStar(12, 38);
    pkbSP->insertFollowsStar(15, 38);
    pkbSP->insertFollowsStar(16, 38);
    pkbSP->insertFollowsStar(19, 38);
    pkbSP->insertFollowsStar(20, 38);
    pkbSP->insertFollowsStar(23, 38);
    pkbSP->insertFollowsStar(24, 38);
    pkbSP->insertFollowsStar(27, 38);
    pkbSP->insertFollowsStar(28, 38);
    pkbSP->insertFollowsStar(31, 38);
    pkbSP->insertFollowsStar(32, 38);
    pkbSP->insertFollowsStar(34, 38);
    pkbSP->insertFollowsStar(35, 38);
    pkbSP->insertFollowsStar(36, 38);
    pkbSP->insertNext(36, 38);
    pkbSP->insertModifies(38, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(38, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(38, 39);
    pkbSP->insertFollowsStar(1, 39);
    pkbSP->insertFollowsStar(2, 39);
    pkbSP->insertFollowsStar(3, 39);
    pkbSP->insertFollowsStar(4, 39);
    pkbSP->insertFollowsStar(5, 39);
    pkbSP->insertFollowsStar(6, 39);
    pkbSP->insertFollowsStar(7, 39);
    pkbSP->insertFollowsStar(8, 39);
    pkbSP->insertFollowsStar(9, 39);
    pkbSP->insertFollowsStar(10, 39);
    pkbSP->insertFollowsStar(11, 39);
    pkbSP->insertFollowsStar(12, 39);
    pkbSP->insertFollowsStar(15, 39);
    pkbSP->insertFollowsStar(16, 39);
    pkbSP->insertFollowsStar(19, 39);
    pkbSP->insertFollowsStar(20, 39);
    pkbSP->insertFollowsStar(23, 39);
    pkbSP->insertFollowsStar(24, 39);
    pkbSP->insertFollowsStar(27, 39);
    pkbSP->insertFollowsStar(28, 39);
    pkbSP->insertFollowsStar(31, 39);
    pkbSP->insertFollowsStar(32, 39);
    pkbSP->insertFollowsStar(34, 39);
    pkbSP->insertFollowsStar(35, 39);
    pkbSP->insertFollowsStar(36, 39);
    pkbSP->insertFollowsStar(38, 39);
    pkbSP->insertNext(38, 39);
    pkbSP->insertModifies(39, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(39, 40);
    pkbSP->insertFollowsStar(1, 40);
    pkbSP->insertFollowsStar(2, 40);
    pkbSP->insertFollowsStar(3, 40);
    pkbSP->insertFollowsStar(4, 40);
    pkbSP->insertFollowsStar(5, 40);
    pkbSP->insertFollowsStar(6, 40);
    pkbSP->insertFollowsStar(7, 40);
    pkbSP->insertFollowsStar(8, 40);
    pkbSP->insertFollowsStar(9, 40);
    pkbSP->insertFollowsStar(10, 40);
    pkbSP->insertFollowsStar(11, 40);
    pkbSP->insertFollowsStar(12, 40);
    pkbSP->insertFollowsStar(15, 40);
    pkbSP->insertFollowsStar(16, 40);
    pkbSP->insertFollowsStar(19, 40);
    pkbSP->insertFollowsStar(20, 40);
    pkbSP->insertFollowsStar(23, 40);
    pkbSP->insertFollowsStar(24, 40);
    pkbSP->insertFollowsStar(27, 40);
    pkbSP->insertFollowsStar(28, 40);
    pkbSP->insertFollowsStar(31, 40);
    pkbSP->insertFollowsStar(32, 40);
    pkbSP->insertFollowsStar(34, 40);
    pkbSP->insertFollowsStar(35, 40);
    pkbSP->insertFollowsStar(36, 40);
    pkbSP->insertFollowsStar(38, 40);
    pkbSP->insertFollowsStar(39, 40);
    pkbSP->insertNext(39, 40);
    pkbSP->insertUses(40, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(40, 41);
    pkbSP->insertParentsStar(40, 41);
    pkbSP->insertNext(40, 41);
    pkbSP->insertModifies(41, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertModifies(40, "x");
    pkbSP->insertParents(40, 42);
    pkbSP->insertParentsStar(40, 42);
    pkbSP->insertNext(40, 42);
    pkbSP->insertModifies(42, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(40, "c");
    pkbSP->insertFollows(40, 43);
    pkbSP->insertFollowsStar(1, 43);
    pkbSP->insertFollowsStar(2, 43);
    pkbSP->insertFollowsStar(3, 43);
    pkbSP->insertFollowsStar(4, 43);
    pkbSP->insertFollowsStar(5, 43);
    pkbSP->insertFollowsStar(6, 43);
    pkbSP->insertFollowsStar(7, 43);
    pkbSP->insertFollowsStar(8, 43);
    pkbSP->insertFollowsStar(9, 43);
    pkbSP->insertFollowsStar(10, 43);
    pkbSP->insertFollowsStar(11, 43);
    pkbSP->insertFollowsStar(12, 43);
    pkbSP->insertFollowsStar(15, 43);
    pkbSP->insertFollowsStar(16, 43);
    pkbSP->insertFollowsStar(19, 43);
    pkbSP->insertFollowsStar(20, 43);
    pkbSP->insertFollowsStar(23, 43);
    pkbSP->insertFollowsStar(24, 43);
    pkbSP->insertFollowsStar(27, 43);
    pkbSP->insertFollowsStar(28, 43);
    pkbSP->insertFollowsStar(31, 43);
    pkbSP->insertFollowsStar(32, 43);
    pkbSP->insertFollowsStar(34, 43);
    pkbSP->insertFollowsStar(35, 43);
    pkbSP->insertFollowsStar(36, 43);
    pkbSP->insertFollowsStar(38, 43);
    pkbSP->insertFollowsStar(39, 43);
    pkbSP->insertFollowsStar(40, 43);
    pkbSP->insertNext(42, 43);
    pkbSP->insertNext(41, 43);
    pkbSP->insertNext(42, 43);
    pkbSP->insertModifies(43, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(43, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(43, 44);
    pkbSP->insertFollowsStar(1, 44);
    pkbSP->insertFollowsStar(2, 44);
    pkbSP->insertFollowsStar(3, 44);
    pkbSP->insertFollowsStar(4, 44);
    pkbSP->insertFollowsStar(5, 44);
    pkbSP->insertFollowsStar(6, 44);
    pkbSP->insertFollowsStar(7, 44);
    pkbSP->insertFollowsStar(8, 44);
    pkbSP->insertFollowsStar(9, 44);
    pkbSP->insertFollowsStar(10, 44);
    pkbSP->insertFollowsStar(11, 44);
    pkbSP->insertFollowsStar(12, 44);
    pkbSP->insertFollowsStar(15, 44);
    pkbSP->insertFollowsStar(16, 44);
    pkbSP->insertFollowsStar(19, 44);
    pkbSP->insertFollowsStar(20, 44);
    pkbSP->insertFollowsStar(23, 44);
    pkbSP->insertFollowsStar(24, 44);
    pkbSP->insertFollowsStar(27, 44);
    pkbSP->insertFollowsStar(28, 44);
    pkbSP->insertFollowsStar(31, 44);
    pkbSP->insertFollowsStar(32, 44);
    pkbSP->insertFollowsStar(34, 44);
    pkbSP->insertFollowsStar(35, 44);
    pkbSP->insertFollowsStar(36, 44);
    pkbSP->insertFollowsStar(38, 44);
    pkbSP->insertFollowsStar(39, 44);
    pkbSP->insertFollowsStar(40, 44);
    pkbSP->insertFollowsStar(43, 44);
    pkbSP->insertNext(43, 44);
    pkbSP->insertModifies(44, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(44, 45);
    pkbSP->insertFollowsStar(1, 45);
    pkbSP->insertFollowsStar(2, 45);
    pkbSP->insertFollowsStar(3, 45);
    pkbSP->insertFollowsStar(4, 45);
    pkbSP->insertFollowsStar(5, 45);
    pkbSP->insertFollowsStar(6, 45);
    pkbSP->insertFollowsStar(7, 45);
    pkbSP->insertFollowsStar(8, 45);
    pkbSP->insertFollowsStar(9, 45);
    pkbSP->insertFollowsStar(10, 45);
    pkbSP->insertFollowsStar(11, 45);
    pkbSP->insertFollowsStar(12, 45);
    pkbSP->insertFollowsStar(15, 45);
    pkbSP->insertFollowsStar(16, 45);
    pkbSP->insertFollowsStar(19, 45);
    pkbSP->insertFollowsStar(20, 45);
    pkbSP->insertFollowsStar(23, 45);
    pkbSP->insertFollowsStar(24, 45);
    pkbSP->insertFollowsStar(27, 45);
    pkbSP->insertFollowsStar(28, 45);
    pkbSP->insertFollowsStar(31, 45);
    pkbSP->insertFollowsStar(32, 45);
    pkbSP->insertFollowsStar(34, 45);
    pkbSP->insertFollowsStar(35, 45);
    pkbSP->insertFollowsStar(36, 45);
    pkbSP->insertFollowsStar(38, 45);
    pkbSP->insertFollowsStar(39, 45);
    pkbSP->insertFollowsStar(40, 45);
    pkbSP->insertFollowsStar(43, 45);
    pkbSP->insertFollowsStar(44, 45);
    pkbSP->insertNext(44, 45);
    pkbSP->insertUses(45, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(45, 46);
    pkbSP->insertParentsStar(45, 46);
    pkbSP->insertNext(45, 46);
    pkbSP->insertModifies(46, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(45, "y");
    pkbSP->insertUses(46, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(45, "x");
    pkbSP->insertParents(45, 47);
    pkbSP->insertParentsStar(45, 47);
    pkbSP->insertNext(45, 47);
    pkbSP->insertModifies(47, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(45, "c");
    pkbSP->insertFollows(45, 48);
    pkbSP->insertFollowsStar(1, 48);
    pkbSP->insertFollowsStar(2, 48);
    pkbSP->insertFollowsStar(3, 48);
    pkbSP->insertFollowsStar(4, 48);
    pkbSP->insertFollowsStar(5, 48);
    pkbSP->insertFollowsStar(6, 48);
    pkbSP->insertFollowsStar(7, 48);
    pkbSP->insertFollowsStar(8, 48);
    pkbSP->insertFollowsStar(9, 48);
    pkbSP->insertFollowsStar(10, 48);
    pkbSP->insertFollowsStar(11, 48);
    pkbSP->insertFollowsStar(12, 48);
    pkbSP->insertFollowsStar(15, 48);
    pkbSP->insertFollowsStar(16, 48);
    pkbSP->insertFollowsStar(19, 48);
    pkbSP->insertFollowsStar(20, 48);
    pkbSP->insertFollowsStar(23, 48);
    pkbSP->insertFollowsStar(24, 48);
    pkbSP->insertFollowsStar(27, 48);
    pkbSP->insertFollowsStar(28, 48);
    pkbSP->insertFollowsStar(31, 48);
    pkbSP->insertFollowsStar(32, 48);
    pkbSP->insertFollowsStar(34, 48);
    pkbSP->insertFollowsStar(35, 48);
    pkbSP->insertFollowsStar(36, 48);
    pkbSP->insertFollowsStar(38, 48);
    pkbSP->insertFollowsStar(39, 48);
    pkbSP->insertFollowsStar(40, 48);
    pkbSP->insertFollowsStar(43, 48);
    pkbSP->insertFollowsStar(44, 48);
    pkbSP->insertFollowsStar(45, 48);
    pkbSP->insertNext(47, 48);
    pkbSP->insertNext(46, 48);
    pkbSP->insertNext(47, 48);
    pkbSP->insertModifies(48, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(48, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertModifies(49, "x");
    pkbSP->insertModifies("modifyX", "x");
    pkbSP->insertModifies(50, "y");
    pkbSP->insertModifies("doNotModifyX", "y");
    pkbSP->insertUses(50, "x");
    pkbSP->insertUses("doNotModifyX", "x");
    pkbSP->insertUses(23, "x");
    pkbSP->insertUses(9, "x");
    pkbSP->insertModifies(23, "y");
    pkbSP->insertModifies(9, "y");
    pkbSP->insertCallsStar("main", "doNotModifyX");
    pkbSP->insertUses("main", "x");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(19, "x");
    pkbSP->insertModifies(7, "x");
    pkbSP->insertCallsStar("main", "modifyX");
    pkbSP->insertModifies("main", "x");


    SECTION("No synonym") {
        REQUIRE(testQuery("Select BOOLEAN such that Affects (1, 2)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (1, 2)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (1, 4)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (2, 3)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (5, 6)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (7, 8)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (11, 13)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (15, 17)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (19, 21)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (23, 25)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (27, 29)", {"TRUE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (2, 1)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (4, 1)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (3, 2)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (6, 5)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (8, 7)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (13, 11)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (17, 15)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (21, 19)", {"FALSE"}, pkbQPS));
        REQUIRE(testQuery("Select BOOLEAN such that Affects (25, 23)", {"FALSE"}, pkbQPS));
    }
    SECTION("One synonym") {
        REQUIRE(testQuery(R"(assign a; Select a such that Affects(a, _))", {"1", "3", "11", "27", "31", "33", "35", "39", "41", "44"}, pkbQPS));
        REQUIRE(testQuery(R"(assign a; Select a such that Affects(_, a))", {"2", "4", "13", "29", "34", "37", "38", "43", "46", "48"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery(R"(assign a1; assign a2; Select <a1, a2> such that Affects(a1, a2))", {"1 2", "3 4", "11 13", "27 29", "31 34", "33 34", "35 37", "35 38", "39 43", "41 43", "44 46", "44 48"}, pkbQPS));
        REQUIRE(testQuery(R"(assign a; while w; Select <a, w> such that Affects(a, _) and Parent(w, a))", {"33 32"}, pkbQPS));
        REQUIRE(testQuery(R"(assign a; while w; Select <a, w> such that Affects(_, a) and Parent(w, a))", {"37 36"}, pkbQPS));
    }

    SECTION("Three synonyms") {
        REQUIRE(testQuery(R"(assign a1; assign a2; while w; Select <a1, a2, w> such that Affects(a1, a2) and Parent(w, a2))", {"35 37 36"}, pkbQPS));
    }
}

TEST_CASE("With integration tests") {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    std::shared_ptr<PKBSP> pkbSP = std::make_shared<PKBSP>(stores.first);
    std::shared_ptr<PKBQPS> pkbQPS = std::make_shared<PKBQPS>(stores.second);
    QPS qps;

    pkbSP->insertProcedure("main");
    pkbSP->insertAssign(1, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(1, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertAssign(2, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(2, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(3, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(3, {"3"});
    pkbSP->insertConstant("3");
    pkbSP->insertAssign(4, "z");
    pkbSP->insertVariable("z");
    pkbSP->insertPattern(4, {"x", "4", "+"});
    pkbSP->insertConstant("4");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(5, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(6, "a");
    pkbSP->insertVariable("a");
    pkbSP->insertPattern(6, {"x", "5", "+"});
    pkbSP->insertConstant("5");
    pkbSP->insertVariable("x");
    pkbSP->insertCall(7, "modifyX");
    pkbSP->insertAssign(8, "b");
    pkbSP->insertVariable("b");
    pkbSP->insertPattern(8, {"x", "6", "+"});
    pkbSP->insertConstant("6");
    pkbSP->insertVariable("x");
    pkbSP->insertCall(9, "doNotModifyX");
    pkbSP->insertAssign(10, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertPattern(10, {"x", "7", "+"});
    pkbSP->insertConstant("7");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(11, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(11, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertIf(12, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(13, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(13, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(14, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertRead(15, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertIf(16, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(17, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(17, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(18, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertCall(19, "modifyX");
    pkbSP->insertIf(20, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(21, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(21, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(22, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertCall(23, "doNotModifyX");
    pkbSP->insertIf(24, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(25, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(25, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(26, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertAssign(27, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(27, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertIf(28, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(29, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(29, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(30, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertAssign(31, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(31, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertWhile(32, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(33, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(33, {"2"});
    pkbSP->insertConstant("2");
    pkbSP->insertAssign(34, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(34, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(35, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(35, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertWhile(36, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(37, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(37, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(38, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(38, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(39, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(39, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertIf(40, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(41, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(41, {"2"});
    pkbSP->insertConstant("2");
    pkbSP->insertRead(42, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertAssign(43, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(43, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertAssign(44, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(44, {"1"});
    pkbSP->insertConstant("1");
    pkbSP->insertIf(45, {"b"});
    pkbSP->insertConstant("3");
    pkbSP->insertVariable("b");
    pkbSP->insertAssign(46, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(46, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertRead(47, "c");
    pkbSP->insertVariable("c");
    pkbSP->insertAssign(48, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(48, {"x", "2", "+"});
    pkbSP->insertConstant("2");
    pkbSP->insertVariable("x");
    pkbSP->insertPrint(49, "modifyX");
    pkbSP->insertVariable("modifyX");
    pkbSP->insertProcedure("modifyX");
    pkbSP->insertAssign(50, "x");
    pkbSP->insertVariable("x");
    pkbSP->insertPattern(50, {"2"});
    pkbSP->insertConstant("2");
    pkbSP->insertProcedure("doNotModifyX");
    pkbSP->insertAssign(51, "y");
    pkbSP->insertVariable("y");
    pkbSP->insertPattern(51, {"x"});
    pkbSP->insertVariable("x");
    pkbSP->insertModifies(1, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(1, 2);
    pkbSP->insertFollowsStar(1, 2);
    pkbSP->insertNext(1, 2);
    pkbSP->insertModifies(2, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(2, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(2, 3);
    pkbSP->insertFollowsStar(1, 3);
    pkbSP->insertFollowsStar(2, 3);
    pkbSP->insertNext(2, 3);
    pkbSP->insertModifies(3, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(3, 4);
    pkbSP->insertFollowsStar(1, 4);
    pkbSP->insertFollowsStar(2, 4);
    pkbSP->insertFollowsStar(3, 4);
    pkbSP->insertNext(3, 4);
    pkbSP->insertModifies(4, "z");
    pkbSP->insertModifies("main", "z");
    pkbSP->insertUses(4, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(4, 5);
    pkbSP->insertFollowsStar(1, 5);
    pkbSP->insertFollowsStar(2, 5);
    pkbSP->insertFollowsStar(3, 5);
    pkbSP->insertFollowsStar(4, 5);
    pkbSP->insertNext(4, 5);
    pkbSP->insertModifies(5, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(5, 6);
    pkbSP->insertFollowsStar(1, 6);
    pkbSP->insertFollowsStar(2, 6);
    pkbSP->insertFollowsStar(3, 6);
    pkbSP->insertFollowsStar(4, 6);
    pkbSP->insertFollowsStar(5, 6);
    pkbSP->insertNext(5, 6);
    pkbSP->insertModifies(6, "a");
    pkbSP->insertModifies("main", "a");
    pkbSP->insertUses(6, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(6, 7);
    pkbSP->insertFollowsStar(1, 7);
    pkbSP->insertFollowsStar(2, 7);
    pkbSP->insertFollowsStar(3, 7);
    pkbSP->insertFollowsStar(4, 7);
    pkbSP->insertFollowsStar(5, 7);
    pkbSP->insertFollowsStar(6, 7);
    pkbSP->insertNext(6, 7);
    pkbSP->insertCalls("main", "modifyX");
    pkbSP->insertFollows(7, 8);
    pkbSP->insertFollowsStar(1, 8);
    pkbSP->insertFollowsStar(2, 8);
    pkbSP->insertFollowsStar(3, 8);
    pkbSP->insertFollowsStar(4, 8);
    pkbSP->insertFollowsStar(5, 8);
    pkbSP->insertFollowsStar(6, 8);
    pkbSP->insertFollowsStar(7, 8);
    pkbSP->insertNext(7, 8);
    pkbSP->insertModifies(8, "b");
    pkbSP->insertModifies("main", "b");
    pkbSP->insertUses(8, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(8, 9);
    pkbSP->insertFollowsStar(1, 9);
    pkbSP->insertFollowsStar(2, 9);
    pkbSP->insertFollowsStar(3, 9);
    pkbSP->insertFollowsStar(4, 9);
    pkbSP->insertFollowsStar(5, 9);
    pkbSP->insertFollowsStar(6, 9);
    pkbSP->insertFollowsStar(7, 9);
    pkbSP->insertFollowsStar(8, 9);
    pkbSP->insertNext(8, 9);
    pkbSP->insertCalls("main", "doNotModifyX");
    pkbSP->insertFollows(9, 10);
    pkbSP->insertFollowsStar(1, 10);
    pkbSP->insertFollowsStar(2, 10);
    pkbSP->insertFollowsStar(3, 10);
    pkbSP->insertFollowsStar(4, 10);
    pkbSP->insertFollowsStar(5, 10);
    pkbSP->insertFollowsStar(6, 10);
    pkbSP->insertFollowsStar(7, 10);
    pkbSP->insertFollowsStar(8, 10);
    pkbSP->insertFollowsStar(9, 10);
    pkbSP->insertNext(9, 10);
    pkbSP->insertModifies(10, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertUses(10, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(10, 11);
    pkbSP->insertFollowsStar(1, 11);
    pkbSP->insertFollowsStar(2, 11);
    pkbSP->insertFollowsStar(3, 11);
    pkbSP->insertFollowsStar(4, 11);
    pkbSP->insertFollowsStar(5, 11);
    pkbSP->insertFollowsStar(6, 11);
    pkbSP->insertFollowsStar(7, 11);
    pkbSP->insertFollowsStar(8, 11);
    pkbSP->insertFollowsStar(9, 11);
    pkbSP->insertFollowsStar(10, 11);
    pkbSP->insertNext(10, 11);
    pkbSP->insertModifies(11, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(11, 12);
    pkbSP->insertFollowsStar(1, 12);
    pkbSP->insertFollowsStar(2, 12);
    pkbSP->insertFollowsStar(3, 12);
    pkbSP->insertFollowsStar(4, 12);
    pkbSP->insertFollowsStar(5, 12);
    pkbSP->insertFollowsStar(6, 12);
    pkbSP->insertFollowsStar(7, 12);
    pkbSP->insertFollowsStar(8, 12);
    pkbSP->insertFollowsStar(9, 12);
    pkbSP->insertFollowsStar(10, 12);
    pkbSP->insertFollowsStar(11, 12);
    pkbSP->insertNext(11, 12);
    pkbSP->insertUses(12, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(12, 13);
    pkbSP->insertParentsStar(12, 13);
    pkbSP->insertNext(12, 13);
    pkbSP->insertModifies(13, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(12, "y");
    pkbSP->insertUses(13, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(12, "x");
    pkbSP->insertParents(12, 14);
    pkbSP->insertParentsStar(12, 14);
    pkbSP->insertNext(12, 14);
    pkbSP->insertModifies(14, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(12, "c");
    pkbSP->insertFollows(12, 15);
    pkbSP->insertFollowsStar(1, 15);
    pkbSP->insertFollowsStar(2, 15);
    pkbSP->insertFollowsStar(3, 15);
    pkbSP->insertFollowsStar(4, 15);
    pkbSP->insertFollowsStar(5, 15);
    pkbSP->insertFollowsStar(6, 15);
    pkbSP->insertFollowsStar(7, 15);
    pkbSP->insertFollowsStar(8, 15);
    pkbSP->insertFollowsStar(9, 15);
    pkbSP->insertFollowsStar(10, 15);
    pkbSP->insertFollowsStar(11, 15);
    pkbSP->insertFollowsStar(12, 15);
    pkbSP->insertNext(14, 15);
    pkbSP->insertNext(13, 15);
    pkbSP->insertNext(14, 15);
    pkbSP->insertModifies(15, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(15, 16);
    pkbSP->insertFollowsStar(1, 16);
    pkbSP->insertFollowsStar(2, 16);
    pkbSP->insertFollowsStar(3, 16);
    pkbSP->insertFollowsStar(4, 16);
    pkbSP->insertFollowsStar(5, 16);
    pkbSP->insertFollowsStar(6, 16);
    pkbSP->insertFollowsStar(7, 16);
    pkbSP->insertFollowsStar(8, 16);
    pkbSP->insertFollowsStar(9, 16);
    pkbSP->insertFollowsStar(10, 16);
    pkbSP->insertFollowsStar(11, 16);
    pkbSP->insertFollowsStar(12, 16);
    pkbSP->insertFollowsStar(15, 16);
    pkbSP->insertNext(15, 16);
    pkbSP->insertUses(16, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(16, 17);
    pkbSP->insertParentsStar(16, 17);
    pkbSP->insertNext(16, 17);
    pkbSP->insertModifies(17, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(16, "y");
    pkbSP->insertUses(17, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(16, "x");
    pkbSP->insertParents(16, 18);
    pkbSP->insertParentsStar(16, 18);
    pkbSP->insertNext(16, 18);
    pkbSP->insertModifies(18, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(16, "c");
    pkbSP->insertFollows(16, 19);
    pkbSP->insertFollowsStar(1, 19);
    pkbSP->insertFollowsStar(2, 19);
    pkbSP->insertFollowsStar(3, 19);
    pkbSP->insertFollowsStar(4, 19);
    pkbSP->insertFollowsStar(5, 19);
    pkbSP->insertFollowsStar(6, 19);
    pkbSP->insertFollowsStar(7, 19);
    pkbSP->insertFollowsStar(8, 19);
    pkbSP->insertFollowsStar(9, 19);
    pkbSP->insertFollowsStar(10, 19);
    pkbSP->insertFollowsStar(11, 19);
    pkbSP->insertFollowsStar(12, 19);
    pkbSP->insertFollowsStar(15, 19);
    pkbSP->insertFollowsStar(16, 19);
    pkbSP->insertNext(18, 19);
    pkbSP->insertNext(17, 19);
    pkbSP->insertNext(18, 19);
    pkbSP->insertCalls("main", "modifyX");
    pkbSP->insertFollows(19, 20);
    pkbSP->insertFollowsStar(1, 20);
    pkbSP->insertFollowsStar(2, 20);
    pkbSP->insertFollowsStar(3, 20);
    pkbSP->insertFollowsStar(4, 20);
    pkbSP->insertFollowsStar(5, 20);
    pkbSP->insertFollowsStar(6, 20);
    pkbSP->insertFollowsStar(7, 20);
    pkbSP->insertFollowsStar(8, 20);
    pkbSP->insertFollowsStar(9, 20);
    pkbSP->insertFollowsStar(10, 20);
    pkbSP->insertFollowsStar(11, 20);
    pkbSP->insertFollowsStar(12, 20);
    pkbSP->insertFollowsStar(15, 20);
    pkbSP->insertFollowsStar(16, 20);
    pkbSP->insertFollowsStar(19, 20);
    pkbSP->insertNext(19, 20);
    pkbSP->insertUses(20, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(20, 21);
    pkbSP->insertParentsStar(20, 21);
    pkbSP->insertNext(20, 21);
    pkbSP->insertModifies(21, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(20, "y");
    pkbSP->insertUses(21, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(20, "x");
    pkbSP->insertParents(20, 22);
    pkbSP->insertParentsStar(20, 22);
    pkbSP->insertNext(20, 22);
    pkbSP->insertModifies(22, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(20, "c");
    pkbSP->insertFollows(20, 23);
    pkbSP->insertFollowsStar(1, 23);
    pkbSP->insertFollowsStar(2, 23);
    pkbSP->insertFollowsStar(3, 23);
    pkbSP->insertFollowsStar(4, 23);
    pkbSP->insertFollowsStar(5, 23);
    pkbSP->insertFollowsStar(6, 23);
    pkbSP->insertFollowsStar(7, 23);
    pkbSP->insertFollowsStar(8, 23);
    pkbSP->insertFollowsStar(9, 23);
    pkbSP->insertFollowsStar(10, 23);
    pkbSP->insertFollowsStar(11, 23);
    pkbSP->insertFollowsStar(12, 23);
    pkbSP->insertFollowsStar(15, 23);
    pkbSP->insertFollowsStar(16, 23);
    pkbSP->insertFollowsStar(19, 23);
    pkbSP->insertFollowsStar(20, 23);
    pkbSP->insertNext(22, 23);
    pkbSP->insertNext(21, 23);
    pkbSP->insertNext(22, 23);
    pkbSP->insertCalls("main", "doNotModifyX");
    pkbSP->insertFollows(23, 24);
    pkbSP->insertFollowsStar(1, 24);
    pkbSP->insertFollowsStar(2, 24);
    pkbSP->insertFollowsStar(3, 24);
    pkbSP->insertFollowsStar(4, 24);
    pkbSP->insertFollowsStar(5, 24);
    pkbSP->insertFollowsStar(6, 24);
    pkbSP->insertFollowsStar(7, 24);
    pkbSP->insertFollowsStar(8, 24);
    pkbSP->insertFollowsStar(9, 24);
    pkbSP->insertFollowsStar(10, 24);
    pkbSP->insertFollowsStar(11, 24);
    pkbSP->insertFollowsStar(12, 24);
    pkbSP->insertFollowsStar(15, 24);
    pkbSP->insertFollowsStar(16, 24);
    pkbSP->insertFollowsStar(19, 24);
    pkbSP->insertFollowsStar(20, 24);
    pkbSP->insertFollowsStar(23, 24);
    pkbSP->insertNext(23, 24);
    pkbSP->insertUses(24, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(24, 25);
    pkbSP->insertParentsStar(24, 25);
    pkbSP->insertNext(24, 25);
    pkbSP->insertModifies(25, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(24, "y");
    pkbSP->insertUses(25, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(24, "x");
    pkbSP->insertParents(24, 26);
    pkbSP->insertParentsStar(24, 26);
    pkbSP->insertNext(24, 26);
    pkbSP->insertModifies(26, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(24, "c");
    pkbSP->insertFollows(24, 27);
    pkbSP->insertFollowsStar(1, 27);
    pkbSP->insertFollowsStar(2, 27);
    pkbSP->insertFollowsStar(3, 27);
    pkbSP->insertFollowsStar(4, 27);
    pkbSP->insertFollowsStar(5, 27);
    pkbSP->insertFollowsStar(6, 27);
    pkbSP->insertFollowsStar(7, 27);
    pkbSP->insertFollowsStar(8, 27);
    pkbSP->insertFollowsStar(9, 27);
    pkbSP->insertFollowsStar(10, 27);
    pkbSP->insertFollowsStar(11, 27);
    pkbSP->insertFollowsStar(12, 27);
    pkbSP->insertFollowsStar(15, 27);
    pkbSP->insertFollowsStar(16, 27);
    pkbSP->insertFollowsStar(19, 27);
    pkbSP->insertFollowsStar(20, 27);
    pkbSP->insertFollowsStar(23, 27);
    pkbSP->insertFollowsStar(24, 27);
    pkbSP->insertNext(26, 27);
    pkbSP->insertNext(25, 27);
    pkbSP->insertNext(26, 27);
    pkbSP->insertModifies(27, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(27, 28);
    pkbSP->insertFollowsStar(1, 28);
    pkbSP->insertFollowsStar(2, 28);
    pkbSP->insertFollowsStar(3, 28);
    pkbSP->insertFollowsStar(4, 28);
    pkbSP->insertFollowsStar(5, 28);
    pkbSP->insertFollowsStar(6, 28);
    pkbSP->insertFollowsStar(7, 28);
    pkbSP->insertFollowsStar(8, 28);
    pkbSP->insertFollowsStar(9, 28);
    pkbSP->insertFollowsStar(10, 28);
    pkbSP->insertFollowsStar(11, 28);
    pkbSP->insertFollowsStar(12, 28);
    pkbSP->insertFollowsStar(15, 28);
    pkbSP->insertFollowsStar(16, 28);
    pkbSP->insertFollowsStar(19, 28);
    pkbSP->insertFollowsStar(20, 28);
    pkbSP->insertFollowsStar(23, 28);
    pkbSP->insertFollowsStar(24, 28);
    pkbSP->insertFollowsStar(27, 28);
    pkbSP->insertNext(27, 28);
    pkbSP->insertUses(28, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(28, 29);
    pkbSP->insertParentsStar(28, 29);
    pkbSP->insertNext(28, 29);
    pkbSP->insertModifies(29, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(28, "y");
    pkbSP->insertUses(29, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(28, "x");
    pkbSP->insertParents(28, 30);
    pkbSP->insertParentsStar(28, 30);
    pkbSP->insertNext(28, 30);
    pkbSP->insertModifies(30, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(28, "c");
    pkbSP->insertFollows(28, 31);
    pkbSP->insertFollowsStar(1, 31);
    pkbSP->insertFollowsStar(2, 31);
    pkbSP->insertFollowsStar(3, 31);
    pkbSP->insertFollowsStar(4, 31);
    pkbSP->insertFollowsStar(5, 31);
    pkbSP->insertFollowsStar(6, 31);
    pkbSP->insertFollowsStar(7, 31);
    pkbSP->insertFollowsStar(8, 31);
    pkbSP->insertFollowsStar(9, 31);
    pkbSP->insertFollowsStar(10, 31);
    pkbSP->insertFollowsStar(11, 31);
    pkbSP->insertFollowsStar(12, 31);
    pkbSP->insertFollowsStar(15, 31);
    pkbSP->insertFollowsStar(16, 31);
    pkbSP->insertFollowsStar(19, 31);
    pkbSP->insertFollowsStar(20, 31);
    pkbSP->insertFollowsStar(23, 31);
    pkbSP->insertFollowsStar(24, 31);
    pkbSP->insertFollowsStar(27, 31);
    pkbSP->insertFollowsStar(28, 31);
    pkbSP->insertNext(30, 31);
    pkbSP->insertNext(29, 31);
    pkbSP->insertNext(30, 31);
    pkbSP->insertModifies(31, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(31, 32);
    pkbSP->insertFollowsStar(1, 32);
    pkbSP->insertFollowsStar(2, 32);
    pkbSP->insertFollowsStar(3, 32);
    pkbSP->insertFollowsStar(4, 32);
    pkbSP->insertFollowsStar(5, 32);
    pkbSP->insertFollowsStar(6, 32);
    pkbSP->insertFollowsStar(7, 32);
    pkbSP->insertFollowsStar(8, 32);
    pkbSP->insertFollowsStar(9, 32);
    pkbSP->insertFollowsStar(10, 32);
    pkbSP->insertFollowsStar(11, 32);
    pkbSP->insertFollowsStar(12, 32);
    pkbSP->insertFollowsStar(15, 32);
    pkbSP->insertFollowsStar(16, 32);
    pkbSP->insertFollowsStar(19, 32);
    pkbSP->insertFollowsStar(20, 32);
    pkbSP->insertFollowsStar(23, 32);
    pkbSP->insertFollowsStar(24, 32);
    pkbSP->insertFollowsStar(27, 32);
    pkbSP->insertFollowsStar(28, 32);
    pkbSP->insertFollowsStar(31, 32);
    pkbSP->insertNext(31, 32);
    pkbSP->insertUses(32, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(32, 33);
    pkbSP->insertParentsStar(32, 33);
    pkbSP->insertNext(32, 33);
    pkbSP->insertModifies(33, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertModifies(32, "x");
    pkbSP->insertNext(33, 32);
    pkbSP->insertFollows(32, 34);
    pkbSP->insertFollowsStar(1, 34);
    pkbSP->insertFollowsStar(2, 34);
    pkbSP->insertFollowsStar(3, 34);
    pkbSP->insertFollowsStar(4, 34);
    pkbSP->insertFollowsStar(5, 34);
    pkbSP->insertFollowsStar(6, 34);
    pkbSP->insertFollowsStar(7, 34);
    pkbSP->insertFollowsStar(8, 34);
    pkbSP->insertFollowsStar(9, 34);
    pkbSP->insertFollowsStar(10, 34);
    pkbSP->insertFollowsStar(11, 34);
    pkbSP->insertFollowsStar(12, 34);
    pkbSP->insertFollowsStar(15, 34);
    pkbSP->insertFollowsStar(16, 34);
    pkbSP->insertFollowsStar(19, 34);
    pkbSP->insertFollowsStar(20, 34);
    pkbSP->insertFollowsStar(23, 34);
    pkbSP->insertFollowsStar(24, 34);
    pkbSP->insertFollowsStar(27, 34);
    pkbSP->insertFollowsStar(28, 34);
    pkbSP->insertFollowsStar(31, 34);
    pkbSP->insertFollowsStar(32, 34);
    pkbSP->insertNext(32, 34);
    pkbSP->insertModifies(34, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(34, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(34, 35);
    pkbSP->insertFollowsStar(1, 35);
    pkbSP->insertFollowsStar(2, 35);
    pkbSP->insertFollowsStar(3, 35);
    pkbSP->insertFollowsStar(4, 35);
    pkbSP->insertFollowsStar(5, 35);
    pkbSP->insertFollowsStar(6, 35);
    pkbSP->insertFollowsStar(7, 35);
    pkbSP->insertFollowsStar(8, 35);
    pkbSP->insertFollowsStar(9, 35);
    pkbSP->insertFollowsStar(10, 35);
    pkbSP->insertFollowsStar(11, 35);
    pkbSP->insertFollowsStar(12, 35);
    pkbSP->insertFollowsStar(15, 35);
    pkbSP->insertFollowsStar(16, 35);
    pkbSP->insertFollowsStar(19, 35);
    pkbSP->insertFollowsStar(20, 35);
    pkbSP->insertFollowsStar(23, 35);
    pkbSP->insertFollowsStar(24, 35);
    pkbSP->insertFollowsStar(27, 35);
    pkbSP->insertFollowsStar(28, 35);
    pkbSP->insertFollowsStar(31, 35);
    pkbSP->insertFollowsStar(32, 35);
    pkbSP->insertFollowsStar(34, 35);
    pkbSP->insertNext(34, 35);
    pkbSP->insertModifies(35, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(35, 36);
    pkbSP->insertFollowsStar(1, 36);
    pkbSP->insertFollowsStar(2, 36);
    pkbSP->insertFollowsStar(3, 36);
    pkbSP->insertFollowsStar(4, 36);
    pkbSP->insertFollowsStar(5, 36);
    pkbSP->insertFollowsStar(6, 36);
    pkbSP->insertFollowsStar(7, 36);
    pkbSP->insertFollowsStar(8, 36);
    pkbSP->insertFollowsStar(9, 36);
    pkbSP->insertFollowsStar(10, 36);
    pkbSP->insertFollowsStar(11, 36);
    pkbSP->insertFollowsStar(12, 36);
    pkbSP->insertFollowsStar(15, 36);
    pkbSP->insertFollowsStar(16, 36);
    pkbSP->insertFollowsStar(19, 36);
    pkbSP->insertFollowsStar(20, 36);
    pkbSP->insertFollowsStar(23, 36);
    pkbSP->insertFollowsStar(24, 36);
    pkbSP->insertFollowsStar(27, 36);
    pkbSP->insertFollowsStar(28, 36);
    pkbSP->insertFollowsStar(31, 36);
    pkbSP->insertFollowsStar(32, 36);
    pkbSP->insertFollowsStar(34, 36);
    pkbSP->insertFollowsStar(35, 36);
    pkbSP->insertNext(35, 36);
    pkbSP->insertUses(36, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(36, 37);
    pkbSP->insertParentsStar(36, 37);
    pkbSP->insertNext(36, 37);
    pkbSP->insertModifies(37, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(36, "y");
    pkbSP->insertUses(37, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(36, "x");
    pkbSP->insertNext(37, 36);
    pkbSP->insertFollows(36, 38);
    pkbSP->insertFollowsStar(1, 38);
    pkbSP->insertFollowsStar(2, 38);
    pkbSP->insertFollowsStar(3, 38);
    pkbSP->insertFollowsStar(4, 38);
    pkbSP->insertFollowsStar(5, 38);
    pkbSP->insertFollowsStar(6, 38);
    pkbSP->insertFollowsStar(7, 38);
    pkbSP->insertFollowsStar(8, 38);
    pkbSP->insertFollowsStar(9, 38);
    pkbSP->insertFollowsStar(10, 38);
    pkbSP->insertFollowsStar(11, 38);
    pkbSP->insertFollowsStar(12, 38);
    pkbSP->insertFollowsStar(15, 38);
    pkbSP->insertFollowsStar(16, 38);
    pkbSP->insertFollowsStar(19, 38);
    pkbSP->insertFollowsStar(20, 38);
    pkbSP->insertFollowsStar(23, 38);
    pkbSP->insertFollowsStar(24, 38);
    pkbSP->insertFollowsStar(27, 38);
    pkbSP->insertFollowsStar(28, 38);
    pkbSP->insertFollowsStar(31, 38);
    pkbSP->insertFollowsStar(32, 38);
    pkbSP->insertFollowsStar(34, 38);
    pkbSP->insertFollowsStar(35, 38);
    pkbSP->insertFollowsStar(36, 38);
    pkbSP->insertNext(36, 38);
    pkbSP->insertModifies(38, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(38, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(38, 39);
    pkbSP->insertFollowsStar(1, 39);
    pkbSP->insertFollowsStar(2, 39);
    pkbSP->insertFollowsStar(3, 39);
    pkbSP->insertFollowsStar(4, 39);
    pkbSP->insertFollowsStar(5, 39);
    pkbSP->insertFollowsStar(6, 39);
    pkbSP->insertFollowsStar(7, 39);
    pkbSP->insertFollowsStar(8, 39);
    pkbSP->insertFollowsStar(9, 39);
    pkbSP->insertFollowsStar(10, 39);
    pkbSP->insertFollowsStar(11, 39);
    pkbSP->insertFollowsStar(12, 39);
    pkbSP->insertFollowsStar(15, 39);
    pkbSP->insertFollowsStar(16, 39);
    pkbSP->insertFollowsStar(19, 39);
    pkbSP->insertFollowsStar(20, 39);
    pkbSP->insertFollowsStar(23, 39);
    pkbSP->insertFollowsStar(24, 39);
    pkbSP->insertFollowsStar(27, 39);
    pkbSP->insertFollowsStar(28, 39);
    pkbSP->insertFollowsStar(31, 39);
    pkbSP->insertFollowsStar(32, 39);
    pkbSP->insertFollowsStar(34, 39);
    pkbSP->insertFollowsStar(35, 39);
    pkbSP->insertFollowsStar(36, 39);
    pkbSP->insertFollowsStar(38, 39);
    pkbSP->insertNext(38, 39);
    pkbSP->insertModifies(39, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(39, 40);
    pkbSP->insertFollowsStar(1, 40);
    pkbSP->insertFollowsStar(2, 40);
    pkbSP->insertFollowsStar(3, 40);
    pkbSP->insertFollowsStar(4, 40);
    pkbSP->insertFollowsStar(5, 40);
    pkbSP->insertFollowsStar(6, 40);
    pkbSP->insertFollowsStar(7, 40);
    pkbSP->insertFollowsStar(8, 40);
    pkbSP->insertFollowsStar(9, 40);
    pkbSP->insertFollowsStar(10, 40);
    pkbSP->insertFollowsStar(11, 40);
    pkbSP->insertFollowsStar(12, 40);
    pkbSP->insertFollowsStar(15, 40);
    pkbSP->insertFollowsStar(16, 40);
    pkbSP->insertFollowsStar(19, 40);
    pkbSP->insertFollowsStar(20, 40);
    pkbSP->insertFollowsStar(23, 40);
    pkbSP->insertFollowsStar(24, 40);
    pkbSP->insertFollowsStar(27, 40);
    pkbSP->insertFollowsStar(28, 40);
    pkbSP->insertFollowsStar(31, 40);
    pkbSP->insertFollowsStar(32, 40);
    pkbSP->insertFollowsStar(34, 40);
    pkbSP->insertFollowsStar(35, 40);
    pkbSP->insertFollowsStar(36, 40);
    pkbSP->insertFollowsStar(38, 40);
    pkbSP->insertFollowsStar(39, 40);
    pkbSP->insertNext(39, 40);
    pkbSP->insertUses(40, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(40, 41);
    pkbSP->insertParentsStar(40, 41);
    pkbSP->insertNext(40, 41);
    pkbSP->insertModifies(41, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertModifies(40, "x");
    pkbSP->insertParents(40, 42);
    pkbSP->insertParentsStar(40, 42);
    pkbSP->insertNext(40, 42);
    pkbSP->insertModifies(42, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(40, "c");
    pkbSP->insertFollows(40, 43);
    pkbSP->insertFollowsStar(1, 43);
    pkbSP->insertFollowsStar(2, 43);
    pkbSP->insertFollowsStar(3, 43);
    pkbSP->insertFollowsStar(4, 43);
    pkbSP->insertFollowsStar(5, 43);
    pkbSP->insertFollowsStar(6, 43);
    pkbSP->insertFollowsStar(7, 43);
    pkbSP->insertFollowsStar(8, 43);
    pkbSP->insertFollowsStar(9, 43);
    pkbSP->insertFollowsStar(10, 43);
    pkbSP->insertFollowsStar(11, 43);
    pkbSP->insertFollowsStar(12, 43);
    pkbSP->insertFollowsStar(15, 43);
    pkbSP->insertFollowsStar(16, 43);
    pkbSP->insertFollowsStar(19, 43);
    pkbSP->insertFollowsStar(20, 43);
    pkbSP->insertFollowsStar(23, 43);
    pkbSP->insertFollowsStar(24, 43);
    pkbSP->insertFollowsStar(27, 43);
    pkbSP->insertFollowsStar(28, 43);
    pkbSP->insertFollowsStar(31, 43);
    pkbSP->insertFollowsStar(32, 43);
    pkbSP->insertFollowsStar(34, 43);
    pkbSP->insertFollowsStar(35, 43);
    pkbSP->insertFollowsStar(36, 43);
    pkbSP->insertFollowsStar(38, 43);
    pkbSP->insertFollowsStar(39, 43);
    pkbSP->insertFollowsStar(40, 43);
    pkbSP->insertNext(42, 43);
    pkbSP->insertNext(41, 43);
    pkbSP->insertNext(42, 43);
    pkbSP->insertModifies(43, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(43, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(43, 44);
    pkbSP->insertFollowsStar(1, 44);
    pkbSP->insertFollowsStar(2, 44);
    pkbSP->insertFollowsStar(3, 44);
    pkbSP->insertFollowsStar(4, 44);
    pkbSP->insertFollowsStar(5, 44);
    pkbSP->insertFollowsStar(6, 44);
    pkbSP->insertFollowsStar(7, 44);
    pkbSP->insertFollowsStar(8, 44);
    pkbSP->insertFollowsStar(9, 44);
    pkbSP->insertFollowsStar(10, 44);
    pkbSP->insertFollowsStar(11, 44);
    pkbSP->insertFollowsStar(12, 44);
    pkbSP->insertFollowsStar(15, 44);
    pkbSP->insertFollowsStar(16, 44);
    pkbSP->insertFollowsStar(19, 44);
    pkbSP->insertFollowsStar(20, 44);
    pkbSP->insertFollowsStar(23, 44);
    pkbSP->insertFollowsStar(24, 44);
    pkbSP->insertFollowsStar(27, 44);
    pkbSP->insertFollowsStar(28, 44);
    pkbSP->insertFollowsStar(31, 44);
    pkbSP->insertFollowsStar(32, 44);
    pkbSP->insertFollowsStar(34, 44);
    pkbSP->insertFollowsStar(35, 44);
    pkbSP->insertFollowsStar(36, 44);
    pkbSP->insertFollowsStar(38, 44);
    pkbSP->insertFollowsStar(39, 44);
    pkbSP->insertFollowsStar(40, 44);
    pkbSP->insertFollowsStar(43, 44);
    pkbSP->insertNext(43, 44);
    pkbSP->insertModifies(44, "x");
    pkbSP->insertModifies("main", "x");
    pkbSP->insertFollows(44, 45);
    pkbSP->insertFollowsStar(1, 45);
    pkbSP->insertFollowsStar(2, 45);
    pkbSP->insertFollowsStar(3, 45);
    pkbSP->insertFollowsStar(4, 45);
    pkbSP->insertFollowsStar(5, 45);
    pkbSP->insertFollowsStar(6, 45);
    pkbSP->insertFollowsStar(7, 45);
    pkbSP->insertFollowsStar(8, 45);
    pkbSP->insertFollowsStar(9, 45);
    pkbSP->insertFollowsStar(10, 45);
    pkbSP->insertFollowsStar(11, 45);
    pkbSP->insertFollowsStar(12, 45);
    pkbSP->insertFollowsStar(15, 45);
    pkbSP->insertFollowsStar(16, 45);
    pkbSP->insertFollowsStar(19, 45);
    pkbSP->insertFollowsStar(20, 45);
    pkbSP->insertFollowsStar(23, 45);
    pkbSP->insertFollowsStar(24, 45);
    pkbSP->insertFollowsStar(27, 45);
    pkbSP->insertFollowsStar(28, 45);
    pkbSP->insertFollowsStar(31, 45);
    pkbSP->insertFollowsStar(32, 45);
    pkbSP->insertFollowsStar(34, 45);
    pkbSP->insertFollowsStar(35, 45);
    pkbSP->insertFollowsStar(36, 45);
    pkbSP->insertFollowsStar(38, 45);
    pkbSP->insertFollowsStar(39, 45);
    pkbSP->insertFollowsStar(40, 45);
    pkbSP->insertFollowsStar(43, 45);
    pkbSP->insertFollowsStar(44, 45);
    pkbSP->insertNext(44, 45);
    pkbSP->insertUses(45, "b");
    pkbSP->insertUses("main", "b");
    pkbSP->insertParents(45, 46);
    pkbSP->insertParentsStar(45, 46);
    pkbSP->insertNext(45, 46);
    pkbSP->insertModifies(46, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(45, "y");
    pkbSP->insertUses(46, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertUses(45, "x");
    pkbSP->insertParents(45, 47);
    pkbSP->insertParentsStar(45, 47);
    pkbSP->insertNext(45, 47);
    pkbSP->insertModifies(47, "c");
    pkbSP->insertModifies("main", "c");
    pkbSP->insertModifies(45, "c");
    pkbSP->insertFollows(45, 48);
    pkbSP->insertFollowsStar(1, 48);
    pkbSP->insertFollowsStar(2, 48);
    pkbSP->insertFollowsStar(3, 48);
    pkbSP->insertFollowsStar(4, 48);
    pkbSP->insertFollowsStar(5, 48);
    pkbSP->insertFollowsStar(6, 48);
    pkbSP->insertFollowsStar(7, 48);
    pkbSP->insertFollowsStar(8, 48);
    pkbSP->insertFollowsStar(9, 48);
    pkbSP->insertFollowsStar(10, 48);
    pkbSP->insertFollowsStar(11, 48);
    pkbSP->insertFollowsStar(12, 48);
    pkbSP->insertFollowsStar(15, 48);
    pkbSP->insertFollowsStar(16, 48);
    pkbSP->insertFollowsStar(19, 48);
    pkbSP->insertFollowsStar(20, 48);
    pkbSP->insertFollowsStar(23, 48);
    pkbSP->insertFollowsStar(24, 48);
    pkbSP->insertFollowsStar(27, 48);
    pkbSP->insertFollowsStar(28, 48);
    pkbSP->insertFollowsStar(31, 48);
    pkbSP->insertFollowsStar(32, 48);
    pkbSP->insertFollowsStar(34, 48);
    pkbSP->insertFollowsStar(35, 48);
    pkbSP->insertFollowsStar(36, 48);
    pkbSP->insertFollowsStar(38, 48);
    pkbSP->insertFollowsStar(39, 48);
    pkbSP->insertFollowsStar(40, 48);
    pkbSP->insertFollowsStar(43, 48);
    pkbSP->insertFollowsStar(44, 48);
    pkbSP->insertFollowsStar(45, 48);
    pkbSP->insertNext(47, 48);
    pkbSP->insertNext(46, 48);
    pkbSP->insertNext(47, 48);
    pkbSP->insertModifies(48, "y");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertUses(48, "x");
    pkbSP->insertUses("main", "x");
    pkbSP->insertFollows(48, 49);
    pkbSP->insertFollowsStar(1, 49);
    pkbSP->insertFollowsStar(2, 49);
    pkbSP->insertFollowsStar(3, 49);
    pkbSP->insertFollowsStar(4, 49);
    pkbSP->insertFollowsStar(5, 49);
    pkbSP->insertFollowsStar(6, 49);
    pkbSP->insertFollowsStar(7, 49);
    pkbSP->insertFollowsStar(8, 49);
    pkbSP->insertFollowsStar(9, 49);
    pkbSP->insertFollowsStar(10, 49);
    pkbSP->insertFollowsStar(11, 49);
    pkbSP->insertFollowsStar(12, 49);
    pkbSP->insertFollowsStar(15, 49);
    pkbSP->insertFollowsStar(16, 49);
    pkbSP->insertFollowsStar(19, 49);
    pkbSP->insertFollowsStar(20, 49);
    pkbSP->insertFollowsStar(23, 49);
    pkbSP->insertFollowsStar(24, 49);
    pkbSP->insertFollowsStar(27, 49);
    pkbSP->insertFollowsStar(28, 49);
    pkbSP->insertFollowsStar(31, 49);
    pkbSP->insertFollowsStar(32, 49);
    pkbSP->insertFollowsStar(34, 49);
    pkbSP->insertFollowsStar(35, 49);
    pkbSP->insertFollowsStar(36, 49);
    pkbSP->insertFollowsStar(38, 49);
    pkbSP->insertFollowsStar(39, 49);
    pkbSP->insertFollowsStar(40, 49);
    pkbSP->insertFollowsStar(43, 49);
    pkbSP->insertFollowsStar(44, 49);
    pkbSP->insertFollowsStar(45, 49);
    pkbSP->insertFollowsStar(48, 49);
    pkbSP->insertNext(48, 49);
    pkbSP->insertUses(49, "modifyX");
    pkbSP->insertUses("main", "modifyX");
    pkbSP->insertModifies(50, "x");
    pkbSP->insertModifies("modifyX", "x");
    pkbSP->insertModifies(51, "y");
    pkbSP->insertModifies("doNotModifyX", "y");
    pkbSP->insertUses(51, "x");
    pkbSP->insertUses("doNotModifyX", "x");
    pkbSP->insertUses(23, "x");
    pkbSP->insertUses(9, "x");
    pkbSP->insertModifies(23, "y");
    pkbSP->insertModifies(9, "y");
    pkbSP->insertCallsStar("main", "doNotModifyX");
    pkbSP->insertUses("main", "x");
    pkbSP->insertModifies("main", "y");
    pkbSP->insertModifies(19, "x");
    pkbSP->insertModifies(7, "x");
    pkbSP->insertCallsStar("main", "modifyX");
    pkbSP->insertModifies("main", "x");

    SECTION("One synonym") {
        REQUIRE(testQuery(R"(stmt s; Select s with s.stmt# = 0)", {}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; Select s with s.stmt# = 1)", {"1"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; Select r with r.stmt# = 1)", {}, pkbQPS));
        REQUIRE(testQuery(R"(read r; Select r with r.stmt# = 5)", {"5"}, pkbQPS));
        REQUIRE(testQuery(R"(print p; Select p with p.stmt# = 1)", {}, pkbQPS));
        REQUIRE(testQuery(R"(print p; Select p with p.stmt# = 49)", {"49"}, pkbQPS));
        REQUIRE(testQuery(R"(call c; Select c with c.stmt# = 1)", {}, pkbQPS));
        REQUIRE(testQuery(R"(call c; Select c with c.stmt# = 7)", {"7"}, pkbQPS));
        REQUIRE(testQuery(R"(while w; Select w with w.stmt# = 1)", {}, pkbQPS));
        REQUIRE(testQuery(R"(while w; Select w with w.stmt# = 32)", {"32"}, pkbQPS));
        REQUIRE(testQuery(R"(if i; Select i with i.stmt# = 1)", {}, pkbQPS));
        REQUIRE(testQuery(R"(if i; Select i with i.stmt# = 12)", {"12"}, pkbQPS));
        REQUIRE(testQuery(R"(assign a; Select a with a.stmt# = 12)", {}, pkbQPS));
        REQUIRE(testQuery(R"(assign a; Select a with a.stmt# = 1)", {"1"}, pkbQPS));
        REQUIRE(testQuery(R"(assign a; Select a with a.stmt# = 13)", {"13"}, pkbQPS));
        REQUIRE(testQuery(R"(assign a; Select a with a.stmt# = 33)", {"33"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; Select r with r.stmt# = 47)", {"47"}, pkbQPS));
        REQUIRE(testQuery(R"(procedure p; Select p with p.procName = "modifyX")", {"modifyX"}, pkbQPS));
        REQUIRE(testQuery(R"(call c; Select c with c.procName = "modifyX")", {"7", "19"}, pkbQPS));
        REQUIRE(testQuery(R"(variable v; Select v with v.varName = "x")", {"x"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; Select r with r.varName = "x")", {"5", "15"}, pkbQPS));
        REQUIRE(testQuery(R"(print p; Select p with p.varName = "modifyX")", {"49"}, pkbQPS));
    }

    SECTION("Two synonym") {
        REQUIRE(testQuery(R"(stmt s; read r; Select <s.stmt#, r.stmt#> such that Follows(s, r) with s.stmt# = 4)", {"4 5"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; print p; Select <s.stmt#, p.stmt#> such that Follows(s, p) with s.stmt# = 48)", {"48 49"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; call c; Select <s.stmt#, c.procName> such that Follows(s, c) with s.stmt# = 6)", {"6 modifyX"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; while w; Select <s.stmt#, w.stmt#> such that Next*(s, w) with s.stmt# = 35)", {"35 36"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; if i; Select <s.stmt#, i.stmt#> such that Next*(s, i) with s.stmt# = 44)", {"44 45"}, pkbQPS));
        REQUIRE(testQuery(R"(stmt s; assign a; Select <s.stmt#, a.stmt#> such that Next*(s, a) with s.stmt# = 47)", {"47 48"}, pkbQPS));

        REQUIRE(testQuery(R"(read r; print p; Select <r.varName, p.varName> such that Next*(r, p) with r.varName = "x")", {"x modifyX"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; call c; Select <r.varName, c.procName> such that Next*(r, c) with c.stmt# = 7)", {"x modifyX"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; while w; Select <r.varName, w.stmt#> such that Next*(r, w) with w.stmt# = 32)", {"x 32", "c 32"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; if i; Select <r.varName, i.stmt#> such that Next*(r, i) with i.stmt# = 12)", {"x 12"}, pkbQPS));
        REQUIRE(testQuery(R"(read r; assign a; Select <r.varName, a.stmt#> such that Next*(r, a) with a.stmt# = 6)", {"x 6"}, pkbQPS));

        REQUIRE(testQuery(R"(print p; call c; Select <p.varName, c.procName> such that Next*(p, c) with c.stmt# = 7)", {}, pkbQPS));
        REQUIRE(testQuery(R"(print p; while w; Select <p.varName, w.stmt#> such that Next*(p, w) with w.stmt# = 36)", {}, pkbQPS));
        REQUIRE(testQuery(R"(print p; if i; Select <p.varName, i.stmt#> such that Next*(p, i) with i.stmt# = 12)", {}, pkbQPS));
        REQUIRE(testQuery(R"(print p; assign a; Select <p.varName, a.stmt#> such that Next*(p, a) with a.stmt# = 6)", {}, pkbQPS));

        REQUIRE(testQuery(R"(call c; while w; Select <c.procName, w.stmt#> such that Next*(c, w) with w.stmt# = 32)", {"modifyX 32", "doNotModifyX 32"}, pkbQPS));
        REQUIRE(testQuery(R"(call c; if i; Select <c.procName, i.stmt#> such that Next*(c, i) with i.stmt# = 12)", {"modifyX 12", "doNotModifyX 12"}, pkbQPS));
        REQUIRE(testQuery(R"(call c; assign a; Select <c.procName, a.stmt#> such that Next*(c, a) with a.stmt# = 10)", {"modifyX 10", "doNotModifyX 10"}, pkbQPS));

        REQUIRE(testQuery(R"(while w; if i; Select <w.stmt#, i.stmt#> such that Next*(w, i) with w.stmt# = 32)", {"32 40", "32 45"}, pkbQPS));
        REQUIRE(testQuery(R"(while w; assign a; Select <w.stmt#, a.stmt#> such that Next*(w, a) with w.stmt# = 32)", {"32 33", "32 34", "32 35", "32 37", "32 38", "32 39", "32 41", "32 43", "32 44", "32 46", "32 48"}, pkbQPS));

        REQUIRE(testQuery(R"(if i; assign a; Select <a.stmt#, i.stmt#> such that Next*(a, i) with i.stmt# = 12)", {"1 12", "2 12", "3 12", "4 12", "6 12", "8 12", "10 12", "11 12"}, pkbQPS));
    }

    SECTION("Three synonyms") {
        REQUIRE(testQuery(R"(stmt s; read r; print p; Select <s.stmt#, r.varName, p.varName> such that Next*(s, r) and Next*(s, p) with s.stmt# = 45)", {"45 c modifyX"}, pkbQPS));
    }
}