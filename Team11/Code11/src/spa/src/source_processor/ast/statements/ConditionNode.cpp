#include "ConditionNode.h"

#include <utility>

ConditionNode::ConditionNode(std::vector<std::string> varList, std::vector<std::string> constList, std::string cond) {
    this->varList = std::move(varList);
    this->constList = std::move(constList);
    this->condOp = std::move(cond);
}

std::vector<std::string> ConditionNode::getVariables() {
    return this->varList;
}

std::vector<std::string> ConditionNode::getConstants() {
    return this->constList;
}

std::string ConditionNode::toString() {
    return this->condOp;
}
