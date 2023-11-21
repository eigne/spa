#ifndef SPA_CONSTANTNODE_H
#define SPA_CONSTANTNODE_H

#pragma once

#include "ExpressionNode.h"
#include <string>

class ConstantNode : public ASTNode {
public:
    explicit ConstantNode(std::string value);
    ~ConstantNode() override = default;
    std::string toString() override;

private:
    std::string value;
};


#endif //SPA_CONSTANTNODE_H
