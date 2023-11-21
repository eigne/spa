#include "catch.hpp"
#include "query_processing_system/query/design_entities/statement_type/synonyms/AssignSynonym.h"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query_evaluator/storage_reader/IAffectsReader.h"
#include "query_processing_system/query/design_abstractions/such_that/affects/Affects.h"

class AffectsReaderStub : public IAffectsReader {
    std::unordered_set<std::string> getVariablesModifiedInStatement(int statementNumber) {
        if (statementNumber == 1) return {"x"};
        if (statementNumber == 2) return {"i"};
        if (statementNumber == 4) return {"x"};
        if (statementNumber == 5) return {"z", "v"};
        if (statementNumber == 6) return {"i"};
        if (statementNumber == 8) return {"x"};
        if (statementNumber == 9) return {"z"};
        if (statementNumber == 10) return {"z"};
        if (statementNumber == 11) return {"y"};
        if (statementNumber == 12) return {"x"};
        return {};
    }

    std::unordered_set<int> getStatementsThatModifyVariable(std::string variableName) {
        if (variableName == "x") return {1, 4, 8, 12};
        if (variableName == "i") return {2, 6};
        if (variableName == "z") return {5, 9, 10};
        if (variableName == "v") return {5};
        if (variableName == "y") return {11};
        return {};
    }

    std::unordered_set<std::string> getVariablesUsedInStatement(int statementNumber) {
        if (statementNumber == 3) return {"i"};
        if (statementNumber == 4) return {"x", "y"};
        if (statementNumber == 5) return {"v"};
        if (statementNumber == 6) return {"i"};
        if (statementNumber == 7) return {"x"};
        if (statementNumber == 8) return {"x"};
        if (statementNumber == 10) return {"z", "x", "i"};
        if (statementNumber == 11) return {"z"};
        if (statementNumber == 12) return {"x", "y", "z"};
        return {};
    }

    std::unordered_set<int> getStatementsThatUseVariable(std::string variableName) {
        if (variableName == "x") return {4, 7, 8, 10, 12};
        if (variableName == "i") return {3, 6, 10};
        if (variableName == "z") return {10, 11, 12};
        if (variableName == "v") return {5};
        if (variableName == "y") return {4, 12};
        return {};
    }

    std::unordered_set<int> getNext(int statementNumber) {
        if (statementNumber == 1) return {2};
        if (statementNumber == 2) return {3};
        if (statementNumber == 3) return {4, 7};
        if (statementNumber == 4) return {5};
        if (statementNumber == 5) return {6};
        if (statementNumber == 6) return {3};
        if (statementNumber == 7) return {8, 9};
        if (statementNumber == 8) return {10};
        if (statementNumber == 9) return {10};
        if (statementNumber == 10) return {11};
        if (statementNumber == 11) return {12};
        if (statementNumber == 12) return {};
        return {};
    }

    std::unordered_set<int> getPrevious(int statementNumber) {
        if (statementNumber == 1) return {};
        if (statementNumber == 2) return {1};
        if (statementNumber == 3) return {2, 6};
        if (statementNumber == 4) return {3};
        if (statementNumber == 5) return {4};
        if (statementNumber == 6) return {5};
        if (statementNumber == 7) return {3};
        if (statementNumber == 8) return {7};
        if (statementNumber == 9) return {7};
        if (statementNumber == 10) return {8, 9};
        if (statementNumber == 11) return {10};
        if (statementNumber == 12) return {11};
        return {};
    }

    std::unordered_set<int> getAllAssigns() {
        return {1, 2, 4, 6, 8, 9, 10, 11, 12};
    }

    // unused
    std::unordered_set<int> getStatementsThatModifyAnyVariable() { return {}; }
    std::unordered_set<int> getStatementsThatUseAnyVariable() { return {}; }
    std::unordered_set<int> getAllWithNext() { return {}; }
    std::unordered_set<int> getAllWithPrevious() { return {}; }
    std::unordered_set<std::string> getAllVariables() { return {}; }
    std::unordered_set<std::string> getAllConstants() { return {}; }
    std::unordered_set<std::string> getAllProcedures() { return {}; }
    std::string getProcedureCalledByCall(int statementNumber) { return {}; }
    std::unordered_set<int> getAllStatements() { return {}; }
    std::unordered_set<int> getAllReads() { return {}; }
    std::unordered_set<int> getAllPrints() { return {}; }
    std::unordered_set<int> getAllCalls() { return {}; }
    std::unordered_set<int> getAllWhiles() { return {}; }
    std::unordered_set<int> getAllIfs() { return {}; }
};

TEST_CASE("Visit Affects Same Synonym") {
    std::shared_ptr<IAffectsReader> affectsReader = std::make_shared<AffectsReaderStub>();

    std::shared_ptr<StatementTypeSynonym> a1 = std::make_shared<AssignSynonym>("a1");

    a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
    std::shared_ptr<Affects> affects1 = std::make_shared<Affects>(a1, a1, false);
    auto test = affects1->evaluateAffects(affectsReader);
    REQUIRE(affects1->evaluateAffects(affectsReader));
    REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 4, 6 }));

    a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
    std::shared_ptr<Affects> affects2 = std::make_shared<Affects>(a1, a1, true);
    REQUIRE(affects2->evaluateAffects(affectsReader));
    REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 8, 9, 10, 11, 12 }));
}

TEST_CASE("Visit Affects One Synonym") {
    std::shared_ptr<IAffectsReader> affectsReader = std::make_shared<AffectsReaderStub>();
    StatementWildcard::allPossibleStatements = { 1, 2, 4, 6, 8, 9, 10, 11, 12 };

    std::shared_ptr<StatementTypeSynonym> a1 = std::make_shared<AssignSynonym>("a1");
    std::shared_ptr<StatementLineNumber> a_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> a_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> a_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementLineNumber> a_6 = std::make_shared<StatementLineNumber>(6);
    std::shared_ptr<StatementLineNumber> a_8 = std::make_shared<StatementLineNumber>(8);
    std::shared_ptr<StatementLineNumber> a_9 = std::make_shared<StatementLineNumber>(9);
    std::shared_ptr<StatementLineNumber> a_10 = std::make_shared<StatementLineNumber>(10);
    std::shared_ptr<StatementLineNumber> a_11 = std::make_shared<StatementLineNumber>(11);
    std::shared_ptr<StatementLineNumber> a_12 = std::make_shared<StatementLineNumber>(12);
    std::shared_ptr<StatementWildcard> s = std::make_shared<StatementWildcard>();

    SECTION("Line 1") {
        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects1 = std::make_shared<Affects>(a1, a_1, false);
        REQUIRE_FALSE(affects1->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers().empty());

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects2 = std::make_shared<Affects>(a1, a_1, true);
        REQUIRE(affects2->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 4, 6, 8, 9, 10, 11, 12 }));

        a1->setStatementNumbers({1, 2, 4, 6, 8, 9, 10, 11, 12});
        std::shared_ptr<Affects> affects3 = std::make_shared<Affects>(a_1, a1, false);
        REQUIRE(affects3->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 4, 8, 10, 12 }));

        a1->setStatementNumbers({1, 2, 4, 6, 8, 9, 10, 11, 12});
        std::shared_ptr<Affects> affects4 = std::make_shared<Affects>(a_1, a1, true);
        REQUIRE(affects4->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 6, 9, 11 }));
    }

    SECTION("LINE 2") {
        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects1 = std::make_shared<Affects>(a1, a_2, false);
        REQUIRE_FALSE(affects1->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers().empty());

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects2 = std::make_shared<Affects>(a1, a_2, true);
        REQUIRE(affects2->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 4, 6, 8, 9, 10, 11, 12 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects3 = std::make_shared<Affects>(a_2, a1, false);
        REQUIRE(affects3->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 6, 10 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects4 = std::make_shared<Affects>(a_2, a1, true);
        REQUIRE(affects4->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 4, 8, 9, 11, 12 }));
    }

    SECTION("LINE 4") {
        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects1 = std::make_shared<Affects>(a1, a_4, false);
        REQUIRE(affects1->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 4 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects2 = std::make_shared<Affects>(a1, a_4, true);
        REQUIRE(affects2->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 2, 6, 8, 9, 10, 11, 12 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects3 = std::make_shared<Affects>(a_4, a1, false);
        REQUIRE(affects3->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 4, 8, 10, 12 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects4 = std::make_shared<Affects>(a_4, a1, true);
        REQUIRE(affects4->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 6, 9, 11 }));
    }

    SECTION("LINE 8") {
        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects1 = std::make_shared<Affects>(a1, a_8, false);
        REQUIRE(affects1->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 4 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects2 = std::make_shared<Affects>(a1, a_8, true);
        REQUIRE(affects2->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 2, 6, 8, 9, 10, 11, 12 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects3 = std::make_shared<Affects>(a_8, a1, false);
        REQUIRE(affects3->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 10, 12 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects4 = std::make_shared<Affects>(a_8, a1, true);
        REQUIRE(affects4->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 4, 6, 8, 9, 11 }));
    }

    SECTION("LINE 12") {
        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects1 = std::make_shared<Affects>(a1, a_12, false);
        REQUIRE(affects1->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 4, 8, 10, 11 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects2 = std::make_shared<Affects>(a1, a_12, true);
        REQUIRE(affects2->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 2, 6, 9, 12 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects3 = std::make_shared<Affects>(a_12, a1, false);
        REQUIRE_FALSE(affects3->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers().empty());

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects4 = std::make_shared<Affects>(a_12, a1, true);
        REQUIRE(affects4->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 4, 6, 8, 9, 10, 11, 12 }));
    }

    SECTION("Wildcard") {
        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affecta1 = std::make_shared<Affects>(a1, s, false);
        REQUIRE(affecta1->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 4, 6, 8, 9, 10, 11 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects2 = std::make_shared<Affects>(a1, s, true);
        REQUIRE(affects2->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 12 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects3 = std::make_shared<Affects>(s, a1, false);
        REQUIRE(affects3->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 4, 6, 8, 10, 11, 12 }));

        a1->setStatementNumbers({ 1, 2, 4, 6, 8, 9, 10, 11, 12 });
        std::shared_ptr<Affects> affects4 = std::make_shared<Affects>(s, a1, true);
        REQUIRE(affects4->evaluateAffects(affectsReader));
        REQUIRE(a1->getStatementNumbers() == std::unordered_set<int>({ 1, 2, 9 }));
    }
}

TEST_CASE("Visit Affects No Synonym") {
    std::shared_ptr<IAffectsReader> affectsReader = std::make_shared<AffectsReaderStub>();
    StatementWildcard::allPossibleStatements = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

    std::shared_ptr<StatementWildcard> a_w1 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementWildcard> a_w2 = std::make_shared<StatementWildcard>();
    std::shared_ptr<StatementLineNumber> a_1 = std::make_shared<StatementLineNumber>(1);
    std::shared_ptr<StatementLineNumber> a_2 = std::make_shared<StatementLineNumber>(2);
    std::shared_ptr<StatementLineNumber> a_3 = std::make_shared<StatementLineNumber>(3);
    std::shared_ptr<StatementLineNumber> a_4 = std::make_shared<StatementLineNumber>(4);
    std::shared_ptr<StatementLineNumber> a_5 = std::make_shared<StatementLineNumber>(5);
    std::shared_ptr<StatementLineNumber> a_6 = std::make_shared<StatementLineNumber>(6);
    std::shared_ptr<StatementLineNumber> a_7 = std::make_shared<StatementLineNumber>(7);
    std::shared_ptr<StatementLineNumber> a_8 = std::make_shared<StatementLineNumber>(8);
    std::shared_ptr<StatementLineNumber> a_9 = std::make_shared<StatementLineNumber>(9);
    std::shared_ptr<StatementLineNumber> a_10 = std::make_shared<StatementLineNumber>(10);
    std::shared_ptr<StatementLineNumber> a_11 = std::make_shared<StatementLineNumber>(11);
    std::shared_ptr<StatementLineNumber> a_12 = std::make_shared<StatementLineNumber>(12);

    SECTION("Two Wildcards") {
        std::shared_ptr<Affects> affects1 = std::make_shared<Affects>(a_w1, a_w2, false);
        REQUIRE(affects1->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects2 = std::make_shared<Affects>(a_w1, a_w2, true);
        REQUIRE_FALSE(affects2->evaluateAffects(affectsReader));
    }

    SECTION("One Wildcard") {
        std::shared_ptr<Affects> affects_1w = std::make_shared<Affects>(a_1, a_w1, false);
        REQUIRE(affects_1w->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_1wn = std::make_shared<Affects>(a_1, a_w1, true);
        REQUIRE_FALSE(affects_1wn->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_w1 = std::make_shared<Affects>(a_w1, a_1, false);
        REQUIRE_FALSE(affects_w1->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_w1n = std::make_shared<Affects>(a_w1, a_1, true);
        REQUIRE(affects_w1n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_2w = std::make_shared<Affects>(a_2, a_w1, false);
        REQUIRE(affects_2w->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_2wn = std::make_shared<Affects>(a_2, a_w1, true);
        REQUIRE_FALSE(affects_2wn->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_w2 = std::make_shared<Affects>(a_w1, a_2, false);
        REQUIRE_FALSE(affects_w2->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_w2n = std::make_shared<Affects>(a_w1, a_2, true);
        REQUIRE(affects_w2n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_4w = std::make_shared<Affects>(a_4, a_w1, false);
        REQUIRE(affects_4w->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_4wn = std::make_shared<Affects>(a_4, a_w1, true);
        REQUIRE_FALSE(affects_4wn->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_w4 = std::make_shared<Affects>(a_w1, a_4, false);
        REQUIRE(affects_w4->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_w4n = std::make_shared<Affects>(a_w1, a_4, true);
        REQUIRE_FALSE(affects_w4n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_8w = std::make_shared<Affects>(a_8, a_w1, false);
        REQUIRE(affects_8w->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_8wn = std::make_shared<Affects>(a_8, a_w1, true);
        REQUIRE_FALSE(affects_8wn->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_w8 = std::make_shared<Affects>(a_w1, a_8, false);
        REQUIRE(affects_w8->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_w8n = std::make_shared<Affects>(a_w1, a_8, true);
        REQUIRE_FALSE(affects_w8n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_12w = std::make_shared<Affects>(a_12, a_w1, false);
        REQUIRE_FALSE(affects_12w->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_12wn = std::make_shared<Affects>(a_12, a_w1, true);
        REQUIRE(affects_12wn->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_w12 = std::make_shared<Affects>(a_w1, a_12, false);
        REQUIRE(affects_w12->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_w12n = std::make_shared<Affects>(a_w1, a_12, true);
        REQUIRE_FALSE(affects_w12n->evaluateAffects(affectsReader));
    }

    SECTION("No Wildcard") {
        std::shared_ptr<Affects> affects_26 = std::make_shared<Affects>(a_2, a_6, false);
        REQUIRE(affects_26->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_26n = std::make_shared<Affects>(a_2, a_6, true);
        REQUIRE_FALSE(affects_26n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_48 = std::make_shared<Affects>(a_4, a_8, false);
        REQUIRE(affects_48->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_48n = std::make_shared<Affects>(a_4, a_8, true);
        REQUIRE_FALSE(affects_48n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_911 = std::make_shared<Affects>(a_9, a_11, false);
        REQUIRE_FALSE(affects_911->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_911n = std::make_shared<Affects>(a_9, a_11, true);
        REQUIRE(affects_911n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_912 = std::make_shared<Affects>(a_9, a_12, false);
        REQUIRE_FALSE(affects_912->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_912n = std::make_shared<Affects>(a_9, a_12, true);
        REQUIRE(affects_912n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_410 = std::make_shared<Affects>(a_4, a_10, false);
        REQUIRE(affects_410->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_410n = std::make_shared<Affects>(a_4, a_10, true);
        REQUIRE_FALSE(affects_410n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_66 = std::make_shared<Affects>(a_6, a_6, false);
        REQUIRE(affects_66->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_66n = std::make_shared<Affects>(a_6, a_6, true);
        REQUIRE_FALSE(affects_66n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_14 = std::make_shared<Affects>(a_1, a_4, false);
        REQUIRE(affects_14->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_14n = std::make_shared<Affects>(a_1, a_4, true);
        REQUIRE_FALSE(affects_14n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_18 = std::make_shared<Affects>(a_1, a_8, false);
        REQUIRE(affects_18->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_18n = std::make_shared<Affects>(a_1, a_8, true);
        REQUIRE_FALSE(affects_18n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_110 = std::make_shared<Affects>(a_1, a_10, false);
        REQUIRE(affects_110->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_110n = std::make_shared<Affects>(a_1, a_10, true);
        REQUIRE_FALSE(affects_110n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_112 = std::make_shared<Affects>(a_1, a_12, false);
        REQUIRE(affects_112->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_112n = std::make_shared<Affects>(a_1, a_12, true);
        REQUIRE_FALSE(affects_112n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_210 = std::make_shared<Affects>(a_2, a_10, false);
        REQUIRE(affects_210->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_210n = std::make_shared<Affects>(a_2, a_10, true);
        REQUIRE_FALSE(affects_210n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_910 = std::make_shared<Affects>(a_9, a_10, false);
        REQUIRE(affects_910->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_910n = std::make_shared<Affects>(a_9, a_10, true);
        REQUIRE_FALSE(affects_910n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_23 = std::make_shared<Affects>(a_2, a_3, false);
        REQUIRE_FALSE(affects_23->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_23n = std::make_shared<Affects>(a_2, a_3, true);
        REQUIRE(affects_23n->evaluateAffects(affectsReader));

        std::shared_ptr<Affects> affects_96 = std::make_shared<Affects>(a_9, a_6, false);
        REQUIRE_FALSE(affects_96->evaluateAffects(affectsReader));
        std::shared_ptr<Affects> affects_96n = std::make_shared<Affects>(a_9, a_6, true);
        REQUIRE(affects_96n->evaluateAffects(affectsReader));
    }
}
