#include "CallNode.h"
#include <utility>
#include "source_processor/ast/ProcedureNode.h"
#include "source_processor/design_extractor/interface/ASTVisitor.h"


CallNode::CallNode(std::string proc, int index) {
    this->procName = std::move(proc);
    setLineNum(index);
}

std::string CallNode::toString() {
    return "call " + procName + ";";
}

std::string CallNode::getProc() {
    return procName;
}
void CallNode::accept(ASTVistor& visitor) {
    auto c = this->shared_from_this();
    visitor.visit(c);
}
