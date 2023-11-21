#include "catch.hpp"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "commons/ExpressionParser.h"
#include "query_processing_system/query_evaluator/storage_reader/IAssignPatternReader.h"
#include "query_processing_system/query/design_abstractions/pattern/AssignPattern.h"

// 1: a = a + b + c
// 2: b = 2 * a + 2 * b + 2 * c
// 3: c = 3 * a + 3 * b + 3 * c
class PatternReaderStub : public IAssignPatternReader {
    std::vector<std::string> postfix1 = ExpressionParser::convertToPostfix("a + b + c");
    std::vector<std::string> postfix2 = ExpressionParser::convertToPostfix("2 * a + 2 * b + 2 * c");
    std::vector<std::string> postfix3 = ExpressionParser::convertToPostfix("3 * a + 3 * b + 3 * c");

    bool hasMatchingPattern(int statementNumber, std::vector<std::string> postfixExpression, bool exactMatch) override {
        auto toCompare = statementNumber == 1 ? postfix1 : statementNumber == 2 ? postfix2 : postfix3;
        if (exactMatch) {
            return postfixExpression == toCompare;
        } else {
            return ExpressionParser::isSubarray(toCompare, postfixExpression);
        }
    }

    std::unordered_set<std::string> getVariablesModifiedInStatement(int statementNumber) override {
        if (statementNumber == 1) {
            return {"a"};
        } else if (statementNumber == 2) {
            return {"b"};
        } else {
            return {"c"};
        }
    }

    std::unordered_set<int> getStatementsThatModifyVariable(std::string variableName) override {
        if (variableName == "a") {
            return {1};
        } else if (variableName == "b") {
            return {2};
        } else {
            return {3};
        }
    }

    std::unordered_set<int> getStatementsThatModifyAnyVariable() override {
        return {1, 2, 3};
    }
};

TEST_CASE("AssignPattern: Wildcard Variable") {
    std::shared_ptr<IAssignPatternReader> patternReader = std::make_shared<PatternReaderStub>();
    VariableWildcard::allPossibleVariables = {"a", "b", "c"};

    std::shared_ptr<AssignSynonym> a1 = std::make_shared<AssignSynonym>("a");
    std::shared_ptr<VariableWildcard> v1 = std::make_shared<VariableWildcard>();

    SECTION("Pure wildcard") {
        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern1 = std::make_shared<AssignPattern>(a1, v1, "", true, false);
        REQUIRE(pattern1->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern2 = std::make_shared<AssignPattern>(a1, v1, "", true, true);
        REQUIRE_FALSE(pattern2->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers().empty());
    }

    SECTION("Wildcard + string") {
        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern1 = std::make_shared<AssignPattern>(a1, v1, "a+b", true, false);
        REQUIRE(pattern1->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern2 = std::make_shared<AssignPattern>(a1, v1, "a+b", true, true);
        REQUIRE(pattern2->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({2, 3}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern3 = std::make_shared<AssignPattern>(a1, v1, "a+2*b", true, false);
        REQUIRE_FALSE(pattern3->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers().empty());

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern4 = std::make_shared<AssignPattern>(a1, v1, "a+2*b", true, true);
        REQUIRE(pattern4->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
    }

    SECTION("Only string") {
        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern1 = std::make_shared<AssignPattern>(a1, v1, "a+b", false, false);
        REQUIRE_FALSE(pattern1->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers().empty());

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern2 = std::make_shared<AssignPattern>(a1, v1, "a+b", false, true);
        REQUIRE(pattern2->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern3 = std::make_shared<AssignPattern>(a1, v1, "a+b+c", false, false);
        REQUIRE(pattern3->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern4 = std::make_shared<AssignPattern>(a1, v1, "a+b+c", false, true);
        REQUIRE(pattern4->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    }
}

TEST_CASE("AssignPattern: Name Variable") {
    std::shared_ptr<IAssignPatternReader> patternReader = std::make_shared<PatternReaderStub>();

    std::shared_ptr<AssignSynonym> a1 = std::make_shared<AssignSynonym>("a");
    std::shared_ptr<VariableName> v_a = std::make_shared<VariableName>("a");

    SECTION("Pure wildcard") {
        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern1 = std::make_shared<AssignPattern>(a1, v_a, "", true, false);
        REQUIRE(pattern1->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern2 = std::make_shared<AssignPattern>(a1, v_a, "", true, true);
        REQUIRE(pattern2->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    }

    SECTION("Wildcard + string") {
        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern1 = std::make_shared<AssignPattern>(a1, v_a, "a+b", true, false);
        REQUIRE(pattern1->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern2 = std::make_shared<AssignPattern>(a1, v_a, "a+b", true, true);
        REQUIRE(pattern2->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({2, 3}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern3 = std::make_shared<AssignPattern>(a1, v_a, "a", true, false);
        REQUIRE(pattern3->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern4 = std::make_shared<AssignPattern>(a1, v_a, "a", true, true);
        REQUIRE(pattern4->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    }

    SECTION("Only string") {
        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern1 = std::make_shared<AssignPattern>(a1, v_a, "a+b", false, false);
        REQUIRE_FALSE(pattern1->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers().empty());

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern2 = std::make_shared<AssignPattern>(a1, v_a, "a+b", false, true);
        REQUIRE(pattern2->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern3 = std::make_shared<AssignPattern>(a1, v_a, "a+b+c", false, false);
        REQUIRE(pattern3->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1}));

        a1->setStatementNumbers({1, 2, 3});
        std::shared_ptr<AssignPattern> pattern4 = std::make_shared<AssignPattern>(a1, v_a, "a+b+c", false, true);
        REQUIRE(pattern4->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({2, 3}));
    }
}

TEST_CASE("AssignPattern: Synonym Variable") {
    std::shared_ptr<IAssignPatternReader> patternReader = std::make_shared<PatternReaderStub>();

    std::shared_ptr<AssignSynonym> a1 = std::make_shared<AssignSynonym>("a");
    std::shared_ptr<VariableSynonym> v1 = std::make_shared<VariableSynonym>("v");

    QuerySelection selection = QuerySelection({a1, v1});
    std::list<std::string> ans;

    SECTION("Pure wildcard") {
        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b", "c"});
        std::shared_ptr<AssignPattern> pattern1 = std::make_shared<AssignPattern>(a1, v1, "", true, false);
        REQUIRE(pattern1->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 a", "2 b", "3 c"}));

        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b", "c"});
        std::shared_ptr<AssignPattern> pattern2 = std::make_shared<AssignPattern>(a1, v1, "", true, true);
        REQUIRE(pattern2->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 b", "1 c", "2 a", "2 c", "3 a", "3 b"}));

        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b"});
        std::shared_ptr<AssignPattern> pattern3 = std::make_shared<AssignPattern>(a1, v1, "", true, false);
        REQUIRE(pattern3->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 a", "2 b"}));

        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b"});
        std::shared_ptr<AssignPattern> pattern4 = std::make_shared<AssignPattern>(a1, v1, "", true, true);
        REQUIRE(pattern4->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 b", "2 a", "3 a", "3 b"}));
    }

    SECTION("Wildcard + string") {
        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b", "c"});
        std::shared_ptr<AssignPattern> pattern1 = std::make_shared<AssignPattern>(a1, v1, "a+b", true, false);
        REQUIRE(pattern1->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 a"}));

        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b", "c"});
        std::shared_ptr<AssignPattern> pattern2 = std::make_shared<AssignPattern>(a1, v1, "a+b", true, true);
        REQUIRE(pattern2->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 b", "1 c", "2 a", "2 b", "2 c", "3 a", "3 b", "3 c"}));

        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"b", "c"});
        std::shared_ptr<AssignPattern> pattern3 = std::make_shared<AssignPattern>(a1, v1, "a", true, false);
        REQUIRE(pattern3->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({2, 3}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"b", "c"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"2 b", "3 c"}));

        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"b", "c"});
        std::shared_ptr<AssignPattern> pattern4 = std::make_shared<AssignPattern>(a1, v1, "a", true, true);
        REQUIRE(pattern4->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"b", "c"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 b", "1 c", "2 c", "3 b"}));
    }

    SECTION("Only string") {
        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b", "c"});
        std::shared_ptr<AssignPattern> pattern1 = std::make_shared<AssignPattern>(a1, v1, "a+b", false, false);
        REQUIRE_FALSE(pattern1->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers().empty());
        REQUIRE(v1->getNames().empty());
        REQUIRE(selection.getAnswer(false).empty());

        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b", "c"});
        std::shared_ptr<AssignPattern> pattern2 = std::make_shared<AssignPattern>(a1, v1, "a+b", false, true);
        REQUIRE(pattern2->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 a", "1 b", "1 c", "2 a", "2 b", "2 c", "3 a", "3 b", "3 c"}));

        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b", "c"});
        std::shared_ptr<AssignPattern> pattern3 = std::make_shared<AssignPattern>(a1, v1, "a+b+c", false, false);
        REQUIRE(pattern3->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 a"}));

        a1->setStatementNumbers({1, 2, 3});
        v1->setNames({"a", "b", "c"});
        std::shared_ptr<AssignPattern> pattern4 = std::make_shared<AssignPattern>(a1, v1, "a+b+c", false, true);
        REQUIRE(pattern4->evaluateAssignPattern(patternReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({1, 2, 3}));
        REQUIRE(v1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
        ans = selection.getAnswer(true);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 b", "1 c", "2 a", "2 b", "2 c", "3 a", "3 b", "3 c"}));
    }
}
