#include "AssignNode.h"

#include <utility>
#include "source_processor/design_extractor/interface/ASTVisitor.h"

AssignNode::AssignNode(std::shared_ptr<VariableNode> left, std::shared_ptr<ExpressionNode> right, int index) {
    leftArg = std::move(left);
    rightArg = std::move(right);
    setLineNum(index);
}

std::string AssignNode::toString() {
    return getLeft()->toString() + " = " + getRight()->toString() + ";";
}

std::shared_ptr<VariableNode> AssignNode::getLeft() {
    return leftArg;
}

void AssignNode::accept(ASTVistor& visitor) {
    auto a = this->shared_from_this();
    visitor.visit(a);
}
std::shared_ptr<ExpressionNode> AssignNode::getRight() {
    return rightArg;
}

std::vector<std::string> AssignNode::getRightVariables() {
    std::vector<std::string> res;
    auto right = (*rightArg).getVariables();
    res.insert(res.end(), right.begin(), right.end());
    sort(res.begin(), res.end());
    res.erase(unique(res.begin(), res.end()), res.end());
    return res;
}

std::vector<std::string> AssignNode::getConstants() {
    return rightArg->getConstants();
}
