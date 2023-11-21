#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#pragma once

#include "StatementNode.h"
#include "ConditionNode.h"
#include <string>

class IfNode : public StatementNode, public std::enable_shared_from_this<IfNode> {
public:
    IfNode(std::shared_ptr<ConditionNode> cond, std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > then,
           std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > el, int index);
    ~IfNode() override = default;
    std::string toString() override;
    void accept(ASTVistor& visitor) override;
    std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > getThenStmtLst();
    std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > getElseStmtLst();
    std::shared_ptr<ConditionNode> getCond();

private:
    std::shared_ptr<ConditionNode> condition;
    std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > thenStmtLst;
    std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > elseStmtLst;
};

#endif //SPA_IFNODE_H
