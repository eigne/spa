#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

#pragma once

#include "ASTNode.h"
#include "ProcedureNode.h"

class ProgramNode : public ASTNode {
public:
    explicit ProgramNode(std::shared_ptr<std::vector<std::shared_ptr<ProcedureNode> > > lst);
    ~ProgramNode() override = default;
    std::shared_ptr<std::vector<std::shared_ptr<ProcedureNode> > > getProcLst();
    std::string toString() override;
private:
    std::shared_ptr<std::vector<std::shared_ptr<ProcedureNode> > > lst;
};

#endif //SPA_PROGRAMNODE_H
