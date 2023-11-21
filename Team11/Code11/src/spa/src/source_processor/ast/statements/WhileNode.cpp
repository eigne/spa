#include "WhileNode.h"

#include <utility>
#include "source_processor/design_extractor/interface/ASTVisitor.h"

WhileNode::WhileNode(std::shared_ptr<ConditionNode> cond,
                     std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > sl, int index) {
    condition = std::move(cond);
    stmtLst = std::move(sl);
    setLineNum(index);
}

std::string WhileNode::toString() {
    std::string res = "while (" + condition->toString() + ") {";
    for (auto & iter : *stmtLst) {
        res += iter->toString();
    }
    res = res + "}";
    return res;
}

std::shared_ptr<ConditionNode> WhileNode::getCond() {
    return condition;
}

std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > WhileNode::getStmtLst() {
    return stmtLst;
}

void WhileNode::accept(ASTVistor& visitor) {
    auto w = this->shared_from_this();
    visitor.visit(w);
}
