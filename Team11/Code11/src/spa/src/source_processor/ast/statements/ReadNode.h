#ifndef SPA_READNODE_H
#define SPA_READNODE_H

#pragma once

#include <string>
#include "StatementNode.h"
#include "source_processor/ast/expressions/VariableNode.h"

class ReadNode : public StatementNode, public std::enable_shared_from_this<ReadNode> {
public:
    ReadNode(std::shared_ptr<VariableNode> var, int index);
    ~ReadNode() override = default;
    std::string toString() override;
    void accept(ASTVistor& visitor) override;
    std::shared_ptr<VariableNode> getVar();

private:
    std::shared_ptr<VariableNode> var;
};

#endif //SPA_READNODE_H
