#include "source_processor/ast/ProcedureNode.h"
#include "source_processor/ast/statements/ReadNode.h"
#include "source_processor/ast/statements/CallNode.h"
#include "source_processor/ast/statements/PrintNode.h"
#include "source_processor/ast/statements/AssignNode.h"
#include "source_processor/ast/statements/ConditionNode.h"
#include "source_processor/ast/statements/RelationalNode.h"
#include "source_processor/ast/statements/WhileNode.h"
#include "source_processor/ast/statements/IfNode.h"
#include "source_processor/ast/expressions/VariableNode.h"
#include "source_processor/ast/expressions/ConstantNode.h"
#include "source_processor/ast/expressions/ExpressionNode.h"

#include "catch.hpp"

auto aPtr = std::make_shared<VariableNode>("a", 1);
auto bPtr = std::make_shared<VariableNode>("b", 2);
auto cPtr = std::make_shared<VariableNode>("c", 3);
VariableNode a = *aPtr;
VariableNode b = *bPtr;
VariableNode c = *cPtr;

TEST_CASE("Variable created correctly") {
    CHECK(a.toString() == "a");
    CHECK(b.toString() == "b");
}

// Test if constants created correctly
auto one = std::make_shared<ConstantNode>("1");
auto two = std::make_shared<ConstantNode>("2");
TEST_CASE("Constants created correctly") {
    CHECK(one->toString() == "1");
    CHECK(two->toString() == "2");
}

// Test creation of operator nodes
ExpressionNode addition ("a + b + 2", std::vector<std::string>{"a", "b"}, std::vector<std::string>{"2"}, 2);
TEST_CASE("Operator created correctly") {
    auto opV = std::make_shared<std::vector<std::string>>();
    opV->emplace_back("+");
    opV->emplace_back("a");
    opV->emplace_back("b");
    CHECK(addition.toString() == "a + b + 2");
    CHECK(addition.getVariables() == std::vector<std::string>{"a", "b"});
    CHECK(addition.getConstants() == std::vector<std::string>{"2"});
    CHECK(addition.getSortedOp() == std::vector<std::string>{ "a", "b", "+", "2", "+" });
}

//Test creation of AssignNodes
auto additionPtr = std::make_shared<ExpressionNode>("b + 2", std::vector<std::string>{"b"}, std::vector<std::string>{"2"}, 2);
auto assignAptr = std::make_shared<AssignNode>(aPtr, additionPtr, 2);
AssignNode assignA = *assignAptr;

TEST_CASE("Assignment created correctly") {
    CHECK(assignA.toString() == "a = b + 2;");
    CHECK(assignA.getLineNum() == 2);
    CHECK(assignA.getLeft() == aPtr);
    CHECK(assignA.getRight() == additionPtr);
    CHECK(assignA.getRightVariables() == std::vector<std::string>{"b"});
    CHECK(assignA.getConstants() == std::vector<std::string>{"2"});
}

// Test creation of ReadNodes
auto ra = std::make_shared<ReadNode>(aPtr, 1);
TEST_CASE("Read created correctly") {
    CHECK(ra->toString() == "read a;");
    CHECK(ra->getLineNum() == 1);
}

// Test creation of PrintNode
auto pb = std::make_shared<PrintNode>(bPtr, 3);
TEST_CASE("Print created correctly") {
    CHECK(pb->toString() == "print b;");
}

std::vector<std::string> constList {
    "2",
};

std::vector<std::string> varList {
    "a",
};

// Test creation of ConditionNode
auto eq = std::make_shared<ConditionNode>(varList, constList, "a < 2");

TEST_CASE("Condition created correctly") {
    CHECK(eq->toString() == "a < 2");
}

// Test creation of WhileNode
auto lst = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ra, assignAptr});
auto w = std::make_shared<WhileNode>(eq, lst, 15);

TEST_CASE("WhileNode created correctly") {
    CHECK(w->toString() == "while (a < 2) {read a;a = b + 2;}");
    CHECK(w->getLineNum() == 15);
    CHECK(w->getCond() == eq);
}

// Test creation of IfNode
auto vra = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{ra});
auto vAPtr = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>(std::vector<std::shared_ptr<StatementNode>>{assignAptr});
auto i = std::make_shared<IfNode>(eq, vra, vAPtr, 30);

TEST_CASE("IfNode created correctly") {
    CHECK(i->toString() == "if (a < 2) then {read a;} else {a = b + 2;}");
}

// Test creation of ProcedureNode
auto psl = std::make_shared<std::vector<std::shared_ptr<StatementNode>>>();

// THIS TEST WILL FAIL because we are assigning ; to every stmt, even if and while. Non urgent change to amend all statement nodes to enforce their own ; in toString method
TEST_CASE("ProcedureNode created correctly") {
    psl->push_back(i);
    psl->push_back(w);

    ProcedureNode p{"test",psl};
    CHECK(p.toString() == "procedure test {if (a < 2) then {read a;} else {a = b + 2;}while (a < 2) {read a;a = b + 2;}}");
}
