#include "catch.hpp"
#include "stubs/PKBEntityStub.h"
#include "source_processor/design_extractor/extractors/EntityExtractor.h"
#include "source_processor/ast/expressions/VariableNode.h"
#include "source_processor/ast/expressions/ConstantNode.h"

TEST_CASE("Entity Extraction Test") {
    SECTION("Program with no procedure") {
        std::shared_ptr<PKBEntityStub> pkb = std::make_shared<PKBEntityStub>();
        std::shared_ptr<EntityExtractor> extractor = std::make_shared<EntityExtractor>(pkb);

        std::string procedureName = "empty";
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        REQUIRE_THROWS(extractor->extract(program));
    }

    SECTION("Program with empty procedure") {
        std::shared_ptr<PKBEntityStub> pkb = std::make_shared<PKBEntityStub>();
        std::shared_ptr<EntityExtractor> extractor = std::make_shared<EntityExtractor>(pkb);

        std::string procedureName = "empty";
        auto stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>();
        std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->procedures.size() == 1);
        REQUIRE(pkb->variables.size() == 0);
        REQUIRE(pkb->statements.size() == 0);
        REQUIRE(pkb->readStatements.size() == 0);
        REQUIRE(pkb->printStatements.size() == 0);
        REQUIRE(pkb->readVariables.size() == 0);
        REQUIRE(pkb->printVariables.size() == 0);
    }

    SECTION("Read and print statement test") {
        std::shared_ptr<PKBEntityStub> pkb = std::make_shared<PKBEntityStub>();
        std::shared_ptr<EntityExtractor> extractor = std::make_shared<EntityExtractor>(pkb);

        std::string procedureName = "testProc";
        std::string printVarName = "testPrintVar";
        std::string readVarName = "testReadVar";

        std::shared_ptr<VariableNode> printVar = std::make_shared<VariableNode>(printVarName, 1);
        std::shared_ptr<VariableNode> readVar = std::make_shared<VariableNode>(readVarName, 2);

        std::shared_ptr<PrintNode> printNode = std::make_shared<PrintNode>(printVar, 1);
        std::shared_ptr<ReadNode> readNode = std::make_shared<ReadNode>(readVar, 2);

        auto stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{printNode, readNode});
        std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        // Assertions to check all entities are correctly extracted
        REQUIRE(pkb->procedures.size() == 1);
        REQUIRE(pkb->variables.size() == 2);
        REQUIRE(pkb->statements.size() == 2);
        REQUIRE(pkb->readStatements.size() == 1);
        REQUIRE(pkb->printStatements.size() == 1);
        REQUIRE(pkb->readVariables.size() == 1);
        REQUIRE(pkb->printVariables.size() == 1);
        REQUIRE(pkb->procedures.find(procedureName) != pkb->procedures.end());
        REQUIRE(pkb->variables.find(printVarName) != pkb->variables.end());
        REQUIRE(pkb->variables.find(readVarName) != pkb->variables.end());
        REQUIRE(pkb->statements.find(1) != pkb->statements.end());
    }

    SECTION("AssignStatement Test") {
        std::shared_ptr<PKBEntityStub> pkb = std::make_shared<PKBEntityStub>();
        std::shared_ptr<EntityExtractor> extractor = std::make_shared<EntityExtractor>(pkb);

        std::string procedureName = "testProc";
        std::string assignLhsVarName = "testAssignLhsVar";
        std::string assignRhsVarName = "testAssignRhsVar";
        std::string assignRhsConst = "420";

        std::shared_ptr<VariableNode> assignLhsVar = std::make_shared<VariableNode>(assignLhsVarName, 1);

        // Test assign const e.g. testAssignLhsVar = 420
        std::vector<std::string> constants{ assignRhsConst };
        std::vector<std::string> variables{};
        auto assignRhsExpressionNode = std::make_shared<ExpressionNode>("420", variables, constants, 1);
        std::shared_ptr<AssignNode> assignNode = std::make_shared<AssignNode>(assignLhsVar, assignRhsExpressionNode, 1);
        auto stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ assignNode });
        std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->procedures.size() == 1);
        REQUIRE(pkb->variables.size() == 1);
        REQUIRE(pkb->statements.size() == 1);
        REQUIRE(pkb->constants.size() == 1);
        REQUIRE(pkb->assignStatements.size() == 1);
        REQUIRE(pkb->assignVariables.size() == 1);
        
        // Test assign var e.g. testAssignLhsVar = testAssignRhsVar
        pkb = std::make_shared<PKBEntityStub>();
        extractor = std::make_shared<EntityExtractor>(pkb);

        constants = {};
        variables = { assignRhsVarName };
        assignRhsExpressionNode = std::make_shared<ExpressionNode>("testAssignRhsVar", variables, constants, 1);
        assignNode = std::make_shared<AssignNode>(assignLhsVar, assignRhsExpressionNode, 1);
        stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ assignNode });
        procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->procedures.size() == 1);
        REQUIRE(pkb->variables.size() == 2);
        REQUIRE(pkb->constants.size() == 0);
        REQUIRE(pkb->statements.size() == 1);
        REQUIRE(pkb->assignStatements.size() == 1);
        REQUIRE(pkb->assignVariables.size() == 1);

        // Test assign expression e.g. testAssignLhsVar = testAssignRhsVar + 420
        pkb = std::make_shared<PKBEntityStub>();
        extractor = std::make_shared<EntityExtractor>(pkb);

        constants = { assignRhsConst };
        variables = { assignRhsVarName };
        assignRhsExpressionNode = std::make_shared<ExpressionNode>("testAssignRhsVar + 420", variables, constants, 1);
        assignNode = std::make_shared<AssignNode>(assignLhsVar, assignRhsExpressionNode, 1);
        stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ assignNode });
        procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->procedures.size() == 1);
        REQUIRE(pkb->variables.size() == 2);
        REQUIRE(pkb->constants.size() == 1);
        REQUIRE(pkb->statements.size() == 1);
        REQUIRE(pkb->assignStatements.size() == 1);
        REQUIRE(pkb->assignVariables.size() == 1);
    }

    SECTION("IfStatement Test") {
        std::shared_ptr<PKBEntityStub> pkb = std::make_shared<PKBEntityStub>();
        std::shared_ptr<EntityExtractor> extractor = std::make_shared<EntityExtractor>(pkb);

        std::string procedureName = "testProc";
        std::string ifVarName1 = "a";
        std::string ifVarName2 = "b";
        std::string ifConst1 = "420";
        std::string ifConst2 = "90";

        // Test if const e.g. if (a + 420 == b + 90) then {} else {}
        std::vector<std::string> constants{ ifConst1, ifConst2 };
        std::vector<std::string> variables{ ifVarName1, ifVarName2 };
        auto ifConditionalNode = std::make_shared<ConditionNode>(variables, constants, "==");
        auto emptyStmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{});
        std::shared_ptr<IfNode> ifNode = std::make_shared<IfNode>(ifConditionalNode, emptyStmtList, emptyStmtList, 1);

        auto stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ ifNode });
        std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->procedures.size() == 1);
        REQUIRE(pkb->variables.size() == 2);
        REQUIRE(pkb->constants.size() == 2);
        REQUIRE(pkb->statements.size() == 1);
        REQUIRE(pkb->ifStatements.size() == 1);
    }

    SECTION("WhileStatement Test") {
        std::shared_ptr<PKBEntityStub> pkb = std::make_shared<PKBEntityStub>();
        std::shared_ptr<EntityExtractor> extractor = std::make_shared<EntityExtractor>(pkb);

        std::string procedureName = "testProc";
        std::string whileVarName1 = "a";
        std::string whileVarName2 = "b";
        std::string whileConst1 = "420";
        std::string whileConst2 = "90";

        // Test while const e.g. while (a + 420 == b + 90) {}
        std::vector<std::string> constants{ whileConst1, whileConst2 };
        std::vector<std::string> variables{ whileVarName1, whileVarName2 };
        auto whileConditionalNode = std::make_shared<ConditionNode>(variables, constants, "==");
        auto emptyStmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{});
        std::shared_ptr<WhileNode> whileNode = std::make_shared<WhileNode>(whileConditionalNode, emptyStmtList, 1);

        auto stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ whileNode });
        std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->procedures.size() == 1);
        REQUIRE(pkb->variables.size() == 2);
        REQUIRE(pkb->constants.size() == 2);
        REQUIRE(pkb->statements.size() == 1);
        REQUIRE(pkb->whileStatements.size() == 1);
    }

    SECTION("Single procedure with nesting") {
        std::shared_ptr<PKBEntityStub> pkb = std::make_shared<PKBEntityStub>();
        std::shared_ptr<EntityExtractor> extractor = std::make_shared<EntityExtractor>(pkb);

        std::string procedureName = "testProc";
        std::string printVarName = "a";
        std::string readVarName = "b";
        std::string ifConst1 = "20";
        std::string assignLhsVarName = "c";
        std::string assignRhsVarName = "d";
        std::string assignRhsConst = "420";
        std::string whileVarName = "e";
        std::string whileConst = "2";
        std::string printVarName2 = "f";
        std::string ifConst2 = "3";
        std::string ifVarName = "g";
        std::string assignLhsVarName2 = "h";
        std::string assignLhsVarName3 = "j";
        std::string assignRhsVarName2 = "i";
        std::string assignRhsVarName3 = "k";

        // Generate nodes for 
        // procedure testProc {
        // 1.    print a;
        // 2.    read b;
        // 3.    if (a == 20) {
        // 4.        c = d + 420;
        //     } else {
        // 5.        while (e != 2) {
        // 6.		       print f;
        // 7.            if (g <= 3) {
        // 8.                h = i + 420; 
        //             } else {
        // 9.                j = k + 420;
        //             }
        //         }
        //     }
        // 10.    read b;
        // }

        std::shared_ptr<VariableNode> printVar = std::make_shared<VariableNode>(printVarName, 1);
        std::shared_ptr<PrintNode> printNode = std::make_shared<PrintNode>(printVar, 1);

        std::shared_ptr<VariableNode> readVar = std::make_shared<VariableNode>(readVarName, 2);
        std::shared_ptr<ReadNode> readNode = std::make_shared<ReadNode>(readVar, 2);

        std::vector<std::string> constants{ ifConst1 };
        std::vector<std::string> variables{ printVarName };
        auto ifConditionalNode = std::make_shared<ConditionNode>(variables, constants, "==");

        std::shared_ptr<VariableNode> assignLhsVar = std::make_shared<VariableNode>(assignLhsVarName, 4);
        constants = { assignRhsConst };
        variables = { assignRhsVarName };
        auto assignRhsExpressionNode = std::make_shared<ExpressionNode>("d + 420", variables, constants, 4);
        std::shared_ptr<AssignNode> assignNode = std::make_shared<AssignNode>(assignLhsVar, assignRhsExpressionNode, 4);

        constants = { whileConst };
        variables = { whileVarName };
        auto whileConditionalNode = std::make_shared<ConditionNode>(variables, constants, "!=");

        std::shared_ptr<VariableNode> printVar2 = std::make_shared<VariableNode>(printVarName2, 6);
        std::shared_ptr<PrintNode> printNode2 = std::make_shared<PrintNode>(printVar2, 6);

        constants = { ifConst2 };
        variables = { ifVarName };
        auto ifConditionalNode2 = std::make_shared<ConditionNode>(variables, constants, "<=");
        std::shared_ptr<VariableNode> assignLhsVar2 = std::make_shared<VariableNode>(assignLhsVarName2, 8);
        std::shared_ptr<VariableNode> assignLhsVar3 = std::make_shared<VariableNode>(assignLhsVarName3, 9);
        variables = { assignRhsVarName2 };
        constants = { assignRhsConst };
        auto assignRhsExpressionNode2 = std::make_shared<ExpressionNode>("i + 420", variables, constants, 8);
        variables = { assignRhsVarName3 };
        auto assignRhsExpressionNode3 = std::make_shared<ExpressionNode>("k + 420", variables, constants, 9);
        std::shared_ptr<AssignNode> assignNode2 = std::make_shared<AssignNode>(assignLhsVar2, assignRhsExpressionNode2, 8);
        std::shared_ptr<AssignNode> assignNode3 = std::make_shared<AssignNode>(assignLhsVar3, assignRhsExpressionNode3, 9);

        auto thenStmt2 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ assignNode2 });
        auto elseStmt2 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ assignNode3 });
        std::shared_ptr<IfNode> ifNode2 = std::make_shared<IfNode>(ifConditionalNode2, thenStmt2, elseStmt2, 7);

        auto stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ printNode2, ifNode2 });
        std::shared_ptr<WhileNode> whileNode = std::make_shared<WhileNode>(whileConditionalNode, stmtList, 5);

        auto thenStmt = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ assignNode });
        auto elseStmt = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ whileNode });
        std::shared_ptr<IfNode> ifNode = std::make_shared<IfNode>(ifConditionalNode, thenStmt, elseStmt, 3);


        std::shared_ptr<VariableNode> readVar2 = std::make_shared<VariableNode>(readVarName, 10);
        std::shared_ptr<ReadNode> readNode2 = std::make_shared<ReadNode>(readVar, 10);

        stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ printNode, readNode, ifNode, readNode2 });
        std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->procedures.size() == 1);
        REQUIRE(pkb->variables.size() == 11);
        REQUIRE(pkb->constants.size() == 4);
        REQUIRE(pkb->statements.size() == 10);
        REQUIRE(pkb->readStatements.size() == 2);
        REQUIRE(pkb->printStatements.size() == 2);
        REQUIRE(pkb->readVariables.size() == 1);
        REQUIRE(pkb->printVariables.size() == 2);
        REQUIRE(pkb->ifStatements.size() == 2);
        REQUIRE(pkb->whileStatements.size() == 1);
        REQUIRE(pkb->assignStatements.size() == 3);
        REQUIRE(pkb->assignVariables.size() == 3);
        REQUIRE(pkb->patterns.size() == 3);
    }

    SECTION("Multi-procedure Test") {
        /*
            Generate nodes for the multi-procedure example in the course webiste:
            procedure main {
            01      flag = 0;
            02      call computeCentroid;
            03      call printResults;
                }
                procedure readPoint {
            04      read x;
            05      read y;
                }
                procedure printResults {
            06      print flag;
            07      print cenX;
            08      print cenY;
            09      print normSq;
                }
                procedure computeCentroid {
            10      count = 0;
            11      cenX = 0;
            12      cenY = 0;
            13      call readPoint;
            14      while ((x != 0) && (y != 0)) {
            15          count = count + 1;
            16          cenX = cenX + x;
            17          cenY = cenY + y;
            18          call readPoint;
                    }
            19      if (count == 0) then {
            20          flag = 1;
                    } else {
            21          cenX = cenX / count;
            22          cenY = cenY / count;
                    }
            23      normSq = cenX * cenX + cenY * cenY;
                }
        */

        std::shared_ptr<PKBEntityStub> pkb = std::make_shared<PKBEntityStub>();
        std::shared_ptr<EntityExtractor> extractor = std::make_shared<EntityExtractor>(pkb);

        std::string procedureName1 = "main";
        std::string procedureName2 = "readPoint";
        std::string procedureName3 = "printResults";
        std::string procedureName4 = "computeCentroid";

        std::string flagVarName = "flag";
        std::string xVarName = "x";
        std::string yVarName = "y";
        std::string countVarName = "count";
        std::string cenXVarName = "cenX";
        std::string cenYVarName = "cenY";
        std::string normSqVarName = "normSq";

        std::string zeroConst = "0";
        std::string oneConst = "1";

        std::shared_ptr<VariableNode> flagVar1 = std::make_shared<VariableNode>(flagVarName, 1);
        std::shared_ptr<VariableNode> flagVar2 = std::make_shared<VariableNode>(flagVarName, 6);
        std::shared_ptr<VariableNode> flagVar3 = std::make_shared<VariableNode>(flagVarName, 20);
        std::shared_ptr<VariableNode> xVar1 = std::make_shared<VariableNode>(xVarName, 4);
        std::shared_ptr<VariableNode> xVar2 = std::make_shared<VariableNode>(xVarName, 14);
        std::shared_ptr<VariableNode> xVar3 = std::make_shared<VariableNode>(xVarName, 16);
        std::shared_ptr<VariableNode> yVar1 = std::make_shared<VariableNode>(yVarName, 5);
        std::shared_ptr<VariableNode> yVar2 = std::make_shared<VariableNode>(yVarName, 14);
        std::shared_ptr<VariableNode> yVar3 = std::make_shared<VariableNode>(yVarName, 17);
        std::shared_ptr<VariableNode> countVar1 = std::make_shared<VariableNode>(countVarName, 10);
        std::shared_ptr<VariableNode> countVar2 = std::make_shared<VariableNode>(countVarName, 15);
        std::shared_ptr<VariableNode> countVar3 = std::make_shared<VariableNode>(countVarName, 19);
        std::shared_ptr<VariableNode> countVar4 = std::make_shared<VariableNode>(countVarName, 21);
        std::shared_ptr<VariableNode> countVar5 = std::make_shared<VariableNode>(countVarName, 22);
        std::shared_ptr<VariableNode> cenXVar1 = std::make_shared<VariableNode>(cenXVarName, 7);
        std::shared_ptr<VariableNode> cenXVar2 = std::make_shared<VariableNode>(cenXVarName, 11);
        std::shared_ptr<VariableNode> cenXVar3 = std::make_shared<VariableNode>(cenXVarName, 16);
        std::shared_ptr<VariableNode> cenXVar4 = std::make_shared<VariableNode>(cenXVarName, 21);
        std::shared_ptr<VariableNode> cenXVar5 = std::make_shared<VariableNode>(cenXVarName, 23);
        std::shared_ptr<VariableNode> cenYVar1 = std::make_shared<VariableNode>(cenYVarName, 8);
        std::shared_ptr<VariableNode> cenYVar2 = std::make_shared<VariableNode>(cenYVarName, 12);
        std::shared_ptr<VariableNode> cenYVar3 = std::make_shared<VariableNode>(cenYVarName, 17);
        std::shared_ptr<VariableNode> cenYVar4 = std::make_shared<VariableNode>(cenYVarName, 22);
        std::shared_ptr<VariableNode> cenYVar5 = std::make_shared<VariableNode>(cenYVarName, 23);
        std::shared_ptr<VariableNode> normSqVar1 = std::make_shared<VariableNode>(normSqVarName, 9);
        std::shared_ptr<VariableNode> normSqVar2 = std::make_shared<VariableNode>(normSqVarName, 23);

        std::shared_ptr<ConstantNode> zeroConstNode = std::make_shared<ConstantNode>(zeroConst);
        std::shared_ptr<ConstantNode> oneConstNode = std::make_shared<ConstantNode>(oneConst);

        std::shared_ptr<ExpressionNode> expressionNode1 = std::make_shared<ExpressionNode>("0", std::vector<std::string>{}, std::vector<std::string>{zeroConst}, 1);
        std::shared_ptr<AssignNode> assignNode1 = std::make_shared<AssignNode>(flagVar1, expressionNode1, 1);

        std::shared_ptr<ExpressionNode> expressionNode2 = std::make_shared<ExpressionNode>("0", std::vector<std::string>{}, std::vector<std::string>{zeroConst}, 10);
        std::shared_ptr<AssignNode> assignNode2 = std::make_shared<AssignNode>(countVar1, expressionNode2, 10);

        std::shared_ptr<ExpressionNode> expressionNode3 = std::make_shared<ExpressionNode>("0", std::vector<std::string>{}, std::vector<std::string>{zeroConst}, 11);
        std::shared_ptr<AssignNode> assignNode3 = std::make_shared<AssignNode>(cenXVar2, expressionNode3, 11);

        std::shared_ptr<ExpressionNode> expressionNode4 = std::make_shared<ExpressionNode>("0", std::vector<std::string>{}, std::vector<std::string>{zeroConst}, 12);
        std::shared_ptr<AssignNode> assignNode4 = std::make_shared<AssignNode>(cenYVar2, expressionNode4, 12);

        std::shared_ptr<ExpressionNode> expressionNode5 = std::make_shared<ExpressionNode>("count + 1", std::vector<std::string>{countVarName}, std::vector<std::string>{oneConst}, 15);
        std::shared_ptr<AssignNode> assignNode5 = std::make_shared<AssignNode>(countVar2, expressionNode5, 15);

        std::shared_ptr<ExpressionNode> expressionNode6 = std::make_shared<ExpressionNode>("cenX + x", std::vector<std::string>{cenXVarName, xVarName}, std::vector<std::string>{}, 16);
        std::shared_ptr<AssignNode> assignNode6 = std::make_shared<AssignNode>(cenXVar3, expressionNode6, 16);

        std::shared_ptr<ExpressionNode> expressionNode7 = std::make_shared<ExpressionNode>("cenY + y", std::vector<std::string>{cenYVarName, yVarName}, std::vector<std::string>{}, 17);
        std::shared_ptr<AssignNode> assignNode7 = std::make_shared<AssignNode>(cenYVar3, expressionNode7, 17);

        std::shared_ptr<ExpressionNode> expressionNode8 = std::make_shared<ExpressionNode>("1", std::vector<std::string>{}, std::vector<std::string>{oneConst}, 21);
        std::shared_ptr<AssignNode> assignNode8 = std::make_shared<AssignNode>(flagVar3, expressionNode8, 20);

        std::shared_ptr<ExpressionNode> expressionNode9 = std::make_shared<ExpressionNode>("cenX / count", std::vector<std::string>{cenXVarName, countVarName}, std::vector<std::string>{}, 21);
        std::shared_ptr<AssignNode> assignNode9 = std::make_shared<AssignNode>(cenXVar4, expressionNode9, 21);

        std::shared_ptr<ExpressionNode> expressionNode10 = std::make_shared<ExpressionNode>("cenY / count", std::vector<std::string>{cenYVarName, countVarName}, std::vector<std::string>{}, 22);
        std::shared_ptr<AssignNode> assignNode10 = std::make_shared<AssignNode>(cenYVar4, expressionNode10, 22);

        std::shared_ptr<ExpressionNode> expressionNode11 = std::make_shared<ExpressionNode>("cenX * cenX + cenY * cenY", std::vector<std::string>{cenYVarName, cenXVarName}, std::vector<std::string>{}, 23);
        std::shared_ptr<AssignNode> assignNode11 = std::make_shared<AssignNode>(normSqVar2, expressionNode11, 23);

        std::shared_ptr<ReadNode> readNode1 = std::make_shared<ReadNode>(xVar1, 4);
        std::shared_ptr<ReadNode> readNode2 = std::make_shared<ReadNode>(yVar1, 5);
        std::shared_ptr<PrintNode> printNode1 = std::make_shared<PrintNode>(flagVar2, 6);
        std::shared_ptr<PrintNode> printNode2 = std::make_shared<PrintNode>(cenXVar1, 7);
        std::shared_ptr<PrintNode> printNode3 = std::make_shared<PrintNode>(cenYVar1, 8);
        std::shared_ptr<PrintNode> printNode4 = std::make_shared<PrintNode>(normSqVar1, 9);

        auto stmtList2 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{readNode1, readNode2});
        std::shared_ptr<ProcedureNode> proc2 = std::make_shared<ProcedureNode>(procedureName2, stmtList2);

        auto stmtList3 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{printNode1, printNode2, printNode3, printNode4});
        std::shared_ptr<ProcedureNode> proc3 = std::make_shared<ProcedureNode>(procedureName3, stmtList3);

        auto thenStmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode8});
        auto elseStmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode9, assignNode10});
        std::shared_ptr<ConditionNode> ifConditionalNode = std::make_shared<ConditionNode>(std::vector<std::string>{countVarName}, std::vector<std::string>{zeroConst}, "==");
        std::shared_ptr<IfNode> ifNode1 = std::make_shared<IfNode>(ifConditionalNode, thenStmtList, elseStmtList, 19);

        std::shared_ptr<CallNode> callNode2 = std::make_shared<CallNode>(procedureName3, 3);
        std::shared_ptr<CallNode> callNode3 = std::make_shared<CallNode>(procedureName2, 13);
        std::shared_ptr<CallNode> callNode4 = std::make_shared<CallNode>(procedureName2, 18);

        std::shared_ptr<ConditionNode> whileConditionalNode = std::make_shared<ConditionNode>(std::vector<std::string>{xVarName, yVarName}, std::vector<std::string>{zeroConst}, "!=");
        auto whileStmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode5, assignNode6, assignNode7, callNode4});
        std::shared_ptr<WhileNode> whileNode = std::make_shared<WhileNode>(whileConditionalNode, whileStmtList, 14);

        auto stmtList4 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode2, assignNode3, assignNode4, callNode3, whileNode, ifNode1, assignNode11});
        std::shared_ptr<ProcedureNode> proc4 = std::make_shared<ProcedureNode>(procedureName4, stmtList4);

        std::shared_ptr<CallNode> callNode1 = std::make_shared<CallNode>(procedureName4, 2);
        auto stmtList1 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode1, callNode1, callNode2});
        std::shared_ptr<ProcedureNode> proc1 = std::make_shared<ProcedureNode>(procedureName1, stmtList1);

        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{proc1, proc2, proc3, proc4});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);
        extractor->extract(program);

        REQUIRE(pkb->procedures.size() == 4);
        REQUIRE(pkb->callStatements.size() == 4);
    }

    SECTION("Repeated procedure") {
        /*
            procedure A {
            01      call B;
            }

            procedure A {
                print x;
            }
        */

        std::shared_ptr<PKBEntityStub> pkb = std::make_shared<PKBEntityStub>();
        std::shared_ptr<EntityExtractor> extractor = std::make_shared<EntityExtractor>(pkb);

        std::string procedureName1 = "A";
        std::string procedureName2 = "A";

        std::string xVarName = "x";

        std::shared_ptr<VariableNode> xVar = std::make_shared<VariableNode>(xVarName, 2);
        std::shared_ptr<PrintNode> printNode = std::make_shared<PrintNode>(xVar, 2);

        std::shared_ptr<CallNode> callNode = std::make_shared<CallNode>(procedureName2, 1);
        auto stmtList1 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{callNode});
        std::shared_ptr<ProcedureNode> proc1 = std::make_shared<ProcedureNode>(procedureName1, stmtList1);

        auto stmtList2 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{printNode});
        std::shared_ptr<ProcedureNode> proc2 = std::make_shared<ProcedureNode>(procedureName2, stmtList2);

        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{proc1, proc2});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        REQUIRE_THROWS(extractor->extract(program));
    }
}