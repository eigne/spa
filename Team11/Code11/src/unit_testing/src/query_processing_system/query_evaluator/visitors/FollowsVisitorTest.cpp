#include "catch.hpp"
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementSynonym.h"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query_evaluator/storage_reader/IFollowsReader.h"
#include "query_processing_system/query/design_abstractions/such_that/follows/Follows.h"

// 1{2}, 3, 4
class FollowsReaderStub : public IFollowsReader {
    std::unordered_set<int> getFollows(int statementNumber, bool onlyImmediate) override {
        if (statementNumber == 3) {
            return {4};
        } else if (statementNumber == 1) {
            if (onlyImmediate) {
                return {3};
            } else {
                return {3, 4};
            }
        } else {
            return {};
        }
    }

    std::unordered_set<int> getBefore(int statementNumber, bool onlyImmediate) override {
        if (statementNumber == 3) {
            return {1};
        } else if (statementNumber == 4) {
            if (onlyImmediate) {
                return {3};
            } else {
                return {1, 3};
            }
        } else {
            return {};
        }
    }

    std::unordered_set<int> getAllWithFollows() override {
        return {1, 3};
    }

    std::unordered_set<int> getAllWithBefore() override {
        return {3, 4};
    }
};

// e.g. one-liner programs
class NoFollowsStub : public IFollowsReader {
    std::unordered_set<int> getFollows(int statementNumber, bool onlyImmediate) override {
        return {};
    }

    std::unordered_set<int> getBefore(int statementNumber, bool onlyImmediate) override {
        return {};
    }

    std::unordered_set<int> getAllWithFollows() override {
        return {};
    }

    std::unordered_set<int> getAllWithBefore() override {
        return {};
    }
};

TEST_CASE("Visit Follows Same Synonym") {
    std::shared_ptr<IFollowsReader> followsReader = std::make_shared<FollowsReaderStub>();

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");

    s1->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s1, false, false);
    REQUIRE_FALSE(follows1->evaluateFollows(followsReader));

    s1->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s1, false, true);
    REQUIRE(follows2->evaluateFollows(followsReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
}

TEST_CASE("Visit Follows Two Synonyms") {
    std::shared_ptr<IFollowsReader> followsReader = std::make_shared<FollowsReaderStub>();

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");

    QuerySelection selection({s1, s2});
    std::list<std::string> ans;

    s1->setStatementNumbers({1, 2, 3, 4});
    s2->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s2, false, false);
    REQUIRE(follows1->evaluateFollows(followsReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({3, 4}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 3", "3 4"}));

    s1->setStatementNumbers({1, 2, 3, 4});
    s2->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s2, false, true);
    REQUIRE(follows2->evaluateFollows(followsReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 1", "1 2", "1 4",
                                           "2 1", "2 2", "2 3", "2 4",
                                           "3 1", "3 2", "3 3",
                                           "4 1", "4 2", "4 3", "4 4"}));
}

TEST_CASE("Visit FollowsStar Two Synonyms") {
    std::shared_ptr<IFollowsReader> followsReader = std::make_shared<FollowsReaderStub>();

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");

    QuerySelection selection({s1, s2});
    std::list<std::string> ans;

    s1->setStatementNumbers({1, 2, 3, 4});
    s2->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s2, true, false);
    REQUIRE(follows1->evaluateFollows(followsReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({3, 4}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 3", "1 4", "3 4"}));

    s1->setStatementNumbers({1, 2, 3, 4});
    s2->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s2, true, true);
    REQUIRE(follows2->evaluateFollows(followsReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 1", "1 2",
                                           "2 1", "2 2", "2 3", "2 4",
                                           "3 1", "3 2", "3 3",
                                           "4 1", "4 2", "4 3", "4 4"}));
}

TEST_CASE("Visit Follows One Synonym") {
    std::shared_ptr<IFollowsReader> followsReader = std::make_shared<FollowsReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4};

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementWildcard> s = std::make_shared<StatementWildcard>();

    SECTION("Line 1") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s_1, false, false);
        REQUIRE_FALSE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s_1, false, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s_1, s1, false, false);
        REQUIRE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s_1, s1, false, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 4}));
    }

    SECTION("Line 2") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s_2, false, false);
        REQUIRE_FALSE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s_2, false, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s_2, s1, false, false);
        REQUIRE_FALSE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s_2, s1, false, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    }

    SECTION("Line 3") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s_3, false, false);
        REQUIRE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s_3, false, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s_3, s1, false, false);
        REQUIRE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s_3, s1, false, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    }

    SECTION("Line 4") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s_4, false, false);
        REQUIRE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s_4, false, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s_4, s1, false, false);
        REQUIRE_FALSE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s_4, s1, false, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    }

    SECTION("Wildcard") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s, false, false);
        REQUIRE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s, false, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s, s1, false, false);
        REQUIRE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s, s1, false, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2}));
    }
}

TEST_CASE("Visit FollowsStar One Synonym") {
    std::shared_ptr<IFollowsReader> followsReader = std::make_shared<FollowsReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4};

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementWildcard> s = std::make_shared<StatementWildcard>();

    SECTION("Line 1") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s_1, true, false);
        REQUIRE_FALSE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s_1, true, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s_1, s1, true, false);
        REQUIRE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s_1, s1, true, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2}));
    }

    SECTION("Line 2") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s_2, true, false);
        REQUIRE_FALSE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s_2, true, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s_2, s1, true, false);
        REQUIRE_FALSE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s_2, s1, true, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    }

    SECTION("Line 3") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s_3, true, false);
        REQUIRE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s_3, true, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s_3, s1, true, false);
        REQUIRE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s_3, s1, true, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    }

    SECTION("Line 4") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s_4, true, false);
        REQUIRE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s_4, true, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s_4, s1, true, false);
        REQUIRE_FALSE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s_4, s1, true, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    }

    SECTION("Wildcard") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s, true, false);
        REQUIRE(follows1->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s, true, true);
        REQUIRE(follows2->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s, s1, true, false);
        REQUIRE(follows3->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s, s1, true, true);
        REQUIRE(follows4->evaluateFollows(followsReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2}));
    }
}

TEST_CASE("Visit Follows No Synonym") {
    std::shared_ptr<IFollowsReader> followsReader = std::make_shared<FollowsReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4};

    std::shared_ptr<StatementWildcard> s_w1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> s_w2 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);

    SECTION("Two Wildcards") {
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s_w1, s_w2, false, false);
        REQUIRE(follows1->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s_w1, s_w2, false, true);
        REQUIRE_FALSE(follows2->evaluateFollows(followsReader));
    }

    SECTION("One Wildcard") {
        std::shared_ptr<Follows> follows_w1 = std::make_shared<Follows>(s_w1, s_1, false, false);
        REQUIRE_FALSE(follows_w1->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_w1n = std::make_shared<Follows>(s_w1, s_1, false, true);
        REQUIRE(follows_w1n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_1w = std::make_shared<Follows>(s_1, s_w1, false, false);
        REQUIRE(follows_1w->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_1wn = std::make_shared<Follows>(s_1, s_w1, false, true);
        REQUIRE_FALSE(follows_1wn->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_w2 = std::make_shared<Follows>(s_w1, s_2, false, false);
        REQUIRE_FALSE(follows_w2->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_w2n = std::make_shared<Follows>(s_w1, s_2, false, true);
        REQUIRE(follows_w2n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_2w = std::make_shared<Follows>(s_2, s_w1, false, false);
        REQUIRE_FALSE(follows_2w->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_2wn = std::make_shared<Follows>(s_2, s_w1, false, true);
        REQUIRE(follows_2wn->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_w3 = std::make_shared<Follows>(s_w1, s_3, false, false);
        REQUIRE(follows_w3->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_w3n = std::make_shared<Follows>(s_w1, s_3, false, true);
        REQUIRE_FALSE(follows_w3n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_3w = std::make_shared<Follows>(s_3, s_w1, false, false);
        REQUIRE(follows_3w->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_3wn = std::make_shared<Follows>(s_3, s_w1, false, true);
        REQUIRE_FALSE(follows_3wn->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_w4 = std::make_shared<Follows>(s_w1, s_4, false, false);
        REQUIRE(follows_w4->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_w4n = std::make_shared<Follows>(s_w1, s_4, false, true);
        REQUIRE_FALSE(follows_w4n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_4w = std::make_shared<Follows>(s_4, s_w1, false, false);
        REQUIRE_FALSE(follows_4w->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_4wn = std::make_shared<Follows>(s_4, s_w1, false, true);
        REQUIRE(follows_4wn->evaluateFollows(followsReader));
    }

    SECTION("No Wildcard") {
        std::shared_ptr<Follows> follows_11 = std::make_shared<Follows>(s_1, s_1, false, false);
        REQUIRE_FALSE(follows_11->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_11n = std::make_shared<Follows>(s_1, s_1, false, true);
        REQUIRE(follows_11n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_12 = std::make_shared<Follows>(s_1, s_2, false, false);
        REQUIRE_FALSE(follows_12->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_12n = std::make_shared<Follows>(s_1, s_2, false, true);
        REQUIRE(follows_12n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_13 = std::make_shared<Follows>(s_1, s_3, false, false);
        REQUIRE(follows_13->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_13n = std::make_shared<Follows>(s_1, s_3, false, true);
        REQUIRE_FALSE(follows_13n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_14 = std::make_shared<Follows>(s_1, s_4, false, false);
        REQUIRE_FALSE(follows_14->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_14n = std::make_shared<Follows>(s_1, s_4, false, true);
        REQUIRE(follows_14n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_21 = std::make_shared<Follows>(s_2, s_1, false, false);
        REQUIRE_FALSE(follows_21->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_21n = std::make_shared<Follows>(s_2, s_1, false, true);
        REQUIRE(follows_21n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_22 = std::make_shared<Follows>(s_2, s_2, false, false);
        REQUIRE_FALSE(follows_22->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_22n = std::make_shared<Follows>(s_2, s_2, false, true);
        REQUIRE(follows_22n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_23 = std::make_shared<Follows>(s_2, s_3, false, false);
        REQUIRE_FALSE(follows_23->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_23n = std::make_shared<Follows>(s_2, s_3, false, true);
        REQUIRE(follows_23n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_24 = std::make_shared<Follows>(s_2, s_4, false, false);
        REQUIRE_FALSE(follows_24->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_24n = std::make_shared<Follows>(s_2, s_4, false, true);
        REQUIRE(follows_24n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_31 = std::make_shared<Follows>(s_3, s_1, false, false);
        REQUIRE_FALSE(follows_31->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_31n = std::make_shared<Follows>(s_3, s_1, false, true);
        REQUIRE(follows_31n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_32 = std::make_shared<Follows>(s_3, s_2, false, false);
        REQUIRE_FALSE(follows_32->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_32n = std::make_shared<Follows>(s_3, s_2, false, true);
        REQUIRE(follows_32n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_33 = std::make_shared<Follows>(s_3, s_3, false, false);
        REQUIRE_FALSE(follows_33->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_33n = std::make_shared<Follows>(s_3, s_3, false, true);
        REQUIRE(follows_33n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_34 = std::make_shared<Follows>(s_3, s_4, false, false);
        REQUIRE(follows_34->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_34n = std::make_shared<Follows>(s_3, s_4, false, true);
        REQUIRE_FALSE(follows_34n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_41 = std::make_shared<Follows>(s_4, s_1, false, false);
        REQUIRE_FALSE(follows_41->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_41n = std::make_shared<Follows>(s_4, s_1, false, true);
        REQUIRE(follows_41n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_42 = std::make_shared<Follows>(s_4, s_2, false, false);
        REQUIRE_FALSE(follows_42->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_42n = std::make_shared<Follows>(s_4, s_2, false, true);
        REQUIRE(follows_42n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_43 = std::make_shared<Follows>(s_4, s_3, false, false);
        REQUIRE_FALSE(follows_43->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_43n = std::make_shared<Follows>(s_4, s_3, false, true);
        REQUIRE(follows_43n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_44 = std::make_shared<Follows>(s_4, s_4, false, false);
        REQUIRE_FALSE(follows_44->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_44n = std::make_shared<Follows>(s_4, s_4, false, true);
        REQUIRE(follows_44n->evaluateFollows(followsReader));
    }
}

TEST_CASE("Visit FollowsStar No Synonym") {
    std::shared_ptr<IFollowsReader> followsReader = std::make_shared<FollowsReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4};

    std::shared_ptr<StatementWildcard> s_w1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> s_w2 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);

    SECTION("Two Wildcards") {
        std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s_w1, s_w2, true, false);
        REQUIRE(follows1->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s_w1, s_w2, true, true);
        REQUIRE_FALSE(follows2->evaluateFollows(followsReader));
    }

    SECTION("One Wildcard") {
        std::shared_ptr<Follows> follows_w1 = std::make_shared<Follows>(s_w1, s_1, true, false);
        REQUIRE_FALSE(follows_w1->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_w1n = std::make_shared<Follows>(s_w1, s_1, true, true);
        REQUIRE(follows_w1n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_1w = std::make_shared<Follows>(s_1, s_w1, true, false);
        REQUIRE(follows_1w->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_1wn = std::make_shared<Follows>(s_1, s_w1, true, true);
        REQUIRE_FALSE(follows_1wn->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_w2 = std::make_shared<Follows>(s_w1, s_2, true, false);
        REQUIRE_FALSE(follows_w2->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_w2n = std::make_shared<Follows>(s_w1, s_2, true, true);
        REQUIRE(follows_w2n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_2w = std::make_shared<Follows>(s_2, s_w1, true, false);
        REQUIRE_FALSE(follows_2w->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_2wn = std::make_shared<Follows>(s_2, s_w1, true, true);
        REQUIRE(follows_2wn->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_w3 = std::make_shared<Follows>(s_w1, s_3, true, false);
        REQUIRE(follows_w3->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_w3n = std::make_shared<Follows>(s_w1, s_3, true, true);
        REQUIRE_FALSE(follows_w3n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_3w = std::make_shared<Follows>(s_3, s_w1, true, false);
        REQUIRE(follows_3w->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_3wn = std::make_shared<Follows>(s_3, s_w1, true, true);
        REQUIRE_FALSE(follows_3wn->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_w4 = std::make_shared<Follows>(s_w1, s_4, true, false);
        REQUIRE(follows_w4->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_w4n = std::make_shared<Follows>(s_w1, s_4, true, true);
        REQUIRE_FALSE(follows_w4n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_4w = std::make_shared<Follows>(s_4, s_w1, true, false);
        REQUIRE_FALSE(follows_4w->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_4wn = std::make_shared<Follows>(s_4, s_w1, true, true);
        REQUIRE(follows_4wn->evaluateFollows(followsReader));
    }

    SECTION("No Wildcard") {
        std::shared_ptr<Follows> follows_11 = std::make_shared<Follows>(s_1, s_1, true, false);
        REQUIRE_FALSE(follows_11->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_11n = std::make_shared<Follows>(s_1, s_1, true, true);
        REQUIRE(follows_11n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_12 = std::make_shared<Follows>(s_1, s_2, true, false);
        REQUIRE_FALSE(follows_12->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_12n = std::make_shared<Follows>(s_1, s_2, true, true);
        REQUIRE(follows_12n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_13 = std::make_shared<Follows>(s_1, s_3, true, false);
        REQUIRE(follows_13->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_13n = std::make_shared<Follows>(s_1, s_3, true, true);
        REQUIRE_FALSE(follows_13n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_14 = std::make_shared<Follows>(s_1, s_4, true, false);
        REQUIRE(follows_14->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_14n = std::make_shared<Follows>(s_1, s_4, true, true);
        REQUIRE_FALSE(follows_14n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_21 = std::make_shared<Follows>(s_2, s_1, true, false);
        REQUIRE_FALSE(follows_21->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_21n = std::make_shared<Follows>(s_2, s_1, true, true);
        REQUIRE(follows_21n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_22 = std::make_shared<Follows>(s_2, s_2, true, false);
        REQUIRE_FALSE(follows_22->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_22n = std::make_shared<Follows>(s_2, s_2, true, true);
        REQUIRE(follows_22n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_23 = std::make_shared<Follows>(s_2, s_3, true, false);
        REQUIRE_FALSE(follows_23->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_23n = std::make_shared<Follows>(s_2, s_3, true, true);
        REQUIRE(follows_23n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_24 = std::make_shared<Follows>(s_2, s_4, true, false);
        REQUIRE_FALSE(follows_24->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_24n = std::make_shared<Follows>(s_2, s_4, true, true);
        REQUIRE(follows_24n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_31 = std::make_shared<Follows>(s_3, s_1, true, false);
        REQUIRE_FALSE(follows_31->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_31n = std::make_shared<Follows>(s_3, s_1, true, true);
        REQUIRE(follows_31n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_32 = std::make_shared<Follows>(s_3, s_2, true, false);
        REQUIRE_FALSE(follows_32->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_32n = std::make_shared<Follows>(s_3, s_2, true, true);
        REQUIRE(follows_32n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_33 = std::make_shared<Follows>(s_3, s_3, true, false);
        REQUIRE_FALSE(follows_33->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_33n = std::make_shared<Follows>(s_3, s_3, true, true);
        REQUIRE(follows_33n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_34 = std::make_shared<Follows>(s_3, s_4, true, false);
        REQUIRE(follows_34->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_34n = std::make_shared<Follows>(s_3, s_4, true, true);
        REQUIRE_FALSE(follows_34n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_41 = std::make_shared<Follows>(s_4, s_1, true, false);
        REQUIRE_FALSE(follows_41->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_41n = std::make_shared<Follows>(s_4, s_1, true, true);
        REQUIRE(follows_41n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_42 = std::make_shared<Follows>(s_4, s_2, true, false);
        REQUIRE_FALSE(follows_42->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_42n = std::make_shared<Follows>(s_4, s_2, true, true);
        REQUIRE(follows_42n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_43 = std::make_shared<Follows>(s_4, s_3, true, false);
        REQUIRE_FALSE(follows_43->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_43n = std::make_shared<Follows>(s_4, s_3, true, true);
        REQUIRE(follows_43n->evaluateFollows(followsReader));

        std::shared_ptr<Follows> follows_44 = std::make_shared<Follows>(s_4, s_4, true, false);
        REQUIRE_FALSE(follows_44->evaluateFollows(followsReader));
        std::shared_ptr<Follows> follows_44n = std::make_shared<Follows>(s_4, s_4, true, true);
        REQUIRE(follows_44n->evaluateFollows(followsReader));
    }
}

TEST_CASE("No Follows") {
    std::shared_ptr<IFollowsReader> followsReader = std::make_shared<NoFollowsStub>();
    StatementWildcard::allPossibleStatements = {1};
    std::shared_ptr<StatementWildcard> s1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> s2 = std::make_shared<StatementWildcard>();

    std::shared_ptr<Follows> follows1 = std::make_shared<Follows>(s1, s2, false, false);
    REQUIRE_FALSE(follows1->evaluateFollows(followsReader));

    std::shared_ptr<Follows> follows2 = std::make_shared<Follows>(s1, s2, true, false);
    REQUIRE_FALSE(follows2->evaluateFollows(followsReader));

    std::shared_ptr<Follows> follows3 = std::make_shared<Follows>(s1, s2, false, true);
    REQUIRE(follows3->evaluateFollows(followsReader));

    std::shared_ptr<Follows> follows4 = std::make_shared<Follows>(s1, s2, true, true);
    REQUIRE(follows4->evaluateFollows(followsReader));
}
