#pragma once

#include <unordered_set>
#include <string>

class IUsesProcedureReader {
public:
    virtual std::unordered_set<std::string> getVariablesUsedInProcedure(std::string procedureName) = 0;
    virtual std::unordered_set<std::string> getProceduresThatUseVariable(std::string variableName) = 0;
    virtual std::unordered_set<std::string> getProceduresThatUseAnyVariable() = 0;
};
