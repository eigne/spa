#pragma once

#include <memory>
#include "source_processor/ast/statements/IfNode.h"
#include "source_processor/ast/statements/WhileNode.h"
#include "source_processor/ast/statements/CallNode.h"
#include "source_processor/ast/statements/AssignNode.h"
#include "source_processor/ast/statements/PrintNode.h"
#include "source_processor/ast/statements/ReadNode.h"

class ASTVistor {
public:
    virtual void visit(std::shared_ptr<IfNode>& ifNode) = 0;
    virtual void visit(std::shared_ptr<WhileNode>& whileNode) = 0;
    virtual void visit(std::shared_ptr<CallNode>& forNode) = 0;
    virtual void visit(std::shared_ptr<AssignNode>& assignNode) = 0;
    virtual void visit(std::shared_ptr<PrintNode>& printNode) = 0;
    virtual void visit(std::shared_ptr<ReadNode>& readNode) = 0;
};
