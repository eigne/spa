#include "catch.hpp"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query_evaluator/storage_reader/IIfPatternReader.h"
#include "query_processing_system/query/design_abstractions/pattern/IfPattern.h"

// 1: a, 2: b, 3: c, 4: no var
class IfReaderStub : public IIfPatternReader {
    std::unordered_set<int> getIfsThatUseAnyVariable() override {
        return {1, 2, 3};
    }

    std::unordered_set<int> getIfsThatUseVariable(std::string variable) override {
        return {variable == "a" ? 1 : variable == "b" ? 2 : 3};
    }

    std::unordered_set<std::string> getVariablesUsedInIf(int lineNumber) override {
        return {lineNumber == 1 ? "a" : lineNumber == 2 ? "b" : "c"};
    }
};

TEST_CASE("IfPattern: Wildcard Variable") {
    std::shared_ptr<IIfPatternReader> patternReader = std::make_shared<IfReaderStub>();
    VariableWildcard::allPossibleVariables = {"a", "b", "c"};

    std::shared_ptr<IfSynonym> i = std::make_shared<IfSynonym>("i");
    std::shared_ptr<VariableWildcard> v1 = std::make_shared<VariableWildcard>();

    i->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<IfPattern> pattern1 = std::make_shared<IfPattern>(i, v1, false);
    REQUIRE(pattern1->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));

    i->setStatementNumbers({1, 2, 3});
    std::shared_ptr<IfPattern> pattern2 = std::make_shared<IfPattern>(i, v1, true);
    REQUIRE_FALSE(pattern2->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers().empty());

    i->setStatementNumbers({1, 2, 3, 4});
    std::shared_ptr<IfPattern> pattern3 = std::make_shared<IfPattern>(i, v1, true);
    REQUIRE(pattern3->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({4}));
}

TEST_CASE("IfPattern: Name Variable") {
    std::shared_ptr<IIfPatternReader> patternReader = std::make_shared<IfReaderStub>();

    std::shared_ptr<IfSynonym> i = std::make_shared<IfSynonym>("i");
    std::shared_ptr<VariableName> v_a = std::make_shared<VariableName>("a");
    std::shared_ptr<VariableName> v_b = std::make_shared<VariableName>("b");

    i->setStatementNumbers({1, 2, 3});
    std::shared_ptr<IfPattern> pattern1 = std::make_shared<IfPattern>(i, v_a, false);
    REQUIRE(pattern1->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({1}));

    i->setStatementNumbers({1, 2, 3});
    std::shared_ptr<IfPattern> pattern2 = std::make_shared<IfPattern>(i, v_a, true);
    REQUIRE(pattern2->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({2, 3}));

    i->setStatementNumbers({1, 2, 3});
    std::shared_ptr<IfPattern> pattern3 = std::make_shared<IfPattern>(i, v_b, false);
    REQUIRE(pattern3->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({2}));

    i->setStatementNumbers({1, 2, 3});
    std::shared_ptr<IfPattern> pattern4 = std::make_shared<IfPattern>(i, v_b, true);
    REQUIRE(pattern4->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({1, 3}));
}

TEST_CASE("IfPattern: Synonym Variable") {
    std::shared_ptr<IIfPatternReader> patternReader = std::make_shared<IfReaderStub>();

    std::shared_ptr<IfSynonym> i = std::make_shared<IfSynonym>("i");
    std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");

    QuerySelection selection({i, v});
    std::list<std::string> ans;

    i->setStatementNumbers({1, 2, 3});
    v->setNames({"a", "b", "c"});
    std::shared_ptr<IfPattern> pattern1 = std::make_shared<IfPattern>(i, v, false);
    REQUIRE(pattern1->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 a", "2 b", "3 c"}));

    i->setStatementNumbers({1, 2, 3});
    v->setNames({"a", "b", "c"});
    std::shared_ptr<IfPattern> pattern2 = std::make_shared<IfPattern>(i, v, true);
    REQUIRE(pattern2->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 b", "1 c", "2 a", "2 c", "3 a", "3 b"}));

    i->setStatementNumbers({1, 2, 3});
    v->setNames({"b", "c"});
    std::shared_ptr<IfPattern> pattern3 = std::make_shared<IfPattern>(i, v, false);
    REQUIRE(pattern3->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"2 b", "3 c"}));

    i->setStatementNumbers({1, 2, 3});
    v->setNames({"b", "c"});
    std::shared_ptr<IfPattern> pattern4 = std::make_shared<IfPattern>(i, v, true);
    REQUIRE(pattern4->evaluateIfPattern(patternReader));
    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"1 b", "1 c", "2 c", "3 b"}));
}
