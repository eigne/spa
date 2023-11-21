#include "catch.hpp"
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementSynonym.h"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query_evaluator/storage_reader/INextReader.h"
#include "query_processing_system/query/design_abstractions/such_that/next/Next.h"

/*
* 1
* | \
* 2  4
* |  |
* 3  |
* |  /
*  5
*  |
*  6 <--
*  |    |
*  7 ---
*/
class NextReaderStub : public INextReader {
public:
    NextReaderStub() : INextReader() {}

    std::unordered_set<int> getNext(int statementNumber) override {
        if (statementNumber == 1) {
            return { 2, 4 };
        } else if (statementNumber == 2) {
            return { 3 };
        } else if (statementNumber == 3) {
            return { 5 };
        } else if (statementNumber == 4) {           
            return { 5 };
        } else if (statementNumber == 5) {
            return { 6 };
        } else if (statementNumber == 6) {
            return { 7 };
        } else if (statementNumber == 7) {
            return { 6 };
        }
        return {};
    }

    std::unordered_set<int> getPrevious(int statementNumber) override {
        if (statementNumber == 1) {
            return { };
        } else if (statementNumber == 2) {
            return { 1 };
        } else if (statementNumber == 3) {
            return { 2 };
        } else if (statementNumber == 4) {
                return { 1 };
        } else if (statementNumber == 5) {
            return { 3, 4 };            
        } else if (statementNumber == 6) {
            return { 5, 7 };            
        } else if (statementNumber == 7) {
            return { 6 };            
        }
        return {};
    }
};

class NoNextStub : public INextReader {
    std::unordered_set<int> getNext(int statementNumber) override {
        return {};
    }

    std::unordered_set<int> getPrevious(int statementNumber) override {
        return {};
    }
};

TEST_CASE("Visit Next Two Synonyms") {
    std::shared_ptr<INextReader> nextReader = std::make_shared<NextReaderStub>();

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");

    QuerySelection selection({s1, s2});
    std::list<std::string> ans;

    s1->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
    s2->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
    std::shared_ptr<Next> next1 = std::make_shared<Next>(s1, s2, false, false);
    REQUIRE(next1->evaluateNext(nextReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({ 2, 3, 4, 5, 6, 7 }));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 2", "1 4", "2 3", "3 5", "4 5", "5 6", "6 7", "7 6"}));

    s1->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
    s2->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
    std::shared_ptr<Next> next2 = std::make_shared<Next>(s1, s2, false, true);
    REQUIRE(next2->evaluateNext(nextReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 1", "1 3", "1 5", "1 6", "1 7",
                                           "2 1", "2 2", "2 4", "2 5", "2 6", "2 7",
                                           "3 1", "3 2", "3 3", "3 4", "3 6", "3 7",
                                           "4 1", "4 2", "4 3", "4 4", "4 6", "4 7",
                                           "5 1", "5 2", "5 3", "5 4", "5 5", "5 7",
                                           "6 1", "6 2", "6 3", "6 4", "6 5", "6 6",
                                           "7 1", "7 2", "7 3", "7 4", "7 5", "7 7"}));
}

TEST_CASE("Visit NextStar Two Synonyms") {
    std::shared_ptr<INextReader> nextReader = std::make_shared<NextReaderStub>();

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");

    QuerySelection selection({s1, s2});
    std::list<std::string> ans;

    s1->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
    s2->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
    std::shared_ptr<Next> next1 = std::make_shared<Next>(s1, s2, true, false);
    REQUIRE(next1->evaluateNext(nextReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({ 2, 3, 4, 5, 6, 7 }));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 2", "1 3", "1 4", "1 5", "1 6", "1 7",
                                           "2 3", "2 5", "2 6", "2 7",
                                           "3 5", "3 6", "3 7",
                                           "4 5", "4 6", "4 7",
                                           "5 6", "5 7",
                                           "6 6", "6 7",
                                           "7 6", "7 7"}));

    s1->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
    s2->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
    std::shared_ptr<Next> next2 = std::make_shared<Next>(s1, s2, true, true);
    REQUIRE(next2->evaluateNext(nextReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5 }));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 1",
                                           "2 1", "2 2", "2 4",
                                           "3 1", "3 2", "3 3", "3 4",
                                           "4 1", "4 2", "4 3", "4 4",
                                           "5 1", "5 2", "5 3", "5 4", "5 5",
                                           "6 1", "6 2", "6 3", "6 4", "6 5",
                                           "7 1", "7 2", "7 3", "7 4", "7 5"}));
}

TEST_CASE("Visit Next One Synonym") {
    std::shared_ptr<INextReader> nextReader = std::make_shared<NextReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4, 5, 6, 7};

    std::shared_ptr<StatementTypeSynonym> s = std::make_shared<StatementSynonym>("s");
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementLineNumber> s_5 = std::make_shared<StatementLineNumber>(5);
    std::shared_ptr<StatementLineNumber> s_6 = std::make_shared<StatementLineNumber>(6);
    std::shared_ptr<StatementLineNumber> s_7 = std::make_shared<StatementLineNumber>(7);
    std::shared_ptr<StatementWildcard> s_w = std::make_shared<StatementWildcard>();

    SECTION("Line 1") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_1, false, false);
        REQUIRE_FALSE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers().empty());

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_1, false, true);
        REQUIRE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_1, s, false, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 2, 4 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_1, s, false, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 3, 5, 6, 7 }));
    }

    SECTION("Line 4") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_4, false, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_4, false, true);
        REQUIRE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_4, s, false, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 5 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_4, s, false, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 6, 7 }));
    }

    SECTION("Line 5") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_5, false, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 3, 4 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });        
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_5, false, true);        
        REQUIRE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_5, s, false, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 6 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_5, s, false, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 7 }));
    }

    SECTION("Line 6") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_6, false, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 5, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_6, false, true);
        REQUIRE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 6 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_6, s, false, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_6, s, false, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6 }));
    }

    SECTION("Line 7") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_7, false, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 6 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_7, false, true);
        REQUIRE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_7, s, false, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 6 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_7, s, false, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 7 }));
    }

    SECTION("Wildcard") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_w, false, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_w, false, true);
        REQUIRE_FALSE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers().empty());

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_w, s, false, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_w, s, false, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1 }));
    }
}

TEST_CASE("Visit NextStar One Synonym") {
    std::shared_ptr<INextReader> nextReader = std::make_shared<NextReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4, 5, 6, 7};

    std::shared_ptr<StatementTypeSynonym> s = std::make_shared<StatementSynonym>("s");
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementLineNumber> s_5 = std::make_shared<StatementLineNumber>(5);
    std::shared_ptr<StatementLineNumber> s_6 = std::make_shared<StatementLineNumber>(6);
    std::shared_ptr<StatementLineNumber> s_7 = std::make_shared<StatementLineNumber>(7);
    std::shared_ptr<StatementWildcard> s_w = std::make_shared<StatementWildcard>();

    SECTION("Line 1") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_1, true, false);
        REQUIRE_FALSE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers().empty());

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_1, true, true);
        REQUIRE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_1, s, true, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_1, s, true, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1 }));
    }

    SECTION("Line 4") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_4, true, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_4, true, true);
        REQUIRE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_4, s, true, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_4, s, true, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4 }));
    }

    SECTION("Line 5") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_5, true, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_5, true, true);
        REQUIRE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_5, s, true, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_5, s, true, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5 }));
    }

    SECTION("Line 6") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_6, true, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_6, true, true);
        REQUIRE_FALSE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers().empty());

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_6, s, true, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_6, s, true, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5 }));
    }

    SECTION("Line 7") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_7, true, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_7, true, true);
        REQUIRE_FALSE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers().empty());

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_7, s, true, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_7, s, true, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5 }));
    }

    SECTION("Wildcard") {
        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s, s_w, true, false);
        REQUIRE(next1->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next2 = std::make_shared<Next>(s, s_w, true, true);
        REQUIRE_FALSE(next2->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers().empty());

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next3 = std::make_shared<Next>(s_w, s, true, false);
        REQUIRE(next3->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 2, 3, 4, 5, 6, 7 }));

        s->setStatementNumbers({ 1, 2, 3, 4, 5, 6, 7 });
        std::shared_ptr<Next> next4 = std::make_shared<Next>(s_w, s, true, true);
        REQUIRE(next4->evaluateNext(nextReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({ 1 }));
    }
}

TEST_CASE("Visit Next No Synonym") {
    std::shared_ptr<INextReader> nextReader = std::make_shared<NextReaderStub>();
    StatementWildcard::allPossibleStatements = { 1, 2, 3, 4, 5, 6, 7 };

    std::shared_ptr<StatementWildcard> s_w1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> s_w2 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementLineNumber> s_5 = std::make_shared<StatementLineNumber>(5);
    std::shared_ptr<StatementLineNumber> s_6 = std::make_shared<StatementLineNumber>(6);
    std::shared_ptr<StatementLineNumber> s_7 = std::make_shared<StatementLineNumber>(7);

    SECTION("Two Wildcards") {
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s_w1, s_w2, false, false);
        REQUIRE(next1->evaluateNext(nextReader));
    }

    SECTION("One Wildcard") {
        std::shared_ptr<Next> next_w1 = std::make_shared<Next>(s_w1, s_1, false, false);
        REQUIRE_FALSE(next_w1->evaluateNext(nextReader));

        std::shared_ptr<Next> next_1w = std::make_shared<Next>(s_1, s_w1, false, false);
        REQUIRE(next_1w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w2 = std::make_shared<Next>(s_w1, s_2, false, false);
        REQUIRE(next_w2->evaluateNext(nextReader));

        std::shared_ptr<Next> next_2w = std::make_shared<Next>(s_2, s_w1, false, false);
        REQUIRE(next_2w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w3 = std::make_shared<Next>(s_w1, s_3, false, false);
        REQUIRE(next_w3->evaluateNext(nextReader));

        std::shared_ptr<Next> next_notw3 = std::make_shared<Next>(s_w1, s_3, false, true);
        REQUIRE(next_w3->evaluateNext(nextReader));

        std::shared_ptr<Next> next_3w = std::make_shared<Next>(s_3, s_w1, false, false);
        REQUIRE(next_3w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w4 = std::make_shared<Next>(s_w1, s_4, false, false);
        REQUIRE(next_w4->evaluateNext(nextReader));

        std::shared_ptr<Next> next_4w = std::make_shared<Next>(s_4, s_w1, false, false);
        REQUIRE(next_4w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w5 = std::make_shared<Next>(s_w1, s_5, false, false);
        REQUIRE(next_w5->evaluateNext(nextReader));

        std::shared_ptr<Next> next_notw5 = std::make_shared<Next>(s_w1, s_5, false, true);
        REQUIRE(next_w5->evaluateNext(nextReader));

        std::shared_ptr<Next> next_5w = std::make_shared<Next>(s_5, s_w1, false, false);
        REQUIRE(next_5w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w6 = std::make_shared<Next>(s_w1, s_6, false, false);
        REQUIRE(next_w6->evaluateNext(nextReader));

        std::shared_ptr<Next> next_6w = std::make_shared<Next>(s_6, s_w1, false, false);
        REQUIRE(next_6w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_not6w = std::make_shared<Next>(s_6, s_w1, false, true);
        REQUIRE(next_6w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w7 = std::make_shared<Next>(s_w1, s_7, false, false);
        REQUIRE(next_w7->evaluateNext(nextReader));

        std::shared_ptr<Next> next_7w = std::make_shared<Next>(s_7, s_w1, false, false);
        REQUIRE(next_7w->evaluateNext(nextReader));
    }

    SECTION("No Wildcard") {
        std::shared_ptr<Next> next_12 = std::make_shared<Next>(s_1, s_2, false, false);
        REQUIRE(next_12->evaluateNext(nextReader));

        std::shared_ptr<Next> next_14 = std::make_shared<Next>(s_1, s_4, false, false);
        REQUIRE(next_14->evaluateNext(nextReader));

        std::shared_ptr<Next> next_35 = std::make_shared<Next>(s_3, s_5, false, false);
        REQUIRE(next_35->evaluateNext(nextReader));

        std::shared_ptr<Next> next_76 = std::make_shared<Next>(s_7, s_6, false, false);
        REQUIRE(next_76->evaluateNext(nextReader));
    }
}

TEST_CASE("Visit Next Star No Synonym") {
    std::shared_ptr<INextReader> nextReader = std::make_shared<NextReaderStub>();
    StatementWildcard::allPossibleStatements = { 1, 2, 3, 4, 5, 6, 7 };

    std::shared_ptr<StatementWildcard> s_w1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> s_w2 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementLineNumber> s_5 = std::make_shared<StatementLineNumber>(5);
    std::shared_ptr<StatementLineNumber> s_6 = std::make_shared<StatementLineNumber>(6);
    std::shared_ptr<StatementLineNumber> s_7 = std::make_shared<StatementLineNumber>(7);

    SECTION("Two Wildcards") {
        std::shared_ptr<Next> next1 = std::make_shared<Next>(s_w1, s_w2, true, false);
        REQUIRE(next1->evaluateNext(nextReader));
    }

    SECTION("One Wildcard") {
        std::shared_ptr<Next> next_w1 = std::make_shared<Next>(s_w1, s_1, true, false);
        REQUIRE_FALSE(next_w1->evaluateNext(nextReader));

        std::shared_ptr<Next> next_1w = std::make_shared<Next>(s_1, s_w1, true, false);
        REQUIRE(next_1w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w2 = std::make_shared<Next>(s_w1, s_2, true, false);
        REQUIRE(next_w2->evaluateNext(nextReader));

        std::shared_ptr<Next> next_2w = std::make_shared<Next>(s_2, s_w1, true, false);
        REQUIRE(next_2w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w3 = std::make_shared<Next>(s_w1, s_3, true, false);
        REQUIRE(next_w3->evaluateNext(nextReader));

        std::shared_ptr<Next> next_notw3 = std::make_shared<Next>(s_w1, s_3, true, true);
        REQUIRE(next_w3->evaluateNext(nextReader));

        std::shared_ptr<Next> next_3w = std::make_shared<Next>(s_3, s_w1, true, false);
        REQUIRE(next_3w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w4 = std::make_shared<Next>(s_w1, s_4, true, false);
        REQUIRE(next_w4->evaluateNext(nextReader));

        std::shared_ptr<Next> next_4w = std::make_shared<Next>(s_4, s_w1, true, false);
        REQUIRE(next_4w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w5 = std::make_shared<Next>(s_w1, s_5, true, false);
        REQUIRE(next_w5->evaluateNext(nextReader));

        std::shared_ptr<Next> next_notw5 = std::make_shared<Next>(s_w1, s_5, true, true);
        REQUIRE(next_w5->evaluateNext(nextReader));

        std::shared_ptr<Next> next_5w = std::make_shared<Next>(s_5, s_w1, true, false);
        REQUIRE(next_5w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w6 = std::make_shared<Next>(s_w1, s_6, true, false);
        REQUIRE(next_w6->evaluateNext(nextReader));

        std::shared_ptr<Next> next_6w = std::make_shared<Next>(s_6, s_w1, true, false);
        REQUIRE(next_6w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_not6w = std::make_shared<Next>(s_6, s_w1, true, true);
        REQUIRE(next_6w->evaluateNext(nextReader));

        std::shared_ptr<Next> next_w7 = std::make_shared<Next>(s_w1, s_7, true, false);
        REQUIRE(next_w7->evaluateNext(nextReader));

        std::shared_ptr<Next> next_7w = std::make_shared<Next>(s_7, s_w1, true, false);
        REQUIRE(next_7w->evaluateNext(nextReader));
    }

    SECTION("No Wildcard") {
        std::shared_ptr<Next> next_15 = std::make_shared<Next>(s_1, s_5, true, false);
        REQUIRE(next_15->evaluateNext(nextReader));

        std::shared_ptr<Next> next_13 = std::make_shared<Next>(s_1, s_3, true, false);
        REQUIRE(next_13->evaluateNext(nextReader));

        std::shared_ptr<Next> next_26 = std::make_shared<Next>(s_2, s_6, true, false);
        REQUIRE(next_26->evaluateNext(nextReader));

        std::shared_ptr<Next> notNext_77 = std::make_shared<Next>(s_7, s_7, true, true);
        REQUIRE_FALSE(notNext_77->evaluateNext(nextReader));
    }
}

TEST_CASE("No Next") {
    std::shared_ptr<INextReader> nextReader = std::make_shared<NoNextStub>();
    StatementWildcard::allPossibleStatements = { 1 };
    std::shared_ptr<StatementWildcard> s1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> s2 = std::make_shared<StatementWildcard>();

    std::shared_ptr<Next> next1 = std::make_shared<Next>(s1, s2, false, false);
    REQUIRE_FALSE(next1->evaluateNext(nextReader));

    std::shared_ptr<Next> notNext1 = std::make_shared<Next>(s1, s2, false, true);
    REQUIRE(notNext1->evaluateNext(nextReader));

    std::shared_ptr<Next> next2 = std::make_shared<Next>(s1, s2, true, false);
    REQUIRE_FALSE(next2->evaluateNext(nextReader));

    std::shared_ptr<Next> notNext2 = std::make_shared<Next>(s1, s2, true, true);
    REQUIRE(notNext2->evaluateNext(nextReader));
}
