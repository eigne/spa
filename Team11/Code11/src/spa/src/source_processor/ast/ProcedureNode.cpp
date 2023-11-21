#include "ProcedureNode.h"

#include <utility>

ProcedureNode::ProcedureNode(std::string procName,
                             std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > stmtLst) {
    name = std::move(procName);
    this->stmtLst = std::move(stmtLst);
}

std::string ProcedureNode::toString() {
    std::string res = "procedure " + name + " {";
    for (auto & iter : *stmtLst) {
        res += iter->toString();
    }
    res += "}";
    return res;
}

std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > ProcedureNode::getStmtLst() {
    return stmtLst;
}

std::string ProcedureNode::getName() {
    return name;
}
