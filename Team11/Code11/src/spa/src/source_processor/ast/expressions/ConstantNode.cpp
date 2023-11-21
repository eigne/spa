#include "ConstantNode.h"

#include <utility>

ConstantNode::ConstantNode(std::string value) {
    this->value = std::move(value);
}

std::string ConstantNode::toString() {
    return value;
}
