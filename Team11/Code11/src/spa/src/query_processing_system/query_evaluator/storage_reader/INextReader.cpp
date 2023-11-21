#include "INextReader.h"

std::unordered_set<int> INextReader::queryNextStar(int statementNumber, const std::unordered_set<int>& visitedNodes, bool isNext) {
    auto& cache = isNext ? nextStarCache : previousStarCache;

    if (cache.count(statementNumber) && visitedNodes.empty()) {
        return cache.at(statementNumber);
    }

    std::unordered_set<int> nextVisited = visitedNodes;
    nextVisited.insert(statementNumber);

    bool hasInserted = false;
    for (auto visitedNode : visitedNodes) {
        if (!cache.count(visitedNode)) {
            cache.insert({ visitedNode, {} });
        }
        if (!cache.at(visitedNode).count(statementNumber)) {
            hasInserted = true;
            cache.at(visitedNode).insert(statementNumber);
        }
    }

    for (auto element : isNext ? getNext(statementNumber) : getPrevious(statementNumber)) {
        if (cache.count(element) && !cache.at(element).count(statementNumber)) {
            auto toInsert = cache.at(element);
            toInsert.insert(element);
            for (auto visitedNode : visitedNodes) {
                cache.at(visitedNode).insert(toInsert.begin(), toInsert.end());
            }
            if (!cache.count(statementNumber)) {
                cache.insert({ statementNumber, {} });
            }
            cache.at(statementNumber).insert(toInsert.begin(), toInsert.end());
        } else {
            if (visitedNodes.empty() || hasInserted) {
                queryNextStar(element, nextVisited, isNext);
            }
        }
    }

    return cache[statementNumber];
}
