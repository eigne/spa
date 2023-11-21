#include "catch.hpp"
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementSynonym.h"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query_evaluator/storage_reader/IParentReader.h"
#include "query_processing_system/query/design_abstractions/such_that/parent/Parent.h"

// 1{2, 3{4}}
class ParentReaderStub : public IParentReader {
    std::unordered_set<int> getParent(int statementNumber, bool onlyImmediate) override {
        if (statementNumber == 2 || statementNumber == 3) {
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

    std::unordered_set<int> getChild(int statementNumber, bool onlyImmediate) override {
        if (statementNumber == 3) {
            return {4};
        } else if (statementNumber == 1) {
            if (onlyImmediate) {
                return {2, 3};
            } else {
                return {2, 3, 4};
            }
        } else {
            return {};
        }
    }

    std::unordered_set<int> getAllWithParent() override {
        return {2, 3, 4};
    }

    std::unordered_set<int> getAllWithChild() override {
        return {1, 3};
    }
};

class NoNestingStub : public IParentReader {
    std::unordered_set<int> getParent(int statementNumber, bool onlyImmediate) override {
        return {};
    }

    std::unordered_set<int> getChild(int statementNumber, bool onlyImmediate) override {
        return {};
    }

    std::unordered_set<int> getAllWithParent() override {
        return {};
    }

    std::unordered_set<int> getAllWithChild() override {
        return {};
    }
};

TEST_CASE("Visit Parent Same Synonym") {
    std::shared_ptr<IParentReader> parentReader = std::make_shared<ParentReaderStub>();

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");

    s1->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s1, false, false);
    REQUIRE_FALSE(parent1->evaluateParent(parentReader));

    s1->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s1, false, true);
    REQUIRE(parent2->evaluateParent(parentReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
}

TEST_CASE("Visit Parent Two Synonyms") {
    std::shared_ptr<IParentReader> parentReader = std::make_shared<ParentReaderStub>();

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");

    QuerySelection selection({s1, s2});
    std::list<std::string> ans;

    s1->setStatementNumbers({1, 2, 3, 4});
    s2->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s2, false, false);
    REQUIRE(parent1->evaluateParent(parentReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 2", "1 3", "3 4"}));

    s1->setStatementNumbers({1, 2, 3, 4});
    s2->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s2, false, true);
    REQUIRE(parent2->evaluateParent(parentReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 1", "1 4",
                                           "2 1", "2 2", "2 3", "2 4",
                                           "3 1", "3 2", "3 3",
                                           "4 1", "4 2", "4 3", "4 4"}));
}

TEST_CASE("Visit ParentStar Two Synonyms") {
    std::shared_ptr<IParentReader> parentReader = std::make_shared<ParentReaderStub>();

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");

    QuerySelection selection({s1, s2});
    std::list<std::string> ans;

    s1->setStatementNumbers({1, 2, 3, 4});
    s2->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s2, true, false);
    REQUIRE(parent1->evaluateParent(parentReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 2", "1 3", "1 4", "3 4"}));

    s1->setStatementNumbers({1, 2, 3, 4});
    s2->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s2, true, true);
    REQUIRE(parent2->evaluateParent(parentReader));
    REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    REQUIRE(s2->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 1",
                                           "2 1", "2 2", "2 3", "2 4",
                                           "3 1", "3 2", "3 3",
                                           "4 1", "4 2", "4 3", "4 4"}));
}

TEST_CASE("Visit Parent One Synonym") {
    std::shared_ptr<IParentReader> parentReader = std::make_shared<ParentReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4};

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementWildcard> s = std::make_shared<StatementWildcard>();

    SECTION("Line 1") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s_1, false, false);
        REQUIRE_FALSE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s_1, false, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s_1, s1, false, false);
        REQUIRE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s_1, s1, false, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 4}));
    }

    SECTION("Line 2") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s_2, false, false);
        REQUIRE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s_2, false, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s_2, s1, false, false);
        REQUIRE_FALSE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s_2, s1, false, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    }

    SECTION("Line 3") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s_3, false, false);
        REQUIRE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s_3, false, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s_3, s1, false, false);
        REQUIRE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s_3, s1, false, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    }

    SECTION("Line 4") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s_4, false, false);
        REQUIRE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s_4, false, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s_4, s1, false, false);
        REQUIRE_FALSE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s_4, s1, false, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    }

    SECTION("Wildcard") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s, false, false);
        REQUIRE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s, false, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s, s1, false, false);
        REQUIRE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s, s1, false, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));
    }
}

TEST_CASE("Visit ParentStar One Synonym") {
    std::shared_ptr<IParentReader> parentReader = std::make_shared<ParentReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4};

    std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementWildcard> s = std::make_shared<StatementWildcard>();

    SECTION("Line 1") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s_1, true, false);
        REQUIRE_FALSE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s_1, true, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s_1, s1, true, false);
        REQUIRE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s_1, s1, true, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));
    }

    SECTION("Line 2") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s_2, true, false);
        REQUIRE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s_2, true, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s_2, s1, true, false);
        REQUIRE_FALSE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s_2, s1, true, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    }

    SECTION("Line 3") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s_3, true, false);
        REQUIRE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s_3, true, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s_3, s1, true, false);
        REQUIRE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s_3, s1, true, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    }

    SECTION("Line 4") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s_4, true, false);
        REQUIRE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s_4, true, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s_4, s1, true, false);
        REQUIRE_FALSE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers().empty());

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s_4, s1, true, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4}));
    }

    SECTION("Wildcard") {
        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s, true, false);
        REQUIRE(parent1->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1, 3}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s, true, true);
        REQUIRE(parent2->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s, s1, true, false);
        REQUIRE(parent3->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({2, 3, 4}));

        s1->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s, s1, true, true);
        REQUIRE(parent4->evaluateParent(parentReader));
        REQUIRE(s1->getStatementNumbers() == std::unordered_set<int>({1}));
    }
}

TEST_CASE("Visit Parent No Synonym") {
    std::shared_ptr<IParentReader> parentReader = std::make_shared<ParentReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4};

    std::shared_ptr<StatementWildcard> s_w1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> s_w2 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);

    SECTION("Two Wildcards") {
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s_w1, s_w2, false, false);
        REQUIRE(parent1->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s_w1, s_w2, false, true);
        REQUIRE_FALSE(parent2->evaluateParent(parentReader));
    }

    SECTION("One Wildcard") {
        std::shared_ptr<Parent> parent_w1 = std::make_shared<Parent>(s_w1, s_1, false, false);
        REQUIRE_FALSE(parent_w1->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_w1n = std::make_shared<Parent>(s_w1, s_1, false, true);
        REQUIRE(parent_w1n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_1w = std::make_shared<Parent>(s_1, s_w1, false, false);
        REQUIRE(parent_1w->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_1wn = std::make_shared<Parent>(s_1, s_w1, false, true);
        REQUIRE_FALSE(parent_1wn->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_w2 = std::make_shared<Parent>(s_w1, s_2, false, false);
        REQUIRE(parent_w2->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_w2n = std::make_shared<Parent>(s_w1, s_2, false, true);
        REQUIRE_FALSE(parent_w2n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_2w = std::make_shared<Parent>(s_2, s_w1, false, false);
        REQUIRE_FALSE(parent_2w->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_2wn = std::make_shared<Parent>(s_2, s_w1, false, true);
        REQUIRE(parent_2wn->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_w3 = std::make_shared<Parent>(s_w1, s_3, false, false);
        REQUIRE(parent_w3->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_w3n = std::make_shared<Parent>(s_w1, s_3, false, true);
        REQUIRE_FALSE(parent_w3n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_3w = std::make_shared<Parent>(s_3, s_w1, false, false);
        REQUIRE(parent_3w->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_3wn = std::make_shared<Parent>(s_3, s_w1, false, true);
        REQUIRE_FALSE(parent_3wn->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_w4 = std::make_shared<Parent>(s_w1, s_4, false, false);
        REQUIRE(parent_w4->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_w4n = std::make_shared<Parent>(s_w1, s_4, false, true);
        REQUIRE_FALSE(parent_w4n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_4w = std::make_shared<Parent>(s_4, s_w1, false, false);
        REQUIRE_FALSE(parent_4w->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_4wn = std::make_shared<Parent>(s_4, s_w1, false, true);
        REQUIRE(parent_4wn->evaluateParent(parentReader));
    }

    SECTION("No Wildcard") {
        std::shared_ptr<Parent> parent_11 = std::make_shared<Parent>(s_1, s_1, false, false);
        REQUIRE_FALSE(parent_11->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_11n = std::make_shared<Parent>(s_1, s_1, false, true);
        REQUIRE(parent_11n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_12 = std::make_shared<Parent>(s_1, s_2, false, false);
        REQUIRE(parent_12->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_12n = std::make_shared<Parent>(s_1, s_2, false, true);
        REQUIRE_FALSE(parent_12n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_13 = std::make_shared<Parent>(s_1, s_3, false, false);
        REQUIRE(parent_13->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_13n = std::make_shared<Parent>(s_1, s_3, false, true);
        REQUIRE_FALSE(parent_13n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_14 = std::make_shared<Parent>(s_1, s_4, false, false);
        REQUIRE_FALSE(parent_14->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_14n = std::make_shared<Parent>(s_1, s_4, false, true);
        REQUIRE(parent_14n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_21 = std::make_shared<Parent>(s_2, s_1, false, false);
        REQUIRE_FALSE(parent_21->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_21n = std::make_shared<Parent>(s_2, s_1, false, true);
        REQUIRE(parent_21n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_22 = std::make_shared<Parent>(s_2, s_2, false, false);
        REQUIRE_FALSE(parent_22->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_22n = std::make_shared<Parent>(s_2, s_2, false, true);
        REQUIRE(parent_22n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_23 = std::make_shared<Parent>(s_2, s_3, false, false);
        REQUIRE_FALSE(parent_23->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_23n = std::make_shared<Parent>(s_2, s_3, false, true);
        REQUIRE(parent_23n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_24 = std::make_shared<Parent>(s_2, s_4, false, false);
        REQUIRE_FALSE(parent_24->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_24n = std::make_shared<Parent>(s_2, s_4, false, true);
        REQUIRE(parent_24n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_31 = std::make_shared<Parent>(s_3, s_1, false, false);
        REQUIRE_FALSE(parent_31->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_31n = std::make_shared<Parent>(s_3, s_1, false, true);
        REQUIRE(parent_31n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_32 = std::make_shared<Parent>(s_3, s_2, false, false);
        REQUIRE_FALSE(parent_32->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_32n = std::make_shared<Parent>(s_3, s_2, false, true);
        REQUIRE(parent_32n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_33 = std::make_shared<Parent>(s_3, s_3, false, false);
        REQUIRE_FALSE(parent_33->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_33n = std::make_shared<Parent>(s_3, s_3, false, true);
        REQUIRE(parent_33n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_34 = std::make_shared<Parent>(s_3, s_4, false, false);
        REQUIRE(parent_34->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_34n = std::make_shared<Parent>(s_3, s_4, false, true);
        REQUIRE_FALSE(parent_34n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_41 = std::make_shared<Parent>(s_4, s_1, false, false);
        REQUIRE_FALSE(parent_41->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_41n = std::make_shared<Parent>(s_4, s_1, false, true);
        REQUIRE(parent_41n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_42 = std::make_shared<Parent>(s_4, s_2, false, false);
        REQUIRE_FALSE(parent_42->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_42n = std::make_shared<Parent>(s_4, s_2, false, true);
        REQUIRE(parent_42n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_43 = std::make_shared<Parent>(s_4, s_3, false, false);
        REQUIRE_FALSE(parent_43->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_43n = std::make_shared<Parent>(s_4, s_3, false, true);
        REQUIRE(parent_43n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_44 = std::make_shared<Parent>(s_4, s_4, false, false);
        REQUIRE_FALSE(parent_44->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_44n = std::make_shared<Parent>(s_4, s_4, false, true);
        REQUIRE(parent_44n->evaluateParent(parentReader));
    }
}

TEST_CASE("Visit ParentStar No Synonym") {
    std::shared_ptr<IParentReader> parentReader = std::make_shared<ParentReaderStub>();
    StatementWildcard::allPossibleStatements = {1, 2, 3, 4};

    std::shared_ptr<StatementWildcard> s_w1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> s_w2 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> s_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> s_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);

    SECTION("Two Wildcards") {
        std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s_w1, s_w2, true, false);
        REQUIRE(parent1->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s_w1, s_w2, true, true);
        REQUIRE_FALSE(parent2->evaluateParent(parentReader));
    }

    SECTION("One Wildcard") {
        std::shared_ptr<Parent> parent_w1 = std::make_shared<Parent>(s_w1, s_1, true, false);
        REQUIRE_FALSE(parent_w1->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_w1n = std::make_shared<Parent>(s_w1, s_1, true, true);
        REQUIRE(parent_w1n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_1w = std::make_shared<Parent>(s_1, s_w1, true, false);
        REQUIRE(parent_1w->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_1wn = std::make_shared<Parent>(s_1, s_w1, true, true);
        REQUIRE_FALSE(parent_1wn->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_w2 = std::make_shared<Parent>(s_w1, s_2, true, false);
        REQUIRE(parent_w2->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_w2n = std::make_shared<Parent>(s_w1, s_2, true, true);
        REQUIRE_FALSE(parent_w2n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_2w = std::make_shared<Parent>(s_2, s_w1, true, false);
        REQUIRE_FALSE(parent_2w->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_2wn = std::make_shared<Parent>(s_2, s_w1, true, true);
        REQUIRE(parent_2wn->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_w3 = std::make_shared<Parent>(s_w1, s_3, true, false);
        REQUIRE(parent_w3->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_w3n = std::make_shared<Parent>(s_w1, s_3, true, true);
        REQUIRE_FALSE(parent_w3n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_3w = std::make_shared<Parent>(s_3, s_w1, true, false);
        REQUIRE(parent_3w->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_3wn = std::make_shared<Parent>(s_3, s_w1, true, true);
        REQUIRE_FALSE(parent_3wn->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_w4 = std::make_shared<Parent>(s_w1, s_4, true, false);
        REQUIRE(parent_w4->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_w4n = std::make_shared<Parent>(s_w1, s_4, true, true);
        REQUIRE_FALSE(parent_w4n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_4w = std::make_shared<Parent>(s_4, s_w1, true, false);
        REQUIRE_FALSE(parent_4w->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_4wn = std::make_shared<Parent>(s_4, s_w1, true, true);
        REQUIRE(parent_4wn->evaluateParent(parentReader));
    }

    SECTION("No Wildcard") {
        std::shared_ptr<Parent> parent_11 = std::make_shared<Parent>(s_1, s_1, true, false);
        REQUIRE_FALSE(parent_11->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_11n = std::make_shared<Parent>(s_1, s_1, true, true);
        REQUIRE(parent_11n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_12 = std::make_shared<Parent>(s_1, s_2, true, false);
        REQUIRE(parent_12->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_12n = std::make_shared<Parent>(s_1, s_2, true, true);
        REQUIRE_FALSE(parent_12n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_13 = std::make_shared<Parent>(s_1, s_3, true, false);
        REQUIRE(parent_13->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_13n = std::make_shared<Parent>(s_1, s_3, true, true);
        REQUIRE_FALSE(parent_13n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_14 = std::make_shared<Parent>(s_1, s_4, true, false);
        REQUIRE(parent_14->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_14n = std::make_shared<Parent>(s_1, s_4, true, true);
        REQUIRE_FALSE(parent_14n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_21 = std::make_shared<Parent>(s_2, s_1, true, false);
        REQUIRE_FALSE(parent_21->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_21n = std::make_shared<Parent>(s_2, s_1, true, true);
        REQUIRE(parent_21n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_22 = std::make_shared<Parent>(s_2, s_2, true, false);
        REQUIRE_FALSE(parent_22->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_22n = std::make_shared<Parent>(s_2, s_2, true, true);
        REQUIRE(parent_22n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_23 = std::make_shared<Parent>(s_2, s_3, true, false);
        REQUIRE_FALSE(parent_23->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_23n = std::make_shared<Parent>(s_2, s_3, true, true);
        REQUIRE(parent_23n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_24 = std::make_shared<Parent>(s_2, s_4, true, false);
        REQUIRE_FALSE(parent_24->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_24n = std::make_shared<Parent>(s_2, s_4, true, true);
        REQUIRE(parent_24n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_31 = std::make_shared<Parent>(s_3, s_1, true, false);
        REQUIRE_FALSE(parent_31->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_31n = std::make_shared<Parent>(s_3, s_1, true, true);
        REQUIRE(parent_31n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_32 = std::make_shared<Parent>(s_3, s_2, true, false);
        REQUIRE_FALSE(parent_32->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_32n = std::make_shared<Parent>(s_3, s_2, true, true);
        REQUIRE(parent_32n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_33 = std::make_shared<Parent>(s_3, s_3, true, false);
        REQUIRE_FALSE(parent_33->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_33n = std::make_shared<Parent>(s_3, s_3, true, true);
        REQUIRE(parent_33n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_34 = std::make_shared<Parent>(s_3, s_4, true, false);
        REQUIRE(parent_34->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_34n = std::make_shared<Parent>(s_3, s_4, true, true);
        REQUIRE_FALSE(parent_34n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_41 = std::make_shared<Parent>(s_4, s_1, true, false);
        REQUIRE_FALSE(parent_41->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_41n = std::make_shared<Parent>(s_4, s_1, true, true);
        REQUIRE(parent_41n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_42 = std::make_shared<Parent>(s_4, s_2, true, false);
        REQUIRE_FALSE(parent_42->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_42n = std::make_shared<Parent>(s_4, s_2, true, true);
        REQUIRE(parent_42n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_43 = std::make_shared<Parent>(s_4, s_3, true, false);
        REQUIRE_FALSE(parent_43->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_43n = std::make_shared<Parent>(s_4, s_3, true, true);
        REQUIRE(parent_43n->evaluateParent(parentReader));

        std::shared_ptr<Parent> parent_44 = std::make_shared<Parent>(s_4, s_4, true, false);
        REQUIRE_FALSE(parent_44->evaluateParent(parentReader));
        std::shared_ptr<Parent> parent_44n = std::make_shared<Parent>(s_4, s_4, true, true);
        REQUIRE(parent_44n->evaluateParent(parentReader));
    }
}

TEST_CASE("No Parent") {
    std::shared_ptr<IParentReader> parentReader = std::make_shared<NoNestingStub>();
    StatementWildcard::allPossibleStatements = {1};
    std::shared_ptr<StatementWildcard> s1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> s2 = std::make_shared<StatementWildcard>();

    std::shared_ptr<Parent> parent1 = std::make_shared<Parent>(s1, s2, false, false);
    REQUIRE_FALSE(parent1->evaluateParent(parentReader));

    std::shared_ptr<Parent> parent2 = std::make_shared<Parent>(s1, s2, true, false);
    REQUIRE_FALSE(parent2->evaluateParent(parentReader));

    std::shared_ptr<Parent> parent3 = std::make_shared<Parent>(s1, s2, false, true);
    REQUIRE(parent3->evaluateParent(parentReader));

    std::shared_ptr<Parent> parent4 = std::make_shared<Parent>(s1, s2, true, true);
    REQUIRE(parent4->evaluateParent(parentReader));
}
