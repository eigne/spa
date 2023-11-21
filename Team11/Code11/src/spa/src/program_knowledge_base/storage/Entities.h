#pragma once

#include <string>
#include <vector>
#include "program_knowledge_base/data_structures/DoubleMap.h"
#include "program_knowledge_base/data_structures/Set.h"

class Entities {
private:
    DoubleMap<int, std::string> reads;
    DoubleMap<int, std::string> prints;
    DoubleMap<int, std::string> assigns;
    DoubleMap<int, std::string> whiles;
    DoubleMap<int, std::string> ifs;
    DoubleMap<int, std::string> calls;

    Set<std::string> procedures;
    Set<std::string> variables;
    Set<std::string> constants;

public:
    // SP
    void insertRead(int lineNumber, const std::string& entityRead);
    void insertPrint(int lineNumber, const std::string& entityPrinted);
    void insertAssign(int lineNumber, const std::string& entityAssigned);
    void insertWhile(int lineNumber, const std::vector<std::string>& entitiesInWhile);
    void insertIf(int lineNumber, const std::vector<std::string>& entitiesInIf);
    void insertProcedure(const std::string& procedureName);
    void insertVariable(const std::string& variable);
    void insertCall(int lineNumber, const std::string& procedureCalled);
    void insertConstant(const std::string& constant);
    [[nodiscard]] bool containsProcedure(const std::string& procedureName);

    // QPS
    [[nodiscard]] std::unordered_set<int>         getAllReads();
    [[nodiscard]] std::unordered_set<int>         getAllPrints();
    [[nodiscard]] std::unordered_set<int>         getAllAssigns();
    [[nodiscard]] std::unordered_set<int>         getAllWhiles();
    [[nodiscard]] std::unordered_set<int>         getAllIfs();
    [[nodiscard]] std::unordered_set<int>         getAllStatements();
    [[nodiscard]] std::unordered_set<int>         getAllCalls();
    [[nodiscard]] bool                            doCallsExist();

    [[nodiscard]] std::unordered_set<std::string> getAllProcedures();
    [[nodiscard]] std::unordered_set<std::string> getAllVariables();
    [[nodiscard]] std::unordered_set<std::string> getAllConstants();
    [[nodiscard]] std::string                     getProcedureCalledByCall(int statementNumber);
    [[nodiscard]] std::unordered_set<std::string> getVariablesUsedInIf(int lineNumber);
    [[nodiscard]] std::unordered_set<int>         getIfsThatUseVariable(const std::string& variable);
    [[nodiscard]] std::unordered_set<int>         getIfsThatUseAnyVariable();
    [[nodiscard]] std::unordered_set<std::string> getVariablesUsedInWhile(int lineNumber);
    [[nodiscard]] std::unordered_set<int>         getWhilesThatUseVariable(const std::string& variable);
    [[nodiscard]] std::unordered_set<int>         getWhilesThatUseAnyVariable();
};

