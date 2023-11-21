#include "RelationalNode.h"

#include <utility>

RelationalNode::RelationalNode(std::string op, std::shared_ptr<ExpressionNode> left,
                               std::shared_ptr<ExpressionNode> right) {
    this->op = std::move(op);
    this->leftArg = std::move(left);
    this->rightArg = std::move(right);
}

std::string RelationalNode::toString() {
    return this->getLeft()->toString() + " " + op + " " + this->getRight()->toString();
}

std::shared_ptr<ExpressionNode> RelationalNode::getLeft() {
    return this->leftArg;
}

std::shared_ptr<ExpressionNode> RelationalNode::getRight() {
    return this->rightArg;
}

std::string RelationalNode::getOp() {
    return this->op;
}
