#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#pragma once

#include <string>
#include "StatementNode.h"
#include "source_processor/ast/ProcedureNode.h"

class CallNode : public StatementNode, public std::enable_shared_from_this<CallNode> {
public:
    explicit CallNode(std::string proc, int index);
    ~CallNode() override = default;
    std::string toString() override;
    void accept(ASTVistor& visitor) override;
    std::string getProc();

private:
    std::string procName;
};

#endif //SPA_CALLNODE_H
