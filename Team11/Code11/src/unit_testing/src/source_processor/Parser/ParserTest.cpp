#include "catch.hpp"

#include <typeinfo>
#include <iostream>

#include "commons/tokenizer/tokens/Token.h"
#include "source_processor/parser/ParserManager.h"
#include "source_processor/parser/ParserFactory.h"
#include "source_processor/parser/ReadParser.h"
#include "source_processor/parser/PrintParser.h"
#include "source_processor/parser/CallParser.h"
#include "source_processor/parser/AssignmentParser.h"
#include "source_processor/parser/OperatorParser.h"
#include "source_processor/parser/ConditionParser.h"
#include "source_processor/parser/WhileParser.h"
#include "source_processor/parser/IfParser.h"
#include "source_processor/parser/ProcedureParser.h"
#include "source_processor/ast/statements/CallNode.h"
#include "source_processor/ast/statements/IfNode.h"


ReadParser re;
PrintParser pr;
CallParser cp;
AssignmentParser ap;
WhileParser wp;
IfParser ip;
ProcedureParser pp;
TEST_CASE("ParserFactory returns correct parser for the required token") {
    auto dr = ParserFactory::getParser("read");
    CHECK(typeid(*dr) == typeid(re));
    auto dp = ParserFactory::getParser("print");
    CHECK(typeid(*dp) == typeid(pr));
    auto dc = ParserFactory::getParser("call");
    CHECK(typeid(*dc) == typeid(cp));
    auto da = ParserFactory::getParser("a");
    CHECK(typeid(*da) == typeid(ap));
    auto dw = ParserFactory::getParser("while");
    CHECK(typeid(*dw) == typeid(wp));
    auto di = ParserFactory::getParser("if");
    CHECK(typeid(*di) == typeid(ip));
    auto gp = ParserFactory::getParser("procedure");
    CHECK(typeid(*gp) == typeid(pp));
}

std::vector<std::shared_ptr<Token>> readA{
            std::make_shared<Token>("read"),
            std::make_shared<Token>("a"),
            std::make_shared<Token>(";"),
            std::make_shared<Token>("}")
};

std::vector<std::shared_ptr<Token>> readAWithMoreArg{
        std::make_shared<Token>("read"),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};

std::vector<std::shared_ptr<Token>> readInvalidVar{
        std::make_shared<Token>("read"),
        std::make_shared<Token>("1"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};

TEST_CASE("ReadParser parse correctly") {
    auto cur = readA.begin();
    std::shared_ptr<int> lineNum = std::make_shared<int>(11);
    std::shared_ptr<AbstractParser> rp = ParserFactory::getParser("read");
    std::shared_ptr<ASTNode> res = rp->parse(cur, readA.end(), lineNum);
    ReadNode r(std::make_shared<VariableNode>("a",2), 2);
    CHECK(typeid(*res) == typeid(r));
    auto* tis = dynamic_cast<ReadNode*>(res.get());
    CHECK(tis->getVar()->toString() == "a");
    CHECK(tis->getLineNum() == 11); //Statement index saved in var correctly
    CHECK(*lineNum == 12); //Incremented the line number correctly
    CHECK((*cur)->getStringValue() == "}");

    auto moreThan1Arg = readAWithMoreArg.begin();
    CHECK_THROWS_WITH(rp->parse(moreThan1Arg, readAWithMoreArg.end(), lineNum), "Missing semicolon in read a at line number 12");

    auto invalidVar = readInvalidVar.begin();
    CHECK_THROWS_WITH(rp->parse(invalidVar, readAWithMoreArg.end(), lineNum), "Variable to be read is not valid variable");
}

std::vector<std::shared_ptr<Token>> printA{
        std::make_shared<Token>("print"),
        std::make_shared<Token>("a"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};

std::vector<std::shared_ptr<Token>> printAWithMoreArg{
        std::make_shared<Token>("print"),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("a"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};

std::vector<std::shared_ptr<Token>> printInvalidVar{
        std::make_shared<Token>("print"),
        std::make_shared<Token>("1"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};

TEST_CASE("PrintParser parse correctly") {
    std::shared_ptr<int> lineNum = std::make_shared<int>(11);
    std::shared_ptr<AbstractParser> rp = ParserFactory::getParser("print");
    auto pcur = printA.begin();
    std::shared_ptr<ASTNode> res = rp->parse(pcur, printA.end(), lineNum);
    PrintNode r(std::make_shared<VariableNode>("a", 2), 2);
    CHECK(typeid(*res) == typeid(r));
    auto* tis = dynamic_cast<PrintNode*>(res.get());
    CHECK(tis->getVar()->toString() == "a");
    CHECK(tis->getLineNum() == 11);
    CHECK(*lineNum == 12);
    CHECK((*pcur)->getStringValue() == "}");

    auto aWithMoreArg = printAWithMoreArg.begin();
    CHECK_THROWS_WITH(rp->parse(aWithMoreArg, printAWithMoreArg.end(), lineNum), "Missing semicolon in print a at line number 12");

    auto pInvalid = printInvalidVar.begin();
    CHECK_THROWS_WITH(rp->parse(pInvalid, printAWithMoreArg.end(), lineNum),"Variable to be read is not valid variable" );
}

std::vector<std::shared_ptr<Token>> simpleOp{
        std::make_shared<Token>("("),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("*"),
        std::make_shared<Token>("("),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("d"),
        std::make_shared<Token>("*"),
        std::make_shared<Token>("e"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>(";"),
};

TEST_CASE("OperatorNode parses operation correctly") {
    auto cur = simpleOp.begin();
    auto stmtIndex = std::make_shared<int>(1);
    OperatorParser op;
    auto res = op.parse(cur, 1);

    CHECK(res->getVariables() == std::vector<std::string>{"a","b","c","d","e"});
}

TEST_CASE("Invalid operands in assignment should throw error") {
    std::vector<std::shared_ptr<Token>> invalidIntOp{
            std::make_shared<Token>("("),
            std::make_shared<Token>("a"),
            std::make_shared<Token>("*"),
            std::make_shared<Token>("("),
            std::make_shared<Token>("b"),
            std::make_shared<Token>("+"),
            std::make_shared<Token>("02"),
            std::make_shared<Token>(")"),
            std::make_shared<Token>("+"),
            std::make_shared<Token>("d"),
            std::make_shared<Token>("*"),
            std::make_shared<Token>("e"),
            std::make_shared<Token>(")"),
            std::make_shared<Token>(";"),
    };

    auto cur = invalidIntOp.begin();
    auto stmtIndex = std::make_shared<int>(1);
    OperatorParser op;
    CHECK_THROWS_WITH(op.parse(cur, 1), "Operand is not a proper int in assignment line 1");
}

std::vector<std::shared_ptr<Token>> bracesOp{
        std::make_shared<Token>("a"),
        std::make_shared<Token>("*"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("d"),
        std::make_shared<Token>("*"),
        std::make_shared<Token>("e"),
        std::make_shared<Token>(")")
};

std::vector<std::shared_ptr<Token>> proc{
        std::make_shared<Token>("procedure"),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("print"),
        std::make_shared<Token>("a"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};

TEST_CASE("ProcedureParser parses correctly") {
    std::shared_ptr<AbstractParser> rp = ParserFactory::getParser("procedure");
    auto lineNum = std::make_shared<int>(0);
    auto it = proc.begin();
    auto res = rp->parse(it , proc.end(), lineNum);
    CHECK(res->toString() == "procedure a {print a;}");

}

std::vector<std::shared_ptr<Token>> callA {
        std::make_shared<Token>("call"),
        std::make_shared<Token>("lol"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};

TEST_CASE("CallParser parse correctly") {
    std::shared_ptr<int> lineNum = std::make_shared<int>(11);
    std::shared_ptr<AbstractParser> rp = ParserFactory::getParser("call");
    auto pcur = callA.begin();
    std::shared_ptr<ASTNode> res = rp->parse(pcur, callA.end(), lineNum);
    CallNode r("a", 2);
    CHECK(typeid(*res) == typeid(r));
    auto* tis = dynamic_cast<CallNode*>(res.get());
    CHECK(tis->getProc() == "lol");
    CHECK(tis->getLineNum() == 11);
    CHECK(*lineNum == 12);
    CHECK((*pcur)->getStringValue() == "}");
}

std::vector<std::shared_ptr<Token>> assignC {
        std::make_shared<Token>("c"),
        std::make_shared<Token>("="),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};

std::vector<std::shared_ptr<Token>> assignCWithBrace {
        std::make_shared<Token>("c"),
        std::make_shared<Token>("="),
        std::make_shared<Token>("("),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};
TEST_CASE("AssignmentParser parse correctly") {
    std::shared_ptr<int> lineNum = std::make_shared<int>(11);
    auto curToken = assignC.begin();
    std::shared_ptr<AbstractParser> rp = ParserFactory::getParser("a");
    std::shared_ptr<StatementNode> res;
    CHECK_NOTHROW(res = rp->parse(curToken, assignC.end(), lineNum));

    auto ptrWithBrace = assignCWithBrace.begin();
    auto* assign = dynamic_cast<AssignNode*>(res.get());
    std::vector<std::string> listOfVar = {"a", "b"};
    std::vector<std::string> listOfConstant = {"2"};

    CHECK(res->getLineNum() == 11);
    CHECK(*lineNum == 12);
    CHECK((*curToken)->getStringValue() == "}");

    CHECK(res->toString() == "c = a+b+2;");
    CHECK(assign->getRightVariables() == listOfVar);
    CHECK(assign->getConstants() == listOfConstant);

    CHECK_NOTHROW(rp->parse(ptrWithBrace, assignCWithBrace.end(), lineNum));
}

std::vector<std::shared_ptr<Token>> cond {
        std::make_shared<Token>("("),
        std::make_shared<Token>("("),
        std::make_shared<Token>("("),
        std::make_shared<Token>("c"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>("<"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>("&&"),
        std::make_shared<Token>("("),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("<"),
        std::make_shared<Token>("d1"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>(")"),
};

TEST_CASE("Parse Condition Expression correctly") {
    ConditionParser c;
    auto arg = cond.begin();
    auto res = c.parseCondition(arg);

    CHECK(res->getVariables() == std::vector<std::string>{"a", "b", "c", "d1"});
    CHECK(res->getConstants() == std::vector<std::string>{"2"});
}
std::vector<std::shared_ptr<Token>> whileTest{
        std::make_shared<Token>("while"),
        std::make_shared<Token>("("),
        std::make_shared<Token>("c"),
        std::make_shared<Token>("=="),
        std::make_shared<Token>("a"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("="),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}"),
};

TEST_CASE("Parse while loop correctly") {
    std::shared_ptr<int> lineNum = std::make_shared<int>(11);
    auto wToken = whileTest.begin();
    std::shared_ptr<AbstractParser> rp = ParserFactory::getParser("while");
    auto res = rp->parse(wToken, whileTest.end(), lineNum);
    auto* w = dynamic_cast<WhileNode*>(res.get());

    CHECK(*lineNum == 13);

    CHECK(w->getCond()->getConstants() == std::vector<std::string>());
    CHECK(w->getCond()->getVariables() == std::vector<std::string>{"a","c"});
    CHECK(w->getLineNum() == 11);
    auto content = (*(w->getStmtLst()))[0];
    auto a = dynamic_cast<AssignNode*>(content.get());
    CHECK( a->getLineNum()== 12);
}

std::vector<std::shared_ptr<Token>> ifTest{
        std::make_shared<Token>("if"),
        std::make_shared<Token>("("),
        std::make_shared<Token>("("),
        std::make_shared<Token>("c"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>("=="),
        std::make_shared<Token>("a"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>("then"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("="),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}"),
        std::make_shared<Token>("else"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("print"),
        std::make_shared<Token>("z"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}")
};

TEST_CASE("Parse if loop correctly") {
    std::shared_ptr<int> lineNum = std::make_shared<int>(11);
    auto wToken = ifTest.begin();
    std::shared_ptr<AbstractParser> rp = ParserFactory::getParser("if");
    auto res = rp->parse(wToken, ifTest.end(), lineNum);
    auto* i = dynamic_cast<IfNode*>(res.get());

    CHECK(*lineNum == 14);

    CHECK(i->getCond()->getConstants() == std::vector<std::string>());
    CHECK(i->getCond()->getVariables() == std::vector<std::string>{"a","c"});
    CHECK(i->getLineNum() == 11);

    auto thenContent = (*(i->getThenStmtLst()))[0];
    auto a = dynamic_cast<AssignNode*>(thenContent.get());

    CHECK( a->getLineNum()== 12);

    auto elseContent = (*(i->getElseStmtLst()))[0];
    auto p = dynamic_cast<PrintNode*>(elseContent.get());

    CHECK(p->getLineNum() == 13);
    CHECK(p->getVar()->getVarName() == "z");
}

TEST_CASE("If loop only constants") {
    std::vector<std::shared_ptr<Token>> ifConstTest{
            std::make_shared<Token>("if"),
            std::make_shared<Token>("("),
            std::make_shared<Token>("3"),
            std::make_shared<Token>(">"),
            std::make_shared<Token>("4"),
            std::make_shared<Token>(")"),
            std::make_shared<Token>("then"),
            std::make_shared<Token>("{"),
            std::make_shared<Token>("b"),
            std::make_shared<Token>("="),
            std::make_shared<Token>("b"),
            std::make_shared<Token>("+"),
            std::make_shared<Token>("2"),
            std::make_shared<Token>("+"),
            std::make_shared<Token>("c"),
            std::make_shared<Token>(";"),
            std::make_shared<Token>("}"),
            std::make_shared<Token>("else"),
            std::make_shared<Token>("{"),
            std::make_shared<Token>("print"),
            std::make_shared<Token>("z"),
            std::make_shared<Token>(";"),
            std::make_shared<Token>("}")
    };
    std::shared_ptr<int> lineNum = std::make_shared<int>(11);
    auto wToken = ifConstTest.begin();
    std::shared_ptr<AbstractParser> rp = ParserFactory::getParser("if");
    auto res = rp->parse(wToken, ifConstTest.end(), lineNum);
    auto i = dynamic_cast<IfNode*>(res.get());

    CHECK(*lineNum == 14);

    CHECK(i->getCond()->getConstants() == std::vector<std::string>{"3","4"});
    CHECK(i->getCond()->getVariables() == std::vector<std::string>{});
    CHECK(i->getLineNum() == 11);

    auto thenContent = (*(i->getThenStmtLst()))[0];
    auto a = dynamic_cast<AssignNode*>(thenContent.get());

    CHECK( a->getLineNum()== 12);

    auto elseContent = (*(i->getElseStmtLst()))[0];
    auto p = dynamic_cast<PrintNode*>(elseContent.get());

    CHECK(p->getLineNum() == 13);
    CHECK(p->getVar()->getVarName() == "z");
}


std::vector<std::shared_ptr<Token>> prog{
        std::make_shared<Token>("procedure"),
        std::make_shared<Token>("main"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>("="),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("read"),
        std::make_shared<Token>("a"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("print"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("if"),
        std::make_shared<Token>("("),
        std::make_shared<Token>("c"),
        std::make_shared<Token>("=="),
        std::make_shared<Token>("a"),
        std::make_shared<Token>(")"),
        std::make_shared<Token>("then"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("="),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}"),
        std::make_shared<Token>("else"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("print"),
        std::make_shared<Token>("z"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}"),
        std::make_shared<Token>("}"),
};

ParserManager pm;

TEST_CASE("ParserManager parses single procedure program correctly") {
    auto res = pm.generateAST(prog);
    CHECK(typeid(*res) == typeid(ProgramNode(nullptr)));
    auto stmtlst = (*res).getProcLst();
    auto procMain = *(stmtlst->begin());
    auto it = procMain->getStmtLst()->begin();
    auto assignNode = *it;
    it++;
    auto rnode = *it;
    it++;
    auto pnode = *it;
    it++;
    auto ifn = *it;

    CHECK(assignNode->toString() == "c = a+b+2+c;");
    CHECK(rnode->toString() == "read a;");
    CHECK(pnode->toString() == "print b;");
}

std::vector<std::shared_ptr<Token>> multiProcProg{
        std::make_shared<Token>("procedure"),
        std::make_shared<Token>("main"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>("="),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}"),

        std::make_shared<Token>("procedure"),
        std::make_shared<Token>("robot"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("read"),
        std::make_shared<Token>("a"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("call"),
        std::make_shared<Token>("printer"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}"),

        std::make_shared<Token>("procedure"),
        std::make_shared<Token>("printer"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("print"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}"),
};

TEST_CASE("ParserManager parses multiple procedure program correctly") {
    auto res = pm.generateAST(multiProcProg);
    CHECK(typeid(*res) == typeid(ProgramNode(nullptr)));
    auto stmtlst = (*res).getProcLst();
    auto procIt = stmtlst->begin();
    auto procMain = *procIt;
    procIt++;
    auto procRobot = *procIt;
    procIt++;
    auto procPrinter = *procIt;

    auto mainIt = procMain->getStmtLst()->begin();
    auto assignNode = *mainIt;

    auto robotIt = procRobot->getStmtLst()->begin();
    auto rnode = *robotIt;
    robotIt++;
    auto callNode = *robotIt;

    auto printerIt = procPrinter->getStmtLst()->begin();
    auto pnode = *printerIt;

    CHECK(assignNode->toString() == "c = a+b+2+c;");
    CHECK(rnode->toString() == "read a;");
    CHECK(callNode->toString() == "call printer;");
    CHECK(pnode->toString() == "print b;");
}


std::vector<std::shared_ptr<Token>> ProcInProcProg{
        std::make_shared<Token>("procedure"),
        std::make_shared<Token>("main"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>("="),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("procedure"),
        std::make_shared<Token>("printer"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("print"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}"),
        std::make_shared<Token>("}"),
};

TEST_CASE("Procedure in a procedure should fail") {
    CHECK_THROWS_WITH(pm.generateAST(ProcInProcProg), "Procedure cannot be in a procedure");
}

std::vector<std::shared_ptr<Token>> StmtNotInProcProg{
        std::make_shared<Token>("procedure"),
        std::make_shared<Token>("main"),
        std::make_shared<Token>("{"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>("="),
        std::make_shared<Token>("a"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("2"),
        std::make_shared<Token>("+"),
        std::make_shared<Token>("c"),
        std::make_shared<Token>(";"),
        std::make_shared<Token>("}"),
        std::make_shared<Token>("print"),
        std::make_shared<Token>("b"),
        std::make_shared<Token>(";"),
};

TEST_CASE("First level of statements should all be procedures") {
    CHECK_THROWS_WITH(pm.generateAST(StmtNotInProcProg), "Statement print not in a procedure");
}
