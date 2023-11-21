#pragma once

#include <unordered_set>
#include <string>

class IModifiesStatementReader {
public:
    virtual std::unordered_set<std::string> getVariablesModifiedInStatement(int statementNumber) = 0;
    virtual std::unordered_set<int> getStatementsThatModifyVariable(std::string variableName) = 0;
    virtual std::unordered_set<int> getStatementsThatModifyAnyVariable() = 0;
};
