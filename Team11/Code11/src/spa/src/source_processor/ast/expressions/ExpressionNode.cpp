#include "ExpressionNode.h"

#include <utility>

ExpressionNode::ExpressionNode(const std::string& op, std::vector<std::string> var, std::vector<std::string>c, int index) {
    this->op = op;
    sortedOp = ExpressionParser::convertToPostfix(op);
    variables = std::move(var);
    constants = std::move(c);
    lineNum = index;
}

std::string ExpressionNode::toString() {
    return op;
}

std::vector<std::string> ExpressionNode::getSortedOp() {
    return sortedOp;
}

std::vector<std::string> ExpressionNode::getVariables() {
    return variables;
}

std::vector<std::string> ExpressionNode::getConstants() {
    return constants;
}

int ExpressionNode::getLineNum() const {
    return lineNum;
}
