#pragma once

#include <string>
#include <unordered_set>

class IWhilePatternReader {
public:
    virtual std::unordered_set<std::string> getVariablesUsedInWhile(int lineNumber) = 0;
    virtual std::unordered_set<int> getWhilesThatUseVariable(std::string variable) = 0;
    virtual std::unordered_set<int> getWhilesThatUseAnyVariable() = 0;
};
