#ifndef SPA_PRINTNODE_H
#define SPA_PRINTNODE_H

#pragma once

#include "StatementNode.h"
#include <string>
#include <memory>
#include "source_processor/ast/expressions/VariableNode.h"

class PrintNode : public StatementNode, public std::enable_shared_from_this<PrintNode> {
public:
    explicit PrintNode(std::shared_ptr<VariableNode> var, int index);
    ~PrintNode() override = default;
    std::string toString() override;
    void accept(ASTVistor& visitor) override;
    std::shared_ptr<VariableNode> getVar();

private:
    std::shared_ptr<VariableNode> var;
};

#endif //SPA_PRINTNODE_H
