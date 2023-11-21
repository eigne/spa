#pragma once

#include <unordered_set>
#include <string>

class ICallsReader {
public:
    virtual std::unordered_set<std::string> getProceduresCalledBy(std::string procedureName, bool onlyImmediate) = 0;
    virtual std::unordered_set<std::string> getProceduresCalling(std::string procedureName, bool onlyImmediate) = 0;
    virtual std::unordered_set<std::string> getProceduresThatCall() = 0;
    virtual std::unordered_set<std::string> getAllCalledProcedures() = 0;
};
