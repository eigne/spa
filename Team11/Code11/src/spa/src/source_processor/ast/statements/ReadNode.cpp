#include "ReadNode.h"
#include <utility>
#include "source_processor/design_extractor/interface/ASTVisitor.h"

ReadNode::ReadNode(std::shared_ptr<VariableNode> var, int index) {
    this->var = std::move(var);
    setLineNum(index);
}

std::string ReadNode::toString() {
    return "read " + var->toString() + ";";
}

std::shared_ptr<VariableNode> ReadNode::getVar() {
    return var;
}

void ReadNode::accept(ASTVistor& visitor) {
    auto r = this->shared_from_this();
    visitor.visit(r);
}
