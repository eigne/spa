#ifndef SPA_ASSIGNNODE_H
#define SPA_ASSIGNNODE_H

#pragma once

#include "StatementNode.h"
#include <string>
#include "source_processor/ast/expressions/VariableNode.h"
#include "source_processor/ast/expressions/ExpressionNode.h"
#include "source_processor/ast/ASTNode.h"

class AssignNode : public StatementNode, public std::enable_shared_from_this<AssignNode> {
public:
    AssignNode(std::shared_ptr<VariableNode> left, std::shared_ptr<ExpressionNode> right, int index);
    ~AssignNode() override = default;
    std::string toString() override;
    std::shared_ptr<VariableNode> getLeft();
    std::shared_ptr<ExpressionNode> getRight();
    std::vector<std::string> getRightVariables();
    std::vector<std::string> getConstants();
    void accept(ASTVistor& visitor) override;

private:
    std::shared_ptr<VariableNode> leftArg;
    std::shared_ptr<ExpressionNode> rightArg;
};

#endif //SPA_ASSIGNNODE_H
