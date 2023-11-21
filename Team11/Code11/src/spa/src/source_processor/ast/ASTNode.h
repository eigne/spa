#ifndef SPA_ASTNODE_H
#define SPA_ASTNODE_H

#pragma once

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual std::string toString() = 0;
};


#endif //SPA_ASTNODE_H
