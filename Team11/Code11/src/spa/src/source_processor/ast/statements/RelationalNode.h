#ifndef SPA_RELATIONALNODE_H
#define SPA_RELATIONALNODE_H

#pragma once

#include <string>
#include "source_processor/ast/ASTNode.h"
#include "source_processor/ast/expressions/ExpressionNode.h"

/**
 *  RelationalNode refers only to " < > <= >= != ==" relations.
 */
class RelationalNode : public ASTNode {
public:
    RelationalNode(std::string op, std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right);
    ~RelationalNode() override = default;
    std::string toString() override;
    std::shared_ptr<ExpressionNode> getLeft();
    std::shared_ptr<ExpressionNode> getRight();
    std::string getOp();
    std::shared_ptr<std::vector<std::string> > getVariables();
    std::shared_ptr<std::vector<std::string> > getConstants();

private:
    std::string op;
    std::shared_ptr<ExpressionNode> leftArg;
    std::shared_ptr<ExpressionNode> rightArg;
};


#endif //SPA_RELATIONALNODE_H
