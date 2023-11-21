#ifndef SPA_PROCEDURENODE_H
#define SPA_PROCEDURENODE_H

#pragma once

#include <string>
#include "statements/StatementNode.h"

class ProcedureNode : public StatementNode {
public:
    ProcedureNode(std::string procName, std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > stmtLst);
    ~ProcedureNode() override = default;
    std::string toString() override;
    std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > getStmtLst();
    std::string getName();

private:
    std::string name;
    std::shared_ptr<std::vector<std::shared_ptr<StatementNode> > > stmtLst;
    void accept(ASTVistor& visitor)override {
    }
};


#endif //SPA_PROCEDURENODE_H
