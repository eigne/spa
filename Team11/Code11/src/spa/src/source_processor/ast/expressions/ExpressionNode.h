#ifndef SPA_EXPRESSIONNODE_H
#define SPA_EXPRESSIONNODE_H

#pragma once

#include <string>
#include "commons/ExpressionParser.h"
#include "source_processor/ast/ASTNode.h"

class ExpressionNode : public ASTNode {
public:
    ExpressionNode(const std::string& op, std::vector<std::string> var, std::vector<std::string>c, int index);
    ~ExpressionNode() override = default;
    std::string toString() override;
    std::vector<std::string> getSortedOp();
    std::vector<std::string> getVariables();
    std::vector<std::string> getConstants();
    int getLineNum() const;


private:
    std::vector<std::string> sortedOp;
    std::string op;
    std::vector<std::string> variables;
    std::vector<std::string> constants;
    int lineNum;
};


#endif //SPA_EXPRESSIONNODE_H
