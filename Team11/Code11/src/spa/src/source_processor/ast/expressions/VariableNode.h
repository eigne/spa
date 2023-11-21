#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

#pragma once

#include "ExpressionNode.h"
#include <string>

class VariableNode : public ASTNode {
public:
    VariableNode(std::string name, int lineNum);
    ~VariableNode() override = default;
    std::string toString() override;
    std::string getVarName();
    int getAssociatedLineNum() const;

private:
    std::string name;
    int associatedLineNum;
};

#endif //SPA_VARIABLENODE_H
