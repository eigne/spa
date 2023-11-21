#include "IfNode.h"

#include <utility>
#include "source_processor/design_extractor/interface/ASTVisitor.h"

IfNode::IfNode(std::shared_ptr<ConditionNode> cond, std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > then,
               std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > el, int index) {
    condition = std::move(cond);
    thenStmtLst = std::move(then);
    elseStmtLst = std::move(el);
    setLineNum(index);
}

std::string IfNode::toString() {
    std::string res = "if (" + condition->toString() + ") then {";
    for (auto& iter : *thenStmtLst) {
        res += iter->toString();
    }
    res += "} else {";
    for (auto & iter : *elseStmtLst) {
        res += iter->toString();
    }
    res += "}";
    return res;
}

std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > IfNode::getThenStmtLst() {
    return thenStmtLst;
}

std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > IfNode::getElseStmtLst() {
    return elseStmtLst;
}

std::shared_ptr<ConditionNode> IfNode::getCond() {
    return condition;
}

void IfNode::accept(ASTVistor& visitor) {
    auto i = this->shared_from_this();
    visitor.visit(i);
}
