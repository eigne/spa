#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#pragma once

#include "StatementNode.h"
#include "ConditionNode.h"
#include <string>

class WhileNode : public StatementNode, public std::enable_shared_from_this<WhileNode> {
public:
    WhileNode(std::shared_ptr<ConditionNode> cond, std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > sl,
              int index);
    ~WhileNode() override = default;
    std::shared_ptr<ConditionNode> getCond();
    std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > getStmtLst();
    std::string toString() override;
    void accept(ASTVistor& visitor) override;

private:
    std::shared_ptr<ConditionNode> condition;
    std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > stmtLst;
};

#endif //SPA_WHILENODE_H
