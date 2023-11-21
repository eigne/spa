#include "catch.hpp"
#include "stubs/PKBRelationshipStub.h"
#include "source_processor/design_extractor/extractors/RelationshipExtractor.h"
#include "source_processor/ast/expressions/ConstantNode.h"

TEST_CASE("Relationship Extractor Test") {
    SECTION("Program with no procedure") {
        std::shared_ptr<PKBRelationshipStub> pkb = std::make_shared<PKBRelationshipStub>();
        std::shared_ptr<RelationshipExtractor> extractor = std::make_shared<RelationshipExtractor>(pkb);

        std::string procedureName = "empty";
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        REQUIRE_THROWS(extractor->extract(program));
    }

    SECTION("Program with empty procedure") {
        std::shared_ptr<PKBRelationshipStub> pkb = std::make_shared<PKBRelationshipStub>();
        std::shared_ptr<RelationshipExtractor> extractor = std::make_shared<RelationshipExtractor>(pkb);

        std::string procedureName = "empty";
        auto stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>());
        std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->modifiesP.size() == 0);
        REQUIRE(pkb->usesP.size() == 0);
        REQUIRE(pkb->modifiesS.size() == 0);
        REQUIRE(pkb->usesS.size() == 0);
        REQUIRE(pkb->modifiesP.find(procedureName) == pkb->modifiesP.end());
        REQUIRE(pkb->usesP.find(procedureName) == pkb->usesP.end());
        REQUIRE(pkb->modifiesS.find(1) == pkb->modifiesS.end());
        REQUIRE(pkb->usesS.find(1) == pkb->usesS.end());
        REQUIRE(pkb->parents.size() == 0);
        REQUIRE(pkb->follows.size() == 0);
    }

    SECTION("Single Procedure no nesting") {
        std::shared_ptr<PKBRelationshipStub> pkb = std::make_shared<PKBRelationshipStub>();
        std::shared_ptr<RelationshipExtractor> extractor = std::make_shared<RelationshipExtractor>(pkb);

        std::string procedureName = "testProc";
        std::string printVarName = "testPrintVar";
        std::string readVarName = "testReadVar";
        std::string assignLhsVarName = "a";
        std::string assignRhsVarName = "b";
        std::string assignRhsConst = "420";
        std::string assignRhsConst2 = "42";

        std::shared_ptr<VariableNode> printVar = std::make_shared<VariableNode>(printVarName, 1);
        std::shared_ptr<VariableNode> readVar = std::make_shared<VariableNode>(readVarName, 2);
        std::shared_ptr<VariableNode> assignLhsVar = std::make_shared<VariableNode>(assignLhsVarName, 3);

        std::shared_ptr<PrintNode> printNode = std::make_shared<PrintNode>(printVar, 1);
        std::shared_ptr<ReadNode> readNode = std::make_shared<ReadNode>(readVar, 2);

        std::vector<std::string> assignRhsVarNames{assignRhsVarName, assignLhsVarName};
        std::vector<std::string> assignRhsConsts{assignRhsConst, assignRhsConst2};
        std::shared_ptr<ExpressionNode> assignRhs = std::make_shared<ExpressionNode>("a + 42 + b + 420", assignRhsVarNames, assignRhsConsts, 3);
        std::shared_ptr<AssignNode> assignNode = std::make_shared<AssignNode>(assignLhsVar, assignRhs, 3);

        auto stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{printNode, readNode, assignNode});
        std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->modifiesP[procedureName].size() == 2);
        REQUIRE(pkb->usesP[procedureName].size() == 3);
        REQUIRE(pkb->modifiesS.size() == 2);
        REQUIRE(pkb->usesS.size() == 2);
        REQUIRE(pkb->usesS[3].size() == 2);
        REQUIRE(pkb->modifiesP.find(procedureName) != pkb->modifiesP.end());
        REQUIRE(pkb->usesP.find(procedureName) != pkb->usesP.end());
        REQUIRE(pkb->modifiesS.find(2) != pkb->modifiesS.end());
        REQUIRE(pkb->usesS.find(1) != pkb->usesS.end());
        REQUIRE(pkb->follows.size() == 2);
        REQUIRE(pkb->follows.at(1).size() == 1);
        REQUIRE(pkb->parents.size() == 0);
    }

    SECTION("Single procedure with nesting") {
        std::shared_ptr<PKBRelationshipStub> pkb = std::make_shared<PKBRelationshipStub>();
        std::shared_ptr<RelationshipExtractor> extractor = std::make_shared<RelationshipExtractor>(pkb);

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
        // 10.               print a;
        //             }
        //         }
        //     }
        // 11.    read b;
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

        std::shared_ptr<VariableNode> printVar3 = std::make_shared<VariableNode>(printVarName, 10);
        std::shared_ptr<PrintNode> printNode3 = std::make_shared<PrintNode>(printVar3, 10);

        auto thenStmt2 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ assignNode2 });
        auto elseStmt2 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ assignNode3, printNode3 });
        std::shared_ptr<IfNode> ifNode2 = std::make_shared<IfNode>(ifConditionalNode2, thenStmt2, elseStmt2, 7);

        auto stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ printNode2, ifNode2 });
        std::shared_ptr<WhileNode> whileNode = std::make_shared<WhileNode>(whileConditionalNode, stmtList, 5);

        auto thenStmt = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ assignNode });
        auto elseStmt = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ whileNode });
        std::shared_ptr<IfNode> ifNode = std::make_shared<IfNode>(ifConditionalNode, thenStmt, elseStmt, 3);


        std::shared_ptr<VariableNode> readVar2 = std::make_shared<VariableNode>(readVarName, 11);
        std::shared_ptr<ReadNode> readNode2 = std::make_shared<ReadNode>(readVar, 11);

        stmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ printNode, readNode, ifNode, readNode2 });
        std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>(procedureName, stmtList);
        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{procedure});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->modifiesP[procedureName].size() == 4);
        REQUIRE(pkb->usesP[procedureName].size() == 7);
        REQUIRE(pkb->modifiesS.size() == 7);
        REQUIRE(pkb->usesS.size() == 9);
        REQUIRE(pkb->follows[1] == std::unordered_set{2});
        REQUIRE(pkb->follows[9].size() == 1);
        REQUIRE(pkb->follows[9].find(10) != pkb->follows[9].end());
        REQUIRE(pkb->follows[10].size() == 0);
        REQUIRE(pkb->followsT[1].size() == 3);
        REQUIRE(pkb->parents.size() == 3);
        REQUIRE(pkb->parents[3] == std::unordered_set{4, 5});
        REQUIRE(pkb->parents[5] == std::unordered_set{6, 7});
        REQUIRE(pkb->parents[7] == std::unordered_set{ 8, 9, 10 });
        REQUIRE(pkb->parentsT[5] == std::unordered_set{ 6, 7, 8, 9, 10 });

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

        std::shared_ptr<PKBRelationshipStub> pkb = std::make_shared<PKBRelationshipStub>();
        std::shared_ptr<RelationshipExtractor> extractor = std::make_shared<RelationshipExtractor>(pkb);

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

        REQUIRE(pkb->usesP[procedureName1].count("cenX") != 0);
        REQUIRE(pkb->usesP[procedureName1].count("flag") != 0);
        REQUIRE(pkb->usesP[procedureName1].count("cenY") != 0);
        REQUIRE(pkb->usesP[procedureName1].count("count") != 0);
        REQUIRE(pkb->usesS[3].count("normSq") != 0);
        REQUIRE(pkb->usesS[2].count("count") != 0);

        REQUIRE(pkb->modifiesP[procedureName1].count("x") != 0);
        REQUIRE(pkb->modifiesP[procedureName1].count("y") != 0);
        REQUIRE(pkb->modifiesS[13].count("x") != 0);

        REQUIRE(pkb->calls["main"].size() == 2);
        REQUIRE(pkb->calls["computeCentroid"].size() == 1);
        REQUIRE(pkb->calls["printResults"].size() == 0);
        REQUIRE(pkb->calls["readPoint"].size() == 0);

        REQUIRE(pkb->callsT["main"].size() == 3);
        REQUIRE(pkb->callsT["computeCentroid"].size() == 1);
        REQUIRE(pkb->callsT["printResults"].size() == 0);
        REQUIRE(pkb->callsT["readPoint"].size() == 0);
    }

    SECTION("Self-call test") {
        /*
            procedure A {
               call A;
            }
        */

        std::shared_ptr<PKBRelationshipStub> pkb = std::make_shared<PKBRelationshipStub>();
        std::shared_ptr<RelationshipExtractor> extractor = std::make_shared<RelationshipExtractor>(pkb);

        std::string procedureName = "A";

        std::shared_ptr<CallNode> callNode = std::make_shared<CallNode>(procedureName, 1);

        std::shared_ptr<ProcedureNode> proc = std::make_shared<ProcedureNode>(procedureName, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{callNode}));

        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{proc}));

        REQUIRE_THROWS(extractor->extract(program));
    }

    SECTION("Non-existent call test") {
        /*
            procedure A {
                call B;
            }
        */

        std::shared_ptr<PKBRelationshipStub> pkb = std::make_shared<PKBRelationshipStub>();
        std::shared_ptr<RelationshipExtractor> extractor = std::make_shared<RelationshipExtractor>(pkb);

        std::string procedureName1 = "A";
        std::string procedureName2 = "B";

        std::shared_ptr<ProcedureNode> proc1 = std::make_shared<ProcedureNode>(procedureName2, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{}));
        std::shared_ptr<CallNode> callNode = std::make_shared<CallNode>(procedureName2, 1);

        std::shared_ptr<ProcedureNode> proc2 = std::make_shared<ProcedureNode>(procedureName1, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{callNode}));

        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{proc2}));
        
        REQUIRE_THROWS(extractor->extract(program));
    }

    SECTION("Cyclic call test") {
        /*
            procedure A {
                call B;
            }

            procedure B {
                call C;
            }

            procedure C {
                call A;
            }
        */

        std::shared_ptr<PKBRelationshipStub> pkb = std::make_shared<PKBRelationshipStub>();
        std::shared_ptr<RelationshipExtractor> extractor = std::make_shared<RelationshipExtractor>(pkb);

        std::string procedureName1 = "A";
        std::string procedureName2 = "B";
        std::string procedureName3 = "C";

        std::shared_ptr<CallNode> callNode1 = std::make_shared<CallNode>(procedureName2, 1);
        std::shared_ptr<CallNode> callNode2 = std::make_shared<CallNode>(procedureName3, 2);
        std::shared_ptr<CallNode> callNode3 = std::make_shared<CallNode>(procedureName1, 3);

        std::shared_ptr<ProcedureNode> proc1 = std::make_shared<ProcedureNode>(procedureName1, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{callNode1}));
        std::shared_ptr<ProcedureNode> proc2 = std::make_shared<ProcedureNode>(procedureName2, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{callNode2}));
        std::shared_ptr<ProcedureNode> proc3 = std::make_shared<ProcedureNode>(procedureName3, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{callNode3}));

        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{proc1, proc2, proc3}));

        REQUIRE_THROWS(extractor->extract(program));
    }

    SECTION("CFG Test") {
        /*
            procedure First {
        01      read x;
        02      read z;
        03      call Second; 
            }

            procedure Second {
        04    x = 0;
        05    i = 5;
        06    while (i!=0) {
        07      x = x + 2*y;
        08      call Third;
        09      i = i - 1; 
                
        10      if (p==2) then {
        11        a = b;
                } else {
        12          b = c;
                }
        13      x = 2;
              }
        14    if (x==1) then {
        15      x = x+1;
                
        16      while (x!=0) {
        17        x = x + 1;
                }
        18      p = 1;
              } else {
        19      z = 1; 
                
        20      if (l != 9) then {
        21        z = 10;
                } else {
        22        z = 11;
                }
              }
        23    z = z + x + i;
        24    y = z + 2;
        25    x = x * y + z; 
            }

            procedure Third {
        26    z = 5;
        27    v = z;
        28    print v; 
            }
        */

        std::shared_ptr<PKBRelationshipStub> pkb = std::make_shared<PKBRelationshipStub>();
        std::shared_ptr<RelationshipExtractor> extractor = std::make_shared<RelationshipExtractor>(pkb);

        std::string procedureName1 = "First";
        std::string procedureName2 = "Second";
        std::string procedureName3 = "Third";

        std::string xVarName = "x";
        std::string zVarName = "z";
        std::string iVarName = "i";
        std::string yVarName = "y";
        std::string aVarName = "a";
        std::string bVarName = "b";
        std::string cVarName = "c";
        std::string pVarName = "p";
        std::string lVarName = "l";
        std::string vVarName = "v";

        std::string zeroConst = "0";
        std::string oneConst = "1";
        std::string twoConst = "2";
        std::string fiveConst = "5";
        std::string nineConst = "9";
        std::string tenConst = "10";
        std::string elevenConst = "11";

        std::shared_ptr<VariableNode> xVar1 = std::make_shared<VariableNode>(xVarName, 1);
        std::shared_ptr<VariableNode> xVar2 = std::make_shared<VariableNode>(xVarName, 4);
        std::shared_ptr<VariableNode> xVar3 = std::make_shared<VariableNode>(xVarName, 7);
        std::shared_ptr<VariableNode> xVar4 = std::make_shared<VariableNode>(xVarName, 13);
        std::shared_ptr<VariableNode> xVar5 = std::make_shared<VariableNode>(xVarName, 15);
        std::shared_ptr<VariableNode> xVar6 = std::make_shared<VariableNode>(xVarName, 17);
        std::shared_ptr<VariableNode> xVar7 = std::make_shared<VariableNode>(xVarName, 25);

        std::shared_ptr<VariableNode> yVar1 = std::make_shared<VariableNode>(yVarName, 24);

        std::shared_ptr<VariableNode> zVar1 = std::make_shared<VariableNode>(zVarName, 2);
        std::shared_ptr<VariableNode> zVar2 = std::make_shared<VariableNode>(zVarName, 19);
        std::shared_ptr<VariableNode> zVar3 = std::make_shared<VariableNode>(zVarName, 21);
        std::shared_ptr<VariableNode> zVar4 = std::make_shared<VariableNode>(zVarName, 22);
        std::shared_ptr<VariableNode> zVar5 = std::make_shared<VariableNode>(zVarName, 23);
        std::shared_ptr<VariableNode> zVar6 = std::make_shared<VariableNode>(zVarName, 24);
        std::shared_ptr<VariableNode> zVar7 = std::make_shared<VariableNode>(zVarName, 25);
        std::shared_ptr<VariableNode> zVar8 = std::make_shared<VariableNode>(zVarName, 26);

        std::shared_ptr<VariableNode> iVar1 = std::make_shared<VariableNode>(iVarName, 5);
        std::shared_ptr<VariableNode> iVar2 = std::make_shared<VariableNode>(iVarName, 6);
        std::shared_ptr<VariableNode> iVar3 = std::make_shared<VariableNode>(iVarName, 9);

        std::shared_ptr<VariableNode> aVar1 = std::make_shared<VariableNode>(aVarName, 11);
        std::shared_ptr<VariableNode> bVar1 = std::make_shared<VariableNode>(bVarName, 12);
        std::shared_ptr<VariableNode> pVar1 = std::make_shared<VariableNode>(pVarName, 18);
        std::shared_ptr<VariableNode> vVar1 = std::make_shared<VariableNode>(vVarName, 27);
        std::shared_ptr<VariableNode> vVar2 = std::make_shared<VariableNode>(vVarName, 28);

        // Proc First
        std::shared_ptr<ReadNode> readNode1 = std::make_shared<ReadNode>(xVar1, 1);
        std::shared_ptr<ReadNode> readNode2 = std::make_shared<ReadNode>(zVar1, 2);
        std::shared_ptr<CallNode> callNode1 = std::make_shared<CallNode>(procedureName2, 3);
        std::shared_ptr<ProcedureNode> proc1 = std::make_shared<ProcedureNode>(procedureName1, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{readNode1, readNode2, callNode1}));

        // Proc Third
        std::shared_ptr<PrintNode> printNode1 = std::make_shared<PrintNode>(vVar2, 28);
        std::shared_ptr<AssignNode> assignNode1 = std::make_shared<AssignNode>(zVar8, std::make_shared<ExpressionNode>("5", std::vector<std::string>{}, std::vector<std::string>{fiveConst}, 26), 26);
        std::shared_ptr<AssignNode> assignNode2 = std::make_shared<AssignNode>(vVar1, std::make_shared<ExpressionNode>("z", std::vector<std::string>{zVarName}, std::vector<std::string>{}, 27), 27);
        std::shared_ptr<ProcedureNode> proc3 = std::make_shared<ProcedureNode>(procedureName3, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode1, assignNode2, printNode1}));

        // Proc Second
        std::shared_ptr<AssignNode> assignNode3 = std::make_shared<AssignNode>(xVar2, std::make_shared<ExpressionNode>("0", std::vector<std::string>{}, std::vector<std::string>{zeroConst}, 4), 4);
        std::shared_ptr<AssignNode> assignNode4 = std::make_shared<AssignNode>(iVar1, std::make_shared<ExpressionNode>("5", std::vector<std::string>{}, std::vector<std::string>{fiveConst}, 5), 5);

        // while statement - line 6
        std::shared_ptr<AssignNode> assignNode5 = std::make_shared<AssignNode>(xVar3, std::make_shared<ExpressionNode>("x+2*y", std::vector<std::string>{xVarName, yVarName}, std::vector<std::string>{twoConst}, 7), 7);
        std::shared_ptr<CallNode> callNode2 = std::make_shared<CallNode>(procedureName3, 8);
        std::shared_ptr<AssignNode> assignNode6 = std::make_shared<AssignNode>(iVar3, std::make_shared<ExpressionNode>("i-1", std::vector<std::string>{iVarName}, std::vector<std::string>{oneConst}, 9), 9);
        std::shared_ptr<ConditionNode> ifConditionalNode1 = std::make_shared<ConditionNode>(std::vector<std::string>{pVarName}, std::vector<std::string>{twoConst}, "==");
        std::shared_ptr<AssignNode> assignNode7 = std::make_shared<AssignNode>(aVar1, std::make_shared<ExpressionNode>("b", std::vector<std::string>{bVarName}, std::vector<std::string>{}, 11), 11);
        std::shared_ptr<AssignNode> assignNode8 = std::make_shared<AssignNode>(bVar1, std::make_shared<ExpressionNode>("c", std::vector<std::string>{cVarName}, std::vector<std::string>{}, 12), 12);
        std::shared_ptr<IfNode> ifNode1 = std::make_shared<IfNode>(ifConditionalNode1, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode7}), std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode8}), 10);
        std::shared_ptr<AssignNode> assignNode9 = std::make_shared<AssignNode>(xVar4, std::make_shared<ExpressionNode>("2", std::vector<std::string>{}, std::vector<std::string>{twoConst}, 13), 13);
        std::shared_ptr<WhileNode> whileNode1 = std::make_shared<WhileNode>(std::make_shared<ConditionNode>(std::vector<std::string>{iVarName}, std::vector<std::string>{zeroConst}, "!="), std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode5, callNode2, assignNode6, ifNode1, assignNode9}), 6);

        // if statement - line 14
        std::shared_ptr<ConditionNode> ifConditionalNode2 = std::make_shared<ConditionNode>(std::vector<std::string>{xVarName}, std::vector<std::string>{oneConst}, "==");
        std::shared_ptr<AssignNode> assignNode10 = std::make_shared<AssignNode>(xVar5, std::make_shared<ExpressionNode>("x+1", std::vector<std::string>{xVarName}, std::vector<std::string>{oneConst}, 15), 15);
        std::shared_ptr<AssignNode> assignNode11 = std::make_shared<AssignNode>(xVar6, std::make_shared<ExpressionNode>("x+1", std::vector<std::string>{xVarName}, std::vector<std::string>{oneConst}, 17), 17);
        std::shared_ptr<WhileNode> whileNode2 = std::make_shared<WhileNode>(std::make_shared<ConditionNode>(std::vector<std::string>{xVarName}, std::vector<std::string>{zeroConst}, "!="), std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode11}), 16);
        std::shared_ptr<AssignNode> assignNode12 = std::make_shared<AssignNode>(pVar1, std::make_shared<ExpressionNode>("1", std::vector<std::string>{}, std::vector<std::string>{oneConst}, 18), 18);
        auto ifThenStmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode10, whileNode2, assignNode12});
        std::shared_ptr<AssignNode> assignNode13 = std::make_shared<AssignNode>(zVar2, std::make_shared<ExpressionNode>("1", std::vector<std::string>{}, std::vector<std::string>{oneConst}, 19), 19);
        std::shared_ptr<ConditionNode> ifConditionalNode3 = std::make_shared<ConditionNode>(std::vector<std::string>{lVarName}, std::vector<std::string>{nineConst}, "!=");
        std::shared_ptr<AssignNode> assignNode14 = std::make_shared<AssignNode>(zVar3, std::make_shared<ExpressionNode>("10", std::vector<std::string>{}, std::vector<std::string>{tenConst}, 21), 21);
        std::shared_ptr<AssignNode> assignNode15 = std::make_shared<AssignNode>(zVar4, std::make_shared<ExpressionNode>("11", std::vector<std::string>{}, std::vector<std::string>{elevenConst}, 22), 22);
        std::shared_ptr<IfNode> ifNode2 = std::make_shared<IfNode>(ifConditionalNode3, std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode14}), std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode15}), 20);
        auto ifElseStmtList = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode13, ifNode2});
        std::shared_ptr<IfNode> ifNode3 = std::make_shared<IfNode>(ifConditionalNode2, ifThenStmtList, ifElseStmtList, 14);

        std::shared_ptr<AssignNode> assignNode16 = std::make_shared<AssignNode>(zVar5, std::make_shared<ExpressionNode>("z+x+i", std::vector<std::string>{zVarName, xVarName, iVarName}, std::vector<std::string>{}, 23), 23);
        std::shared_ptr<AssignNode> assignNode17 = std::make_shared<AssignNode>(zVar6, std::make_shared<ExpressionNode>("z+2", std::vector<std::string>{zVarName}, std::vector<std::string>{twoConst}, 24), 24);
        std::shared_ptr<AssignNode> assignNode18 = std::make_shared<AssignNode>(xVar7, std::make_shared<ExpressionNode>("x*y+z", std::vector<std::string>{xVarName, yVarName, zVarName}, std::vector<std::string>{}, 25), 25);

        auto stmtList2 = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignNode3, assignNode4, whileNode1, ifNode3, assignNode16, assignNode17, assignNode18});
        std::shared_ptr<ProcedureNode> proc2 = std::make_shared<ProcedureNode>(procedureName2, stmtList2);

        auto procedureList = std::make_shared<std::vector<std::shared_ptr<ProcedureNode>>>(std::vector<std::shared_ptr<ProcedureNode>>{proc1, proc2, proc3});
        std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>(procedureList);

        extractor->extract(program);

        REQUIRE(pkb->next.size() == 25);
        REQUIRE(pkb->next[14] == std::unordered_set<int>{15, 19});
        REQUIRE(pkb->next[6] == std::unordered_set<int>{7, 14});
        REQUIRE(pkb->next[13] == std::unordered_set<int>{6});
        REQUIRE(pkb->next[11] == std::unordered_set<int>{13});
        REQUIRE(pkb->next[12] == std::unordered_set<int>{13});
        REQUIRE(pkb->next[10] == std::unordered_set<int>{11, 12});
        REQUIRE(pkb->next[18] == std::unordered_set<int>{23});
        REQUIRE(pkb->next[22] == std::unordered_set<int>{23});
        REQUIRE(pkb->next[21] == std::unordered_set<int>{23});
        REQUIRE(pkb->next.count(28) == 0);
        REQUIRE(pkb->next.count(3) == 0);
        REQUIRE(pkb->next.count(25) == 0);
    }
}
