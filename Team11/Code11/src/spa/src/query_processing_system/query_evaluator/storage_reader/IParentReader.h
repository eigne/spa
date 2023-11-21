#pragma once

#include <unordered_set>
#include <string>

class IParentReader {
public:
    virtual std::unordered_set<int> getParent(int statementNumber, bool onlyImmediate) = 0;
    virtual std::unordered_set<int> getChild(int statementNumber, bool onlyImmediate) = 0;
    virtual std::unordered_set<int> getAllWithParent() = 0;
    virtual std::unordered_set<int> getAllWithChild() = 0;
};
