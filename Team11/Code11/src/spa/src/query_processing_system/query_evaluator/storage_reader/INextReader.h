#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

class INextReader {
public:
    std::unordered_map<int, std::unordered_set<int>> nextStarCache;
    std::unordered_map<int, std::unordered_set<int>> previousStarCache;

    INextReader() : nextStarCache({}), previousStarCache({}) {}

    virtual std::unordered_set<int> getNext(int statementNumber) = 0;
    virtual std::unordered_set<int> getPrevious(int statementNumber) = 0;

    std::unordered_set<int> queryNextStar(int statementNumber, const std::unordered_set<int>& visitedNodes, bool isNext);
};
