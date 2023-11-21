#pragma once

#include <unordered_set>
#include <string>

class IFollowsReader {
public:
    virtual std::unordered_set<int> getFollows(int statementNumber, bool onlyImmediate) = 0;
    virtual std::unordered_set<int> getBefore(int statementNumber, bool onlyImmediate) = 0;
    virtual std::unordered_set<int> getAllWithFollows() = 0;
    virtual std::unordered_set<int> getAllWithBefore() = 0;
};
