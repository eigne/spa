#pragma once

#include <string>
#include <unordered_set>

class IIfPatternReader {
public:
    virtual std::unordered_set<std::string> getVariablesUsedInIf(int lineNumber) = 0;
    virtual std::unordered_set<int> getIfsThatUseVariable(std::string variable) = 0;
    virtual std::unordered_set<int> getIfsThatUseAnyVariable() = 0;
};
