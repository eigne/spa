#include "catch.hpp"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query_evaluator/storage_reader/IWhilePatternReader.h"
#include "query_processing_system/query/design_abstractions/pattern/WhilePattern.h"

// 1: a, 2: b, 3: c
class WhileReaderStub : public IWhilePatternReader {
    std::unordered_set<int> getWhilesThatUseAnyVariable() override {
        return {1, 2, 3};
    }

    std::unordered_set<int> getWhilesThatUseVariable(std::string variable) override {
        return {variable == "a" ? 1 : variable == "b" ? 2 : 3};
    }

    std::unordered_set<std::string> getVariablesUsedInWhile(int lineNumber) override {
        return {lineNumber == 1 ? "a" : lineNumber == 2 ? "b" : "c"};
    }
};

TEST_CASE("WhilePattern: Wildcard Variable") {
    std::shared_ptr<IWhilePatternReader> patternReader = std::make_shared<WhileReaderStub>();
    VariableWildcard::allPossibleVariables = {"a", "b", "c"};

    std::shared_ptr<WhileSynonym> w = std::make_shared<WhileSynonym>("w");
    std::shared_ptr<VariableWildcard> v1 = std::make_shared<VariableWildcard>();

    w->setStatementNumbers({1, 2, 3});
    std::shared_ptr<WhilePattern> pattern1 = std::make_shared<WhilePattern>(w, v1, false);
    REQUIRE(pattern1->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));

    w->setStatementNumbers({1, 2, 3});
    std::shared_ptr<WhilePattern> pattern2 = std::make_shared<WhilePattern>(w, v1, true);
    REQUIRE_FALSE(pattern2->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers().empty());

    w->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<WhilePattern> pattern3 = std::make_shared<WhilePattern>(w, v1, true);
    REQUIRE(pattern3->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({4}));
}

TEST_CASE("WhilePattern: Name Variable") {
    std::shared_ptr<IWhilePatternReader> patternReader = std::make_shared<WhileReaderStub>();

    std::shared_ptr<WhileSynonym> w = std::make_shared<WhileSynonym>("w");
    std::shared_ptr<VariableName> v_a = std::make_shared<VariableName>("a");
    std::shared_ptr<VariableName> v_b = std::make_shared<VariableName>("b");

    w->setStatementNumbers({1, 2, 3});
    std::shared_ptr<WhilePattern> pattern1 = std::make_shared<WhilePattern>(w, v_a, false);
    REQUIRE(pattern1->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({1}));

    w->setStatementNumbers({1, 2, 3});
    std::shared_ptr<WhilePattern> pattern2 = std::make_shared<WhilePattern>(w, v_a, true);
    REQUIRE(pattern2->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({2, 3}));

    w->setStatementNumbers({1, 2, 3});
    std::shared_ptr<WhilePattern> pattern3 = std::make_shared<WhilePattern>(w, v_b, false);
    REQUIRE(pattern3->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({2}));

    w->setStatementNumbers({1, 2, 3});
    std::shared_ptr<WhilePattern> pattern4 = std::make_shared<WhilePattern>(w, v_b, true);
    REQUIRE(pattern4->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({1, 3}));
}

TEST_CASE("WhilePattern: Synonym Variable") {
    std::shared_ptr<IWhilePatternReader> patternReader = std::make_shared<WhileReaderStub>();

    std::shared_ptr<WhileSynonym> w = std::make_shared<WhileSynonym>("w");
    std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");

    QuerySelection selection({w, v});
    std::list<std::string> ans;

    w->setStatementNumbers({1, 2, 3});
    v->setNames({"a", "b", "c"});
    std::shared_ptr<WhilePattern> pattern1 = std::make_shared<WhilePattern>(w, v, false);
    REQUIRE(pattern1->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 a", "2 b", "3 c"}));

    w->setStatementNumbers({1, 2, 3});
    v->setNames({"a", "b", "c"});
    std::shared_ptr<WhilePattern> pattern2 = std::make_shared<WhilePattern>(w, v, true);
    REQUIRE(pattern2->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 b", "1 c", "2 a", "2 c", "3 a", "3 b"}));

    w->setStatementNumbers({1, 2, 3});
    v->setNames({"b", "c"});
    std::shared_ptr<WhilePattern> pattern3 = std::make_shared<WhilePattern>(w, v, false);
    REQUIRE(pattern3->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"2 b", "3 c"}));

    w->setStatementNumbers({1, 2, 3});
    v->setNames({"b", "c"});
    std::shared_ptr<WhilePattern> pattern4 = std::make_shared<WhilePattern>(w, v, true);
    REQUIRE(pattern4->evaluateWhilePattern(patternReader));
    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 b", "1 c", "2 c", "3 b"}));
}
