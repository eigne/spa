#ifndef SPA_CONDITIONNODE_H
#define SPA_CONDITIONNODE_H

#pragma once

#include <string>
#include "source_processor/ast/ASTNode.h"
#include "source_processor/ast/expressions/ExpressionNode.h"

class ConditionNode : public ASTNode {
public:
    ConditionNode(std::vector<std::string> varList, std::vector<std::string> constList, std::string cond);
    ~ConditionNode() override = default;
    std::string toString() override;
    std::vector<std::string> getVariables();
    std::vector<std::string> getConstants();

private:
    std::string condOp;
    std::vector<std::string> varList;
    std::vector<std::string> constList;
};

#endif //SPA_CONDITIONNODE_H
