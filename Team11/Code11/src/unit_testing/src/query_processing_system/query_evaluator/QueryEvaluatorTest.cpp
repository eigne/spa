#include "catch.hpp"
#include "query_processing_system/query_evaluator/QueryEvaluator.h"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query/design_abstractions/pattern/AssignPattern.h"
#include "query_processing_system/query/design_abstractions/pattern/IfPattern.h"
#include "query_processing_system/query/design_abstractions/such_that/follows/Follows.h"
#include "query_processing_system/query/design_abstractions/such_that/uses/UsesStatement.h"
#include "query_processing_system/query/design_abstractions/such_that/modifies/ModifiesStatement.h"
#include "query_processing_system/query/design_abstractions/such_that/next/Next.h"
#include "query_processing_system/query/design_abstractions/such_that/calls/Calls.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/procedure_name/StatementProcedureNameAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/StatementVariableNameAttribute.h"


// p1: read(a) print(b) call(p2) while(a==b){ if(a==1){ b=2 }{ a=3 }}
// p2: read(b) print(a) if(1==2){ while(b==1){ a=2 } }{ b=3 }
class StorageReaderStub : public IStorageReader {
    std::unordered_set<int> getAllStatements() override { return {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; }
    std::unordered_set<int> getAllReads() override { return {1, 8}; }
    std::unordered_set<int> getAllPrints() override { return {2, 9}; }
    std::unordered_set<int> getAllCalls() override { return {3}; }
    std::unordered_set<int> getAllWhiles() override { return {4, 11}; }
    std::unordered_set<int> getAllIfs() override { return {5, 10}; }
    std::unordered_set<int> getAllAssigns() override { return {6, 7, 12, 13}; }

    std::unordered_set<std::string> getAllVariables() override { return {"a", "b"}; }
    std::unordered_set<std::string> getAllConstants() override { return {"1", "2", "3"}; }
    std::unordered_set<std::string> getAllProcedures() override { return {"p1", "p2"}; }

    std::unordered_set<std::string> getVariablesUsedInStatement(int statementNumber) override {
        if (statementNumber == 3 || statementNumber == 4) { return {"a", "b"}; }
        if (statementNumber == 5 || statementNumber == 9) { return {"a"}; }
        if (statementNumber == 2 || statementNumber == 10 || statementNumber == 11) { return {"b"}; }
        return {};
    }
    std::unordered_set<int> getStatementsThatUseVariable(std::string variableName) override {
        if (variableName == "a") { return {3, 4, 5, 9}; }
        if (variableName == "b") { return {2, 3, 4, 10, 11}; }
        return {};
    }
    std::unordered_set<int> getStatementsThatUseAnyVariable() override { return {2, 3, 4, 5, 9, 10, 11}; }

    std::unordered_set<std::string> getVariablesUsedInProcedure(std::string procedureName) override {
        if (procedureName == "p1" || procedureName == "p2") { return {"a", "b"}; }
        return {};
    }
    std::unordered_set<std::string> getProceduresThatUseVariable(std::string variableName) override {
        if (variableName == "a" || variableName == "b") { return {"p1", "p2"}; }
        return {};
    }
    std::unordered_set<std::string> getProceduresThatUseAnyVariable() override { return {"p1", "p2"}; }

    std::unordered_set<std::string> getVariablesModifiedInStatement(int statementNumber) override {
        if (statementNumber == 3 || statementNumber == 4 ||
            statementNumber == 5 || statementNumber == 10) { return {"a", "b"}; }
        if (statementNumber == 1 || statementNumber == 7 ||
            statementNumber == 11 || statementNumber == 12) { return {"a"}; }
        if (statementNumber == 6 || statementNumber == 8 || statementNumber == 13) { return {"b"}; }
        return {};
    }
    std::unordered_set<int> getStatementsThatModifyVariable(std::string variableName) override {
        if (variableName == "a") { return {1, 3, 4, 5, 7, 10, 11, 12}; }
        if (variableName == "b") { return {3, 4, 5, 6, 8, 10, 13}; }
        return {};
    }
    std::unordered_set<int> getStatementsThatModifyAnyVariable() override {
        return {1, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13};
    }

    std::unordered_set<std::string> getVariablesModifiedInProcedure(std::string procedureName) override {
        if (procedureName == "p1" || procedureName == "p2") { return {"a", "b"}; }
        return {};
    }
    std::unordered_set<std::string> getProceduresThatModifyVariable(std::string variableName) override {
        if (variableName == "a" || variableName == "b") { return {"p1", "p2"}; }
        return {};
    }
    std::unordered_set<std::string> getProceduresThatModifyAnyVariable() override { return {"p1", "p2"}; }

    std::unordered_set<int> getFollows(int statementNumber, bool onlyImmediate) override {
        if (statementNumber == 1) {
            return onlyImmediate ? std::unordered_set<int>({2}) : std::unordered_set<int>({2, 3, 4});
        } else if (statementNumber == 2) {
            return onlyImmediate ? std::unordered_set<int>({3}) : std::unordered_set<int>({3, 4});
        } else if (statementNumber == 3) {
            return std::unordered_set<int>({4});
        } else if (statementNumber == 8) {
            return onlyImmediate ? std::unordered_set<int>({9}) : std::unordered_set<int>({9, 10});
        } else if (statementNumber == 9) {
            return std::unordered_set<int>({10});
        } else {
                return {};
        }
    }
    std::unordered_set<int> getBefore(int statementNumber, bool onlyImmediate) override {
        if (statementNumber == 2) {
            return std::unordered_set<int>({1});
        } else if (statementNumber == 3) {
            return onlyImmediate ? std::unordered_set<int>({2}) : std::unordered_set<int>({1, 2});
        } else if (statementNumber == 4) {
            return onlyImmediate ? std::unordered_set<int>({3}) : std::unordered_set<int>({1, 2, 3});
        } else if (statementNumber == 9) {
            return std::unordered_set<int>({8});
        } else if (statementNumber == 10) {
            return onlyImmediate ? std::unordered_set<int>({9}) : std::unordered_set<int>({8, 9});
        } else {
            return {};
        }
    }
    std::unordered_set<int> getAllWithFollows() override { return {1, 2, 3, 8, 9}; }
    std::unordered_set<int> getAllWithBefore() override { return {2, 3, 4, 9, 10}; }

    std::unordered_set<int> getParent(int statementNumber, bool onlyImmediate) override {
        if (statementNumber == 5) {
            return std::unordered_set<int>({4});
        } else if (statementNumber == 6 || statementNumber == 7) {
            return onlyImmediate ? std::unordered_set<int>({5}) : std::unordered_set<int>({4, 5});
        } else if (statementNumber == 11) {
            return std::unordered_set<int>({10});
        } else if (statementNumber == 12 || statementNumber == 13) {
            return onlyImmediate ? std::unordered_set<int>({11}) : std::unordered_set<int>({10, 11});
        } else {
            return {};
        }
    }
    std::unordered_set<int> getChild(int statementNumber, bool onlyImmediate) override {
        if (statementNumber == 5) {
            return std::unordered_set<int>({6, 7});
        } else if (statementNumber == 4) {
            return onlyImmediate ? std::unordered_set<int>({5}) : std::unordered_set<int>({5, 6, 7});
        } else if (statementNumber == 11) {
            return std::unordered_set<int>({12, 13});
        } else if (statementNumber == 10) {
            return onlyImmediate ? std::unordered_set<int>({11}) : std::unordered_set<int>({11, 12, 13});
        } else {
            return {};
        }
    }
    std::unordered_set<int> getAllWithParent() override { return {5, 6, 11, 12}; }
    std::unordered_set<int> getAllWithChild() override { return {4, 5, 10, 11}; }

    std::unordered_set<std::string> getProceduresCalledBy(std::string procedureName, bool onlyImmediate) override {
        if (procedureName == "p1") { return {"p2"}; }
        return {};
    }
    std::unordered_set<std::string> getProceduresCalling(std::string procedureName, bool onlyImmediate) override {
        if (procedureName == "p2") { return {"p1"}; }
        return {};
    }
    std::unordered_set<std::string> getProceduresThatCall() override { return {"p1"}; }
    std::unordered_set<std::string> getAllCalledProcedures() override { return {"p2"}; }

    bool hasMatchingPattern(int statementNumber, std::vector<std::string> postfixExpression, bool exactMatch) override {
        if (statementNumber == 6 || statementNumber == 12) {
            return postfixExpression == std::vector<std::string>({"2"}) || (postfixExpression.empty() && !exactMatch);
        } else if (statementNumber == 7 || statementNumber == 13) {
            return postfixExpression == std::vector<std::string>({"3"}) || (postfixExpression.empty() && !exactMatch);
        }
        return false;
    }
    std::unordered_set<std::string> getVariablesUsedInIf(int lineNumber) override {
        if (lineNumber == 5) {
            return {"a"};
        }
        return {};
    }
    std::unordered_set<int> getIfsThatUseVariable(std::string variable) override {
        if (variable == "a") {
            return {5};
        }
        return {};
    }
    std::unordered_set<int> getIfsThatUseAnyVariable() override { return {5}; }

    std::unordered_set<std::string> getVariablesUsedInWhile(int lineNumber) override {
        if (lineNumber == 4) {
            return {"a", "b"};
        } else if (lineNumber == 11){
            return {"b"};
        }
        return {};
    }
    std::unordered_set<int> getWhilesThatUseVariable(std::string variable) override {
        if (variable == "a") {
            return {11};
        } else if (variable == "b"){
            return {4, 11};
        }
        return {};
    }
    std::unordered_set<int> getWhilesThatUseAnyVariable() override { return {4, 11}; }

    std::unordered_set<int> getNext(int statementNumber) override {
        if (statementNumber == 5) {
            return {6, 7};
        } else if (statementNumber == 6 || statementNumber == 7) {
            return {4};
        } else if (statementNumber == 10) {
            return {11, 13};
        } else if (statementNumber == 12) {
            return {11};
        } else if (statementNumber == 13) {
            return {};
        } else {
            return {statementNumber + 1};
        }
    }
    std::unordered_set<int> getPrevious(int statementNumber) override {
        if (statementNumber == 4) {
            return {3, 6, 7};
        } else if (statementNumber == 6 || statementNumber == 7) {
            return {5};
        } else if (statementNumber == 11) {
            return {10, 12};
        } else if (statementNumber == 13) {
            return {10};
        } else if (statementNumber == 1 || statementNumber == 8) {
            return {};
        } else {
            return {statementNumber - 1};
        }
    }

    std::string getProcedureCalledByCall(int statementNumber) override { return statementNumber == 3 ? "p2" : ""; }
};

TEST_CASE("Test No Clauses") {
    QueryEvaluator evaluator(std::make_shared<StorageReaderStub>());
    std::list<std::string> ans;

    SECTION("Boolean") {
        Query q;
        QuerySelection sel1;
        q.setSelection(sel1);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"TRUE"}));
    }

    SECTION("Synonym") {
        Query q1;
        std::shared_ptr<StatementTypeSynonym> s = std::make_shared<StatementSynonym>("s");
        QuerySelection sel1(s);
        q1.addSynonym(s);
        q1.setSelection(sel1);
        ans = evaluator.evaluateQuery(q1);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1", "10", "11", "12", "13", "2", "3", "4", "5", "6", "7", "8", "9"}));

        Query q2;
        std::shared_ptr<StatementTypeSynonym> a = std::make_shared<AssignSynonym>("a");
        QuerySelection sel2({a, a});
        q2.addSynonym(a);
        q2.setSelection(sel2);
        ans = evaluator.evaluateQuery(q2);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"12 12", "13 13", "6 6", "7 7"}));
    }

    SECTION("Attribute") {
        Query q;
        std::shared_ptr<ReadSynonym> r = std::make_shared<ReadSynonym>("r");
        std::shared_ptr<PrintSynonym> p = std::make_shared<PrintSynonym>("p");
        auto toSel1 = std::make_shared<StatementVariableNameAttribute>(r);
        auto toSel2 = std::make_shared<StatementVariableNameAttribute>(p);

        QuerySelection sel({toSel1, toSel2});
        q.addSynonym(r);
        q.addSynonym(p);
        q.setSelection(sel);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"a a", "a b", "b a", "b b"}));
    }

    SECTION("Synonym + Attribute") {
        Query q;
        std::shared_ptr<CallSynonym> c = std::make_shared<CallSynonym>("c");
        std::shared_ptr<ProcedureSynonym> p = std::make_shared<ProcedureSynonym>("p");
        auto toSel1 = std::make_shared<StatementProcedureNameAttribute>(c);

        QuerySelection sel3({toSel1, p});
        q.addSynonym(c);
        q.addSynonym(p);
        q.setSelection(sel3);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"p2 p1", "p2 p2"}));
    }
}

TEST_CASE("Test Single Clause") {
    QueryEvaluator evaluator(std::make_shared<StorageReaderStub>());
    std::list<std::string> ans;

    SECTION("Follows, 2 synonyms") {
        Query q1;
        std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");
        QuerySelection sel1({s1, s2});
        std::shared_ptr<Clause> follows = std::make_shared<Follows>(s1, s2, false, false);
        q1.addSynonym(s1);
        q1.addSynonym(s2);
        q1.setSelection(sel1);
        q1.addClause(follows);
        ans = evaluator.evaluateQuery(q1);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 2", "2 3", "3 4", "8 9", "9 10"}));
    }

    SECTION("Next, 2 synonyms") {
        Query q2;
        std::shared_ptr<StatementTypeSynonym> s3 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementTypeSynonym> s4 = std::make_shared<StatementSynonym>("s2");
        QuerySelection sel2({s3, s4});
        std::shared_ptr<Clause> next = std::make_shared<Next>(s3, s4, false, false);
        q2.addSynonym(s3);
        q2.addSynonym(s4);
        q2.setSelection(sel2);
        q2.addClause(next);
        ans = evaluator.evaluateQuery(q2);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 2", "10 11", "10 13", "11 12", "12 11",
                                               "2 3", "3 4", "4 5", "5 6", "5 7", "6 4", "7 4",
                                               "8 9", "9 10" }));
    }

    SECTION("Assign Pattern, 2 synonyms") {
        Query q3;
        std::shared_ptr<AssignSynonym> a = std::make_shared<AssignSynonym>("a");
        std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
        QuerySelection sel3({a, v});
        std::shared_ptr<Clause> pattern = std::make_shared<AssignPattern>(a, v, "", true, false);
        q3.addSynonym(a);
        q3.addSynonym(v);
        q3.setSelection(sel3);
        q3.addClause(pattern);
        ans = evaluator.evaluateQuery(q3);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"12 a", "13 b", "6 b", "7 a" }));
    }

    SECTION("Assign Pattern, 1 synonym") {
        Query q3;
        std::shared_ptr<IfSynonym> i = std::make_shared<IfSynonym>("i");
        std::shared_ptr<VariableWildcard> v = std::make_shared<VariableWildcard>();
        QuerySelection sel3(i);
        std::shared_ptr<Clause> pattern = std::make_shared<IfPattern>(i, v, false);
        q3.addSynonym(i);
        q3.setSelection(sel3);
        q3.addClause(pattern);
        ans = evaluator.evaluateQuery(q3);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"5"}));
    }

    SECTION("not Next Star, Wildcard") {
        Query q4;
        std::shared_ptr<StatementTypeSynonym> s5 = std::make_shared<StatementSynonym>("s5");
        std::shared_ptr<StatementWildcard> s_w = std::make_shared<StatementWildcard>();
        QuerySelection sel4(s5);
        std::shared_ptr<Clause> next2 = std::make_shared<Next>(s5, s_w, true, true);
        q4.addSynonym(s5);
        q4.setSelection(sel4);
        q4.addClause(next2);
        ans = evaluator.evaluateQuery(q4);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"13"}));
    }

    SECTION("Next Star, number") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s = std::make_shared<StatementSynonym>("s");
        std::shared_ptr<StatementLineNumber> s_4 = std::make_shared<StatementLineNumber>(4);
        QuerySelection sel(s);
        std::shared_ptr<Clause> next2 = std::make_shared<Next>(s, s_4, true, false);
        q.addSynonym(s);
        q.setSelection(sel);
        q.addClause(next2);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1", "2", "3", "4", "5", "6", "7"}));
    }
}

TEST_CASE("Multiple clauses") {
    QueryEvaluator evaluator(std::make_shared<StorageReaderStub>());
    std::list<std::string> ans;

    SECTION("Falsy clause not in selection") {
        Query q;
        std::shared_ptr<AssignSynonym> a1 = std::make_shared<AssignSynonym>("a1");
        std::shared_ptr<AssignSynonym> a2 = std::make_shared<AssignSynonym>("a2");
        std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
        QuerySelection sel(v);
        std::shared_ptr<Clause> follows = std::make_shared<Follows>(a1, a2, true, false);
        std::shared_ptr<Clause> pattern = std::make_shared<AssignPattern>(a2, v, "2", true, false);
        q.addSynonym(a1);
        q.addSynonym(a2);
        q.addSynonym(v);
        q.setSelection(sel);
        q.addClause(follows);
        q.addClause(pattern);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({}));
    }

    SECTION("No shared synonyms") {
        Query q;
        std::shared_ptr<AssignSynonym> a1 = std::make_shared<AssignSynonym>("a1");
        std::shared_ptr<AssignSynonym> a2 = std::make_shared<AssignSynonym>("a2");
        std::shared_ptr<VariableSynonym> v1 = std::make_shared<VariableSynonym>("v1");
        std::shared_ptr<VariableSynonym> v2 = std::make_shared<VariableSynonym>("v2");
        QuerySelection sel({a1, v2});
        std::shared_ptr<Clause> pattern1 = std::make_shared<AssignPattern>(a1, v1, "2", false, false);
        std::shared_ptr<Clause> pattern2 = std::make_shared<AssignPattern>(a2, v2, "3", false, false);
        q.addSynonym(a1);
        q.addSynonym(a2);
        q.addSynonym(v1);
        q.addSynonym(v2);
        q.setSelection(sel);
        q.addClause(pattern1);
        q.addClause(pattern2);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"12 a", "12 b", "6 a", "6 b"}));
    }

    SECTION("Shared synonyms selected") {
        Query q;
        std::shared_ptr<AssignSynonym> a1 = std::make_shared<AssignSynonym>("a1");
        std::shared_ptr<AssignSynonym> a2 = std::make_shared<AssignSynonym>("a2");
        std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v1");
        QuerySelection sel({a1, a2, v});
        std::shared_ptr<Clause> pattern1 = std::make_shared<AssignPattern>(a1, v, "2", false, false);
        std::shared_ptr<Clause> pattern2 = std::make_shared<AssignPattern>(a2, v, "3", false, false);
        q.addSynonym(a1);
        q.addSynonym(a2);
        q.addSynonym(v);
        q.setSelection(sel);
        q.addClause(pattern1);
        q.addClause(pattern2);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"12 7 a", "6 13 b"}));
    }

    SECTION("Shared synonym not selected") {
        Query q;
        std::shared_ptr<AssignSynonym> a1 = std::make_shared<AssignSynonym>("a1");
        std::shared_ptr<AssignSynonym> a2 = std::make_shared<AssignSynonym>("a2");
        std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v1");
        QuerySelection sel({a1, a2});
        std::shared_ptr<Clause> pattern1 = std::make_shared<AssignPattern>(a1, v, "2", false, false);
        std::shared_ptr<Clause> pattern2 = std::make_shared<AssignPattern>(a2, v, "3", false, false);
        q.addSynonym(a1);
        q.addSynonym(a2);
        q.addSynonym(v);
        q.setSelection(sel);
        q.addClause(pattern1);
        q.addClause(pattern2);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"12 7", "6 13"}));
    }

    SECTION("Clauses form a cycle") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");
        std::shared_ptr<StatementTypeSynonym> s3 = std::make_shared<StatementSynonym>("s3");
        QuerySelection sel({s1, s2, s3});
        std::shared_ptr<Clause> next1 = std::make_shared<Next>(s1, s2, false, false);
        std::shared_ptr<Clause> next2 = std::make_shared<Next>(s2, s3, false, false);
        std::shared_ptr<Clause> next3 = std::make_shared<Next>(s3, s1, false, false);
        q.addSynonym(s1);
        q.addSynonym(s2);
        q.addSynonym(s3);
        q.setSelection(sel);
        q.addClause(next1);
        q.addClause(next2);
        q.addClause(next3);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"4 5 6", "4 5 7", "5 6 4", "5 7 4", "6 4 5", "7 4 5"}));
    }

    SECTION("Next* form a cycle") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");
        std::shared_ptr<StatementTypeSynonym> s3 = std::make_shared<StatementSynonym>("s3");
        QuerySelection sel({s1, s2, s3});
        std::shared_ptr<Clause> next1 = std::make_shared<Next>(s1, s2, true, false);
        std::shared_ptr<Clause> next2 = std::make_shared<Next>(s2, s3, true, false);
        std::shared_ptr<Clause> next3 = std::make_shared<Next>(s2, s1, true, false);
        q.addSynonym(s1);
        q.addSynonym(s2);
        q.addSynonym(s3);
        q.setSelection(sel);
        q.addClause(next1);
        q.addClause(next2);
        q.addClause(next3);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"11 11 11", "11 11 12", "11 12 11", "11 12 12",
                                               "12 11 11", "12 11 12", "12 12 11", "12 12 12",
                                               "4 4 4", "4 4 5", "4 4 6", "4 4 7",
                                               "4 5 4", "4 5 5", "4 5 6", "4 5 7",
                                               "4 6 4", "4 6 5", "4 6 6", "4 6 7",
                                               "4 7 4", "4 7 5", "4 7 6", "4 7 7",
                                               "5 4 4", "5 4 5", "5 4 6", "5 4 7",
                                               "5 5 4", "5 5 5", "5 5 6", "5 5 7",
                                               "5 6 4", "5 6 5", "5 6 6", "5 6 7",
                                               "5 7 4", "5 7 5", "5 7 6", "5 7 7",
                                               "6 4 4", "6 4 5", "6 4 6", "6 4 7",
                                               "6 5 4", "6 5 5", "6 5 6", "6 5 7",
                                               "6 6 4", "6 6 5", "6 6 6", "6 6 7",
                                               "6 7 4", "6 7 5", "6 7 6", "6 7 7",
                                               "7 4 4", "7 4 5", "7 4 6", "7 4 7",
                                               "7 5 4", "7 5 5", "7 5 6", "7 5 7",
                                               "7 6 4", "7 6 5", "7 6 6", "7 6 7",
                                               "7 7 4", "7 7 5", "7 7 6", "7 7 7"}));
    }

    SECTION("Relation bridge is limited without knowledge by selected") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");
        std::shared_ptr<StatementTypeSynonym> s3 = std::make_shared<StatementSynonym>("s3");
        std::shared_ptr<VariableWildcard> v = std::make_shared<VariableWildcard>();
        QuerySelection sel({s1, s3});
        std::shared_ptr<Clause> follows1 = std::make_shared<Follows>(s1, s2, false, false);
        std::shared_ptr<Clause> follows2 = std::make_shared<Follows>(s2, s3, false, false);
        std::shared_ptr<Clause> uses = std::make_shared<UsesStatement>(s2, v, false);
        q.addSynonym(s1);
        q.addSynonym(s2);
        q.addSynonym(s3);
        q.setSelection(sel);
        q.addClause(follows1);
        q.addClause(follows2);
        q.addClause(uses);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"1 3", "2 4", "8 10"}));
    }

    SECTION("Combination of clauses leads to empty intersection") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        std::shared_ptr<StatementTypeSynonym> s2 = std::make_shared<StatementSynonym>("s2");
        std::shared_ptr<StatementTypeSynonym> s3 = std::make_shared<StatementSynonym>("s3");
        std::shared_ptr<VariableWildcard> v = std::make_shared<VariableWildcard>();
        QuerySelection sel(s2);
        std::shared_ptr<Clause> follows1 = std::make_shared<Follows>(s1, s2, false, false);
        std::shared_ptr<Clause> follows2 = std::make_shared<Follows>(s2, s3, false, false);
        std::shared_ptr<Clause> uses = std::make_shared<UsesStatement>(s2, v, true);
        q.addSynonym(s1);
        q.addSynonym(s2);
        q.addSynonym(s3);
        q.setSelection(sel);
        q.addClause(follows1);
        q.addClause(follows2);
        q.addClause(uses);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({}));
    }

    SECTION("Same synonyms in two clauses") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s = std::make_shared<StatementSynonym>("s");
        std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
        QuerySelection sel({s, v});
        std::shared_ptr<Clause> uses = std::make_shared<UsesStatement>(s, v, false);
        std::shared_ptr<Clause> modifies = std::make_shared<ModifiesStatement>(s, v, false);
        q.addSynonym(s);
        q.addSynonym(v);
        q.setSelection(sel);
        q.addClause(uses);
        q.addClause(modifies);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"10 b", "3 a", "3 b", "4 a", "4 b", "5 a"}));
    }

    SECTION("Same clause happens twice") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s = std::make_shared<StatementSynonym>("s");
        std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
        QuerySelection sel(s);
        std::shared_ptr<Clause> uses1 = std::make_shared<UsesStatement>(s, v, false);
        std::shared_ptr<Clause> uses2 = std::make_shared<UsesStatement>(s, v, false);
        q.addSynonym(s);
        q.addSynonym(v);
        q.setSelection(sel);
        q.addClause(uses1);
        q.addClause(uses2);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"10", "11", "2", "3", "4", "5", "9"}));
    }

    SECTION("A clause and its negation") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s = std::make_shared<StatementSynonym>("s");
        std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
        QuerySelection sel(s);
        std::shared_ptr<Clause> uses1 = std::make_shared<UsesStatement>(s, v, false);
        std::shared_ptr<Clause> uses2 = std::make_shared<UsesStatement>(s, v, true);
        q.addSynonym(s);
        q.addSynonym(v);
        q.setSelection(sel);
        q.addClause(uses1);
        q.addClause(uses2);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({}));
    }

    SECTION("A clause and its negation (swap arguments)") {
        Query q;
        std::shared_ptr<ProcedureSynonym> p1 = std::make_shared<ProcedureSynonym>("p1");
        std::shared_ptr<ProcedureSynonym> p2 = std::make_shared<ProcedureSynonym>("p2");
        QuerySelection sel({p1, p2});
        std::shared_ptr<Clause> calls1 = std::make_shared<Calls>(p1, p2, false, false);
        std::shared_ptr<Clause> calls2 = std::make_shared<Calls>(p2, p1, false, true);
        q.addSynonym(p1);
        q.addSynonym(p2);
        q.setSelection(sel);
        q.addClause(calls1);
        q.addClause(calls2);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"p1 p2"}));
    }

    SECTION("A clause and its negation (change argument)") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s = std::make_shared<StatementSynonym>("s");
        std::shared_ptr<VariableSynonym> v1 = std::make_shared<VariableSynonym>("v1");
        std::shared_ptr<VariableSynonym> v2 = std::make_shared<VariableSynonym>("v2");
        QuerySelection sel({s, v1, v2});
        std::shared_ptr<Clause> uses1 = std::make_shared<UsesStatement>(s, v1, false);
        std::shared_ptr<Clause> uses2 = std::make_shared<UsesStatement>(s, v2, true);
        q.addSynonym(s);
        q.addSynonym(v1);
        q.addSynonym(v2);
        q.setSelection(sel);
        q.addClause(uses1);
        q.addClause(uses2);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"10 b a", "11 b a", "2 b a", "5 a b", "9 a b"}));
    }

    SECTION("Intersecting and differencing yourself") {
        Query q;
        std::shared_ptr<StatementTypeSynonym> s1 = std::make_shared<StatementSynonym>("s1");
        QuerySelection sel(s1);
        std::shared_ptr<Clause> next1 = std::make_shared<Next>(s1, s1, false, true);
        std::shared_ptr<Clause> next2 = std::make_shared<Next>(s1, s1, true, false);
        // s1 not next self, but next* self
        q.addSynonym(s1);
        q.setSelection(sel);
        q.addClause(next1);
        q.addClause(next2);
        ans = evaluator.evaluateQuery(q);
        ans.sort();
        REQUIRE(ans == std::list<std::string>({"11", "12", "4", "5", "6", "7"}));
    }
}
