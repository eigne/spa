#include "ProgramNode.h"

#include <utility>

ProgramNode::ProgramNode(std::shared_ptr<std::vector<std::shared_ptr<ProcedureNode> > > lst) {
    this->lst = std::move(lst);
}

std::string ProgramNode::toString() {
    std::string res;
    for (auto & it : *lst) {
        res += it->toString();
    }
    return res;
}

std::shared_ptr<std::vector<std::shared_ptr<ProcedureNode> > > ProgramNode::getProcLst() {
    return lst;
}
