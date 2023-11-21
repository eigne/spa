#include "catch.hpp"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementSynonym.h"
#include "query_processing_system/query_evaluator/storage_reader/IUsesStatementReader.h"
#include "query_processing_system/query/design_abstractions/such_that/uses/UsesStatement.h"

// 1 -> ab, 2 -> bc, 3-> ac
class UsesStatementReaderStub : public IUsesStatementReader {
    std::unordered_set<std::string> getVariablesUsedInStatement(int lineNumber) override {
        if (lineNumber == 1) {
            return {"a", "b"};
        } else if (lineNumber == 2) {
            return {"b", "c"};
        } else {
            return {"a", "c"};
        }
    }

    std::unordered_set<int> getStatementsThatUseVariable(std::string variableName) override {
        if (variableName == "a") {
            return {1, 3};
        } else if (variableName == "b") {
            return {1, 2};
        } else {
            return {2, 3};
        }
    }

    std::unordered_set<int> getStatementsThatUseAnyVariable() override {
        return {1, 2, 3};
    }
};

TEST_CASE("Visit UsesStatement Two Synonyms") {
    std::shared_ptr<IUsesStatementReader> usesReader = std::make_shared<UsesStatementReaderStub>();

    std::shared_ptr<StatementSynonym> s = std::make_shared<StatementSynonym>("a");
    std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
    QuerySelection selection({s, v});
    std::list<std::string> ans;

    s->setStatementNumbers({1, 2, 3});
    v->setNames({"a", "b", "c"});
    std::shared_ptr<UsesStatement> uses1 = std::make_shared<UsesStatement>(s, v, false);
    REQUIRE(uses1->evaluateUsesStatement(usesReader));
    REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 a", "1 b", "2 b", "2 c", "3 a", "3 c"}));

    s->setStatementNumbers({1, 2, 3});
    v->setNames({"a", "b", "c"});
    std::shared_ptr<UsesStatement> uses2 = std::make_shared<UsesStatement>(s, v, true);
    REQUIRE(uses2->evaluateUsesStatement(usesReader));
    REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 c", "2 a", "3 b"}));
}

TEST_CASE("Visit UsesStatement One Synonym") {
    VariableWildcard::allPossibleVariables = {"a", "b", "c"};
    std::shared_ptr<IUsesStatementReader> usesReader = std::make_shared<UsesStatementReaderStub>();

    std::shared_ptr<StatementSynonym> s = std::make_shared<StatementSynonym>("a");
    std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
    std::shared_ptr<VariableName> v_a = std::make_shared<VariableName>("a");
    std::shared_ptr<VariableWildcard> v_w = std::make_shared<VariableWildcard>();
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);

    SECTION("Variable Name") {
        s->setStatementNumbers({1, 2, 3});
        std::shared_ptr<UsesStatement> uses1 = std::make_shared<UsesStatement>(s, v_a, false);
        REQUIRE(uses1->evaluateUsesStatement(usesReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({1, 3}));

        s->setStatementNumbers({1, 2, 3});
        std::shared_ptr<UsesStatement> uses2 = std::make_shared<UsesStatement>(s, v_a, true);
        REQUIRE(uses2->evaluateUsesStatement(usesReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({2}));
    }

    SECTION("Variable Wildcard") {
        s->setStatementNumbers({1, 2, 3});
        std::shared_ptr<UsesStatement> uses1 = std::make_shared<UsesStatement>(s, v_w, false);
        REQUIRE(uses1->evaluateUsesStatement(usesReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));

        s->setStatementNumbers({1, 2, 3});
        std::shared_ptr<UsesStatement> uses2 = std::make_shared<UsesStatement>(s, v_w, true);
        REQUIRE_FALSE(uses2->evaluateUsesStatement(usesReader));
        REQUIRE(s->getStatementNumbers().empty());

        s->setStatementNumbers({1, 2, 3, 4});
        std::shared_ptr<UsesStatement> uses3 = std::make_shared<UsesStatement>(s, v_w, true);
        REQUIRE(uses3->evaluateUsesStatement(usesReader));
        REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({4}));
    }

    SECTION("Statement Name, Variable Synonym") {
        v->setNames({"a", "b", "c"});
        std::shared_ptr<UsesStatement> uses1 = std::make_shared<UsesStatement>(s_1, v, false);
        REQUIRE(uses1->evaluateUsesStatement(usesReader));
        REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b"}));

        v->setNames({"a", "b", "c"});
        std::shared_ptr<UsesStatement> uses2 = std::make_shared<UsesStatement>(s_1, v, true);
        REQUIRE(uses2->evaluateUsesStatement(usesReader));
        REQUIRE(v->getNames() == std::unordered_set<std::string>({"c"}));
    }
}

TEST_CASE("Visit UsesStatement No Synonym") {
    VariableWildcard::allPossibleVariables = {"a", "b", "c"};
    std::shared_ptr<IUsesStatementReader> usesReader = std::make_shared<UsesStatementReaderStub>();

    std::shared_ptr<VariableName> v_a = std::make_shared<VariableName>("a");
    std::shared_ptr<VariableWildcard> v_w = std::make_shared<VariableWildcard>();
    std::shared_ptr<StatementLineNumber> s_1 = std::make_shared<StatementLineNumber>(1);

    SECTION("Variable Name") {
        std::shared_ptr<UsesStatement> uses1 = std::make_shared<UsesStatement>(s_1, v_a, false);
        REQUIRE(uses1->evaluateUsesStatement(usesReader));

        std::shared_ptr<UsesStatement> uses2 = std::make_shared<UsesStatement>(s_1, v_a, true);
        REQUIRE_FALSE(uses2->evaluateUsesStatement(usesReader));
    }

    SECTION("Variable Wildcard") {
        std::shared_ptr<UsesStatement> uses1 = std::make_shared<UsesStatement>(s_1, v_w, false);
        REQUIRE(uses1->evaluateUsesStatement(usesReader));

        std::shared_ptr<UsesStatement> uses2 = std::make_shared<UsesStatement>(s_1, v_w, true);
        REQUIRE_FALSE(uses2->evaluateUsesStatement(usesReader));
    }
}
