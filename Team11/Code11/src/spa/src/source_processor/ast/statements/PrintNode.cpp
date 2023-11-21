#include "PrintNode.h"
#include <utility>
#include "source_processor/design_extractor/interface/ASTVisitor.h"

PrintNode::PrintNode(std::shared_ptr<VariableNode> var, int index) {
    this->var = std::move(var);
    setLineNum(index);
}

std::shared_ptr<VariableNode> PrintNode::getVar() {
    return var;
}

std::string PrintNode::toString() {
    return "print " + var->toString() + ";";
}

void PrintNode::accept(ASTVistor& visitor) {
    auto p = this->shared_from_this();
    visitor.visit(p);
}
