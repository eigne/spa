#include "VariableNode.h"

#include <utility>

VariableNode::VariableNode(std::string name, int lineNum) {
    this->name = std::move(name);
    associatedLineNum = lineNum;
}

std::string VariableNode::getVarName() {
    return name;
}

int VariableNode::getAssociatedLineNum() const {
    return associatedLineNum;
}

std::string VariableNode::toString() {
    return name;
}
