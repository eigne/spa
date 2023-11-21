#pragma once

#include <unordered_set>
#include <string>

class IUsesStatementReader {
public:
    virtual std::unordered_set<std::string> getVariablesUsedInStatement(int statementNumber) = 0;
    virtual std::unordered_set<int> getStatementsThatUseVariable(std::string variableName) = 0;
    virtual std::unordered_set<int> getStatementsThatUseAnyVariable() = 0;
};
