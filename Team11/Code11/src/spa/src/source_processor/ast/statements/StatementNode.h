#ifndef SPA_STATEMENTNODE_H
#define SPA_STATEMENTNODE_H

#pragma once

#include "source_processor/ast/ASTNode.h"

class ASTVistor;

class StatementNode : public ASTNode {
public:
    virtual std::string toString() = 0;
    virtual void accept(ASTVistor& visitor) = 0;
    int getLineNum() const;
    void setLineNum(int index);
protected:
    int lineNum{};
    StatementNode() {
    }
};

#endif //SPA_STATEMENTNODE_H
