#include "program_knowledge_base//PKB.h"
#include "program_knowledge_base/facades/PKBSP.h"
#include "program_knowledge_base/facades/PKBQPS.h"
#include "source_processor/SourceProcessor.h"
#include "catch.hpp"
#include <unordered_set>
#include "../TestPKBUtility.h"
#include <filesystem>

std::shared_ptr<PKBSP> pkbSP;
std::shared_ptr<PKBQPS> pkbQPS;
std::string inputPath;
std::string testDir;
SourceProcessor sp;

void initSPTest(const std::string& testFile) {
    testDir = "/src/PKBSP/inputs/";
    sp = SourceProcessor();
    auto pkbPair{createPKBPair()};
    pkbSP = std::make_shared<PKBSP>(pkbPair.first);
    pkbQPS = std::make_shared<PKBQPS>(pkbPair.second);
    auto currentPath{std::filesystem::current_path().string() + testDir};
    inputPath = currentPath + testFile;
    sp.process(inputPath, pkbSP);
}

TEST_CASE("PKBSP insertRead test") {
    initSPTest("insertRead.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllReads() == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getAllVariables() == std::unordered_set<std::string>{"a"});
}

TEST_CASE("PKBSP insertRead.txt negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllReads().empty());
}

TEST_CASE("PKBSP insertPrint test") {
    initSPTest("insertPrint.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllPrints() == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getAllVariables() == std::unordered_set<std::string>{"a"});
}

TEST_CASE("PKBSP insertPrint negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllPrints().empty());
}

TEST_CASE("PKBSP insertAssign test") {
    initSPTest("insertAssign.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllAssigns() == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getAllVariables() == std::unordered_set<std::string>{"x"});
}

TEST_CASE("PKBSP insertAssign negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllAssigns().empty());
}

TEST_CASE("PKBSP insertWhile test") {
    initSPTest("insertWhile.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllAssigns() == std::unordered_set<int>{1, 3});
    REQUIRE(pkbQPS->getAllWhiles() == std::unordered_set<int>{2});
    REQUIRE(pkbQPS->getAllVariables() == std::unordered_set<std::string>{"x"});
}

TEST_CASE("PKBSP insertWhile negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllWhiles().empty());
}

TEST_CASE("PKBSP insertWhile constants only test") {
    initSPTest("insertWhileConstantsOnly.txt");
    REQUIRE(pkbQPS->getAllWhiles() == std::unordered_set<int>{1, 3});
    REQUIRE(pkbQPS->getWhilesThatUseAnyVariable() == std::unordered_set<int>{3});
}

TEST_CASE("PKBSP insertIf test") {
    initSPTest("insertIf.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllIfs() == std::unordered_set<int>{2});
    REQUIRE(pkbQPS->getAllVariables() == std::unordered_set<std::string>{"x"});
}

TEST_CASE("PKBSP insertIf negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
    REQUIRE(pkbQPS->getAllIfs().empty());
}

TEST_CASE("PKBSP insertIf constants only test") {
    initSPTest("insertIfConstantsOnly.txt");
    REQUIRE(pkbQPS->getAllIfs() == std::unordered_set<int>{1, 4});
    REQUIRE(pkbQPS->getIfsThatUseAnyVariable() == std::unordered_set<int>{4});
}

TEST_CASE("PKBSP insertIf invalid test - container only") {
    REQUIRE_THROWS_WITH(initSPTest("insertIfInvalidContainerOnly.txt"), "Missing then keyword");
}

TEST_CASE("PKBSP insertIf invalid test - missing then") {
    REQUIRE_THROWS_WITH(initSPTest("insertIfInvalidMissingThen.txt"), "Missing then keyword");
}

TEST_CASE("PKBSP insertIf invalid test - missing else") {
    REQUIRE_THROWS_WITH(initSPTest("insertIfInvalidMissingElse.txt"), "Missing else keyword");
}

TEST_CASE("PKBSP insertProcedure test") {
    initSPTest("insertProcedure.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"main"});
}

TEST_CASE("PKBSP insertProcedure multiple test") {
    initSPTest("insertProcedureMultiple.txt");
    REQUIRE(pkbQPS->getAllProcedures() == std::unordered_set<std::string>{"above1", "above2", "main", "below1", "below2"});
}

TEST_CASE("PKBSP insertVariable test") {
    initSPTest("insertVariable.txt");
    REQUIRE(pkbQPS->getAllVariables() == std::unordered_set<std::string>{"x"});
}

TEST_CASE("PKBSP insertVariable negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getAllVariables().empty());
}

// Inserts 1 as a variable, variable validation missing
TEST_CASE("PKBSP insertVariable invalid test - invalid variable name") {
    REQUIRE_THROWS_WITH(initSPTest("insertVariableInvalidInvalidVariableName.txt"), "Variable 1 to be assigned is not a valid variable name");
}

TEST_CASE("PKBSP insertVariable multiple test") {
    initSPTest("insertVariableMultiple.txt");
    REQUIRE(pkbQPS->getAllVariables() == std::unordered_set<std::string>{"x", "y", "z"});
}

TEST_CASE("PKBSP insertConstant test") {
    initSPTest("insertConstant.txt");
    REQUIRE(pkbQPS->getAllConstants() == std::unordered_set<std::string>{"1"});
}

TEST_CASE("PKBSP insertConstant negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getAllConstants().empty());
}

TEST_CASE("PKBSP insertConstant multiple test") {
    initSPTest("insertConstantMultiple.txt");
    REQUIRE(pkbQPS->getAllConstants() == std::unordered_set<std::string>{"1", "2", "3"});
}

TEST_CASE("PKBSP insertUses multiple test") {
    initSPTest("insertUsesMultiple.txt");
    REQUIRE(pkbQPS->getVariablesUsedInStatement(1) == std::unordered_set<std::string>{"assignVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(2) == std::unordered_set<std::string>{"printVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(3) == std::unordered_set<std::string>{"ifVar", "thenVar", "elseVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(4) == std::unordered_set<std::string>{"thenVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(5) == std::unordered_set<std::string>{"elseVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(6) == std::unordered_set<std::string>{"whileVar", "loopVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(7) == std::unordered_set<std::string>{"loopVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(8).empty());
    REQUIRE(pkbQPS->getVariablesUsedInProcedure("subProc") == std::unordered_set<std::string>{"procVar"});
}

TEST_CASE("PKBSP insertUses negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getStatementsThatUseAnyVariable().empty());
    REQUIRE(pkbQPS->getProceduresThatUseAnyVariable().empty());
}

TEST_CASE("PKBSP insertUses invalid test - assign reverse") {
    initSPTest("insertUsesInvalidAssignReverse.txt");
    REQUIRE(pkbQPS->getStatementsThatUseVariable("x").empty());
    REQUIRE(pkbQPS->getStatementsThatUseVariable("y") == std::unordered_set<int>{1});
}

TEST_CASE("PKBSP insertUses test one line") {
    initSPTest("insertUsesMultipleOneLine.txt");
    REQUIRE(pkbQPS->getVariablesUsedInStatement(1) == std::unordered_set<std::string>{"assignVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(2) == std::unordered_set<std::string>{"printVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(3) == std::unordered_set<std::string>{"ifVar", "thenVar", "elseVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(4) == std::unordered_set<std::string>{"thenVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(5) == std::unordered_set<std::string>{"elseVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(6) == std::unordered_set<std::string>{"whileVar", "loopVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(7) == std::unordered_set<std::string>{"loopVar"});
    REQUIRE(pkbQPS->getVariablesUsedInStatement(8).empty());
    REQUIRE(pkbQPS->getVariablesUsedInProcedure("subProc") == std::unordered_set<std::string>{"procVar"});
}

TEST_CASE("PKBSP insertModifies multiple test") {
    initSPTest("insertModifiesMultiple.txt");
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(1) == std::unordered_set<std::string>{"modifyVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(2) == std::unordered_set<std::string>{"readVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(3) == std::unordered_set<std::string>{"thenVar", "elseVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(4) == std::unordered_set<std::string>{"thenVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(5) == std::unordered_set<std::string>{"elseVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(6) == std::unordered_set<std::string>{"loopVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(7) == std::unordered_set<std::string>{"loopVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(8).empty());
    REQUIRE(pkbQPS->getVariablesModifiedInProcedure("subProc") == std::unordered_set<std::string>{"procVar"});
}

TEST_CASE("PKBSP insertModifies negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getStatementsThatModifyAnyVariable().empty());
    REQUIRE(pkbQPS->getProceduresThatModifyAnyVariable().empty());
}

TEST_CASE("PKBSP insertModifies invalid test - assign reverse") {
    initSPTest("insertModifiesInvalidAssignReverse.txt");
    REQUIRE(pkbQPS->getStatementsThatModifyVariable("y").empty());
    REQUIRE(pkbQPS->getStatementsThatModifyVariable("x") == std::unordered_set<int>{1});
}

TEST_CASE("PKBSP insertModifies multiple test one line") {
    initSPTest("insertModifiesMultipleOneLine.txt");
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(1) == std::unordered_set<std::string>{"modifyVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(2) == std::unordered_set<std::string>{"readVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(3) == std::unordered_set<std::string>{"thenVar", "elseVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(4) == std::unordered_set<std::string>{"thenVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(5) == std::unordered_set<std::string>{"elseVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(6) == std::unordered_set<std::string>{"loopVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(7) == std::unordered_set<std::string>{"loopVar"});
    REQUIRE(pkbQPS->getVariablesModifiedInStatement(8).empty());
    REQUIRE(pkbQPS->getVariablesModifiedInProcedure("subProc") == std::unordered_set<std::string>{"procVar"});
}

TEST_CASE("PKBSP insertFollows multiple test") {
    initSPTest("insertFollowsMultiple.txt");
    REQUIRE(pkbQPS->getAllWithFollows() == std::unordered_set<int>{1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 13, 16, 17, 18, 20, 21, 23, 24, 26, 27});
    REQUIRE(pkbQPS->getFollows(1, true) == std::unordered_set<int>{2});
    REQUIRE(pkbQPS->getFollows(1, false) == std::unordered_set<int>{2, 3, 4, 8, 15});
    REQUIRE(pkbQPS->getFollows(2, true) == std::unordered_set<int>{3});
    REQUIRE(pkbQPS->getFollows(2, false) == std::unordered_set<int>{3, 4, 8, 15});
    REQUIRE(pkbQPS->getFollows(3, true) == std::unordered_set<int>{4});
    REQUIRE(pkbQPS->getFollows(3, false) == std::unordered_set<int>{4, 8, 15});
    REQUIRE(pkbQPS->getFollows(4, true) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getFollows(4, false) == std::unordered_set<int>{8, 15});
    REQUIRE(pkbQPS->getFollows(5, true) == std::unordered_set<int>{6});
    REQUIRE(pkbQPS->getFollows(5, false) == std::unordered_set<int>{6, 7});
    REQUIRE(pkbQPS->getFollows(6, true) == std::unordered_set<int>{7});
    REQUIRE(pkbQPS->getFollows(6, false) == std::unordered_set<int>{7});
    REQUIRE(pkbQPS->getFollows(7, true).empty());
    REQUIRE(pkbQPS->getFollows(7, false).empty());
    REQUIRE(pkbQPS->getFollows(8, true) == std::unordered_set<int>{ 15 });
    REQUIRE(pkbQPS->getFollows(8, false) == std::unordered_set<int>{ 15 });
    REQUIRE(pkbQPS->getFollows(9, true) == std::unordered_set<int>{10});
    REQUIRE(pkbQPS->getFollows(9, false) == std::unordered_set<int>{10, 11});
    REQUIRE(pkbQPS->getFollows(10, true) == std::unordered_set<int>{11});
    REQUIRE(pkbQPS->getFollows(10, false) == std::unordered_set<int>{11});
    REQUIRE(pkbQPS->getFollows(11, true).empty());
    REQUIRE(pkbQPS->getFollows(11, false).empty());
    REQUIRE(pkbQPS->getFollows(12, true) == std::unordered_set<int>{13});
    REQUIRE(pkbQPS->getFollows(12, false) == std::unordered_set<int>{13, 14});
    REQUIRE(pkbQPS->getFollows(13, true) == std::unordered_set<int>{14});
    REQUIRE(pkbQPS->getFollows(13, false) == std::unordered_set<int>{14});
    REQUIRE(pkbQPS->getFollows(14, true).empty());
    REQUIRE(pkbQPS->getFollows(14, true).empty());
    REQUIRE(pkbQPS->getFollows(15, true).empty());
    REQUIRE(pkbQPS->getFollows(15, false).empty());
    REQUIRE(pkbQPS->getFollows(16, true) == std::unordered_set<int>{17});
    REQUIRE(pkbQPS->getFollows(16, false) == std::unordered_set<int>{17, 18, 19});
    REQUIRE(pkbQPS->getFollows(17, true) == std::unordered_set<int>{18});
    REQUIRE(pkbQPS->getFollows(17, false) == std::unordered_set<int>{18, 19});
    REQUIRE(pkbQPS->getFollows(18, true) == std::unordered_set<int>{19});
    REQUIRE(pkbQPS->getFollows(18, false) == std::unordered_set<int>{19});
    REQUIRE(pkbQPS->getFollows(19, true).empty());
    REQUIRE(pkbQPS->getFollows(19, false).empty());
    REQUIRE(pkbQPS->getFollows(20, true) == std::unordered_set<int>{21});
    REQUIRE(pkbQPS->getFollows(20, false) == std::unordered_set<int>{21, 22});
    REQUIRE(pkbQPS->getFollows(21, true) == std::unordered_set<int>{22});
    REQUIRE(pkbQPS->getFollows(21, false) == std::unordered_set<int>{22});
    REQUIRE(pkbQPS->getFollows(22, true).empty());
    REQUIRE(pkbQPS->getFollows(22, false).empty());
    REQUIRE(pkbQPS->getFollows(23, true) == std::unordered_set<int>{24});
    REQUIRE(pkbQPS->getFollows(23, false) == std::unordered_set<int>{24, 25});
    REQUIRE(pkbQPS->getFollows(24, true) == std::unordered_set<int>{25});
    REQUIRE(pkbQPS->getFollows(24, false) == std::unordered_set<int>{25});
    REQUIRE(pkbQPS->getFollows(25, true).empty());
    REQUIRE(pkbQPS->getFollows(25, false).empty());
    REQUIRE(pkbQPS->getFollows(26, true) == std::unordered_set<int>{27});
    REQUIRE(pkbQPS->getFollows(26, false) == std::unordered_set<int>{27, 28});
    REQUIRE(pkbQPS->getFollows(27, true) == std::unordered_set<int>{28});
    REQUIRE(pkbQPS->getFollows(27, false) == std::unordered_set<int>{28});
    REQUIRE(pkbQPS->getFollows(28, true).empty());
    REQUIRE(pkbQPS->getFollows(28, false).empty());
}

TEST_CASE("PKBSP insertFollows negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getAllWithFollows().empty());
}

TEST_CASE("PKBSP insertParents multiple test") {
    initSPTest("insertParentsMultiple.txt");
    REQUIRE(pkbQPS->getAllWithChild() == std::unordered_set<int>{4, 8, 15, 19});
    REQUIRE(pkbQPS->getAllWithParent() == std::unordered_set<int>{5, 6, 7, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28});
    REQUIRE(pkbQPS->getChild(1, true).empty());
    REQUIRE(pkbQPS->getChild(1, false).empty());
    REQUIRE(pkbQPS->getChild(4, true) == std::unordered_set<int>{5, 6, 7});
    REQUIRE(pkbQPS->getChild(4, false) == std::unordered_set<int>{5, 6, 7});
    REQUIRE(pkbQPS->getChild(5, true).empty());
    REQUIRE(pkbQPS->getChild(5, false).empty());
    REQUIRE(pkbQPS->getChild(5, true).empty());
    REQUIRE(pkbQPS->getChild(5, false).empty());
    REQUIRE(pkbQPS->getChild(8, true) == std::unordered_set<int>{9, 10, 11, 12, 13, 14});
    REQUIRE(pkbQPS->getChild(8, false) == std::unordered_set<int>{9, 10, 11, 12, 13, 14});
    REQUIRE(pkbQPS->getChild(9, true).empty());
    REQUIRE(pkbQPS->getChild(9, false).empty());
    REQUIRE(pkbQPS->getChild(12, true).empty());
    REQUIRE(pkbQPS->getChild(12, false).empty());
    REQUIRE(pkbQPS->getChild(15, true) == std::unordered_set<int>{16, 17, 18, 19, 26, 27, 28});
    REQUIRE(pkbQPS->getChild(15, false) == std::unordered_set<int>{16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28});
    REQUIRE(pkbQPS->getChild(16, true).empty());
    REQUIRE(pkbQPS->getChild(16, false).empty());
    REQUIRE(pkbQPS->getChild(19, true) == std::unordered_set<int>{20, 21, 22, 23, 24, 25});
    REQUIRE(pkbQPS->getChild(19, false) == std::unordered_set<int>{20, 21, 22, 23, 24, 25});
    REQUIRE(pkbQPS->getChild(20, true).empty());
    REQUIRE(pkbQPS->getChild(20, false).empty());
    REQUIRE(pkbQPS->getChild(23, true).empty());
    REQUIRE(pkbQPS->getChild(23, false).empty());
    REQUIRE(pkbQPS->getChild(26, true).empty());
    REQUIRE(pkbQPS->getChild(27, false).empty());

    REQUIRE(pkbQPS->getParent(1, true).empty());
    REQUIRE(pkbQPS->getParent(1, false).empty());
    REQUIRE(pkbQPS->getParent(2, true).empty());
    REQUIRE(pkbQPS->getParent(2, false).empty());
    REQUIRE(pkbQPS->getParent(4, true).empty());
    REQUIRE(pkbQPS->getParent(4, false).empty());
    REQUIRE(pkbQPS->getParent(5, true) == std::unordered_set<int>{4});
    REQUIRE(pkbQPS->getParent(5, false) == std::unordered_set<int>{4});
    REQUIRE(pkbQPS->getParent(6, true) == std::unordered_set<int>{4});
    REQUIRE(pkbQPS->getParent(6, false) == std::unordered_set<int>{4});
    REQUIRE(pkbQPS->getParent(7, true) == std::unordered_set<int>{4});
    REQUIRE(pkbQPS->getParent(7, false) == std::unordered_set<int>{4});
    REQUIRE(pkbQPS->getParent(8, true).empty());
    REQUIRE(pkbQPS->getParent(8, false).empty());
    REQUIRE(pkbQPS->getParent(9, true) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(9, false) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(10, true) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(10, false) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(11, true) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(11, false) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(12, true) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(12, false) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(13, true) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(13, false) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(14, true) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(14, false) == std::unordered_set<int>{8});
    REQUIRE(pkbQPS->getParent(15, true).empty());
    REQUIRE(pkbQPS->getParent(15, false).empty());
    REQUIRE(pkbQPS->getParent(16, true) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(16, false) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(17, true) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(17, false) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(19, true) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(19, false) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(20, true) == std::unordered_set<int>{19});
    REQUIRE(pkbQPS->getParent(20, false) == std::unordered_set<int>{19, 15});
    REQUIRE(pkbQPS->getParent(21, true) == std::unordered_set<int>{19});
    REQUIRE(pkbQPS->getParent(21, false) == std::unordered_set<int>{19, 15});
    REQUIRE(pkbQPS->getParent(22, true) == std::unordered_set<int>{19});
    REQUIRE(pkbQPS->getParent(22, false) == std::unordered_set<int>{19, 15});
    REQUIRE(pkbQPS->getParent(23, true) == std::unordered_set<int>{19});
    REQUIRE(pkbQPS->getParent(23, false) == std::unordered_set<int>{19, 15});
    REQUIRE(pkbQPS->getParent(24, true) == std::unordered_set<int>{19});
    REQUIRE(pkbQPS->getParent(24, false) == std::unordered_set<int>{19, 15});
    REQUIRE(pkbQPS->getParent(25, true) == std::unordered_set<int>{19});
    REQUIRE(pkbQPS->getParent(25, false) == std::unordered_set<int>{19, 15});
    REQUIRE(pkbQPS->getParent(26, true) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(26, false) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(27, true) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(27, false) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(28, true) == std::unordered_set<int>{15});
    REQUIRE(pkbQPS->getParent(28, false) == std::unordered_set<int>{15});
}

TEST_CASE("PKBSP insertParents negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE(pkbQPS->getAllWithParent().empty());
    REQUIRE(pkbQPS->getAllWithChild().empty());
}

TEST_CASE("PKBSP insertPattern negative test") {
    initSPTest("insertNegative.txt");
    REQUIRE_FALSE(pkbQPS->hasMatchingPattern(1, std::vector<std::string>{""}, true));
    REQUIRE_FALSE(pkbQPS->hasMatchingPattern(1, std::vector<std::string>{""}, false));
    REQUIRE_FALSE(pkbQPS->hasMatchingPattern(1, std::vector<std::string>{"+"}, true));
    REQUIRE_FALSE(pkbQPS->hasMatchingPattern(1, std::vector<std::string>{"+"}, false));
}

TEST_CASE("PKBSP insertPattern multiple test") {
    initSPTest("insertPatternsMultiple.txt");
    REQUIRE(pkbQPS->hasMatchingPattern(1, std::vector<std::string>{"a", "a", "+", "b", "+", "c", "d", "/", "e", "*", "-", "a", "b", "+", "c", "d", "/", "e", "*", "b", "+", "-", "+"}, true));
    REQUIRE(pkbQPS->hasMatchingPattern(3, std::vector<std::string>{"a", "a", "+", "b", "+", "c", "d", "/", "e", "*", "-", "a", "b", "+", "c", "d", "/", "e", "*", "b", "+", "-", "+"}, true));
    REQUIRE(pkbQPS->hasMatchingPattern(5, std::vector<std::string>{"a", "a", "+", "b", "+", "c", "d", "/", "e", "*", "-", "a", "b", "+", "c", "d", "/", "e", "*", "b", "+", "-", "+"}, true));
    REQUIRE(pkbQPS->hasMatchingPattern(6, std::vector<std::string>{"a", "a", "+", "b", "+", "c", "d", "/", "e", "*", "-", "a", "b", "+", "c", "d", "/", "e", "*", "b", "+", "-", "+"}, true));
    REQUIRE(pkbQPS->hasMatchingPattern(9, std::vector<std::string>{"a", "a", "+", "b", "+", "c", "d", "/", "e", "*", "-", "a", "b", "+", "c", "d", "/", "e", "*", "b", "+", "-", "+"}, true));
}

TEST_CASE("PKBSP insertNext multiple test") {
    initSPTest("insertNextMultiple.txt");
    REQUIRE(pkbQPS->getNext(1) == std::unordered_set<int>{2});
    REQUIRE(pkbQPS->getNext(2) == std::unordered_set<int>{3});
    REQUIRE(pkbQPS->getNext(3) == std::unordered_set<int>{6, 4});
    REQUIRE(pkbQPS->getNext(4) == std::unordered_set<int>{5});
    REQUIRE(pkbQPS->getNext(6) == std::unordered_set<int>{7});
    REQUIRE(pkbQPS->getNext(7) == std::unordered_set<int>{2});
    REQUIRE(pkbQPS->getNext(5) == std::unordered_set<int>{2});

    REQUIRE(pkbQPS->getNext(8) == std::unordered_set<int>{9});
    REQUIRE(pkbQPS->getNext(10).empty());
}

TEST_CASE("PKBSP insertIfPattern multiple test") {
    initSPTest("insertIfPattern.txt");
    REQUIRE(pkbQPS->getAllIfs() == std::unordered_set<int>{1, 3, 7});
    REQUIRE(pkbQPS->getIfsThatUseVariable("x") == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getIfsThatUseVariable("y") == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getIfsThatUseVariable("g") == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getIfsThatUseVariable("h") == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getIfsThatUseVariable("e") == std::unordered_set<int>{3});
    REQUIRE(pkbQPS->getIfsThatUseVariable("f") == std::unordered_set<int>{3});
    REQUIRE(pkbQPS->getIfsThatUseVariable("a") == std::unordered_set<int>{7});
    REQUIRE(pkbQPS->getIfsThatUseVariable("b") == std::unordered_set<int>{7});
}

TEST_CASE("PKBSP insertWhilePattern multiple test") {
    initSPTest("insertWhilePattern.txt");
    REQUIRE(pkbQPS->getAllWhiles() == std::unordered_set<int>{1, 3, 4, 5});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("x") == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("y") == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("g") == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("h") == std::unordered_set<int>{1});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("a") == std::unordered_set<int>{3});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("b") == std::unordered_set<int>{3});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("c") == std::unordered_set<int>{4});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("d") == std::unordered_set<int>{4});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("e") == std::unordered_set<int>{5});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("f") == std::unordered_set<int>{5});
    REQUIRE(pkbQPS->getWhilesThatUseVariable("i").empty());
}

// Skipping Affects tests as there are no Affects related methods in PKB to test with.