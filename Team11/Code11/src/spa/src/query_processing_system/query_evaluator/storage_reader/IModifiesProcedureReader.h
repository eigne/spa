#pragma once

#include <unordered_set>
#include <string>

class IModifiesProcedureReader {
public:
    virtual std::unordered_set<std::string> getVariablesModifiedInProcedure(std::string procedureName) = 0;
    virtual std::unordered_set<std::string> getProceduresThatModifyVariable(std::string variableName) = 0;
    virtual std::unordered_set<std::string> getProceduresThatModifyAnyVariable() = 0;
};
