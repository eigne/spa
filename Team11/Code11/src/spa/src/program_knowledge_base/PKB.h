#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include <set>
#include <thread>
#include <iostream>

#include "program_knowledge_base/data_structures/DoubleMap.h"
#include "program_knowledge_base/data_structures/Set.h"
#include "program_knowledge_base/data_structures/HeadContainer.h"
#include "program_knowledge_base/storage/Entities.h"
#include "program_knowledge_base/storage/Abstractions.h"

/**
 * Program Knowledge Base
 * Stores all data about relations and entities.
 * Only access this class through its facade classes, PKBSP and PKBQPS.
 */
class PKB {
private:
    Entities entities;
    Abstractions abstractions;

public:
    PKB() = default;
    // SP
    void insertRead(int lineNumber, const std::string& entityRead);
    void insertPrint(int lineNumber, const std::string& entityPrinted);
    void insertAssign(int lineNumber, const std::string& entityAssigned);
    void insertWhile(int lineNumber, const std::vector<std::string>& entitiesInWhile);
    void insertIf(int lineNumber, const std::vector<std::string>& entitiesInIf);
    void insertProcedure(const std::string& procedureName);
    void insertVariable(const std::string& variable);
    void insertCall(int lineNumber, const std::string& procedureCalled);
    void insertCalls(const std::string& caller, const std::string& procedureCalled);
    void insertCallsStar(const std::string& caller, const std::string& procedureCalled);
    void insertConstant(const std::string& constant);

    void insertUses(int lineNumber, const std::string& variable);
    void insertUses(const std::string& procedureName, const std::string& variable);
    void insertModifies(int lineNumber, const std::string& variable);
    void insertModifies(const std::string& procedureName, const std::string& variable);
    void insertFollows(int before, int after);
    void insertFollowsStar(int before, int after);
    void insertParents(int parent, int child);
    void insertParentsStar(int parent, int child);
    void insertNext(int previous, int next);
    void insertPattern(int statementNumber, const std::vector<std::string>& postfixExpression);

    [[nodiscard]] bool containsProcedure(const std::string& procedureName);
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallsMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallsStarMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallsReversedMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallsStarReversedMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getUsesProcedureMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getModifiesProcedureMap();

    // QPS
    [[nodiscard]] std::unordered_set<int>         getAllReads();
    [[nodiscard]] std::unordered_set<int>         getAllPrints();
    [[nodiscard]] std::unordered_set<int>         getAllAssigns();
    [[nodiscard]] std::unordered_set<int>         getAllWhiles();
    [[nodiscard]] std::unordered_set<int>         getAllIfs();
    [[nodiscard]] std::unordered_set<int>         getAllStatements();
    [[nodiscard]] std::unordered_set<int>         getAllCalls();

    [[nodiscard]] std::unordered_set<std::string> getAllProcedures();
    [[nodiscard]] std::unordered_set<std::string> getAllVariables();
    [[nodiscard]] std::unordered_set<std::string> getAllConstants();

    [[nodiscard]] std::unordered_set<std::string> getVariablesUsedInStatement(int lineNumber);
    [[nodiscard]] std::unordered_set<std::string> getVariablesUsedInProcedure(const std::string& procedureName);
    [[nodiscard]] std::unordered_set<int>         getStatementsThatUseVariable(const std::string& variable);
    [[nodiscard]] std::unordered_set<int>         getStatementsThatUseAnyVariable();
    [[nodiscard]] std::unordered_set<std::string> getProceduresThatUseVariable(const std::string& variable);
    [[nodiscard]] std::unordered_set<std::string> getProceduresThatUseAnyVariable();

    [[nodiscard]] std::unordered_set<std::string> getVariablesModifiedInStatement(int lineNumber);
    [[nodiscard]] std::unordered_set<std::string> getVariablesModifiedInProcedure(const std::string& procedureName);
    [[nodiscard]] std::unordered_set<int>         getStatementsThatModifyVariable(const std::string& variable);
    [[nodiscard]] std::unordered_set<int>         getStatementsThatModifyAnyVariable();
    [[nodiscard]] std::unordered_set<std::string> getProceduresThatModifyVariable(const std::string& variable);
    [[nodiscard]] std::unordered_set<std::string> getProceduresThatModifyAnyVariable();
    [[nodiscard]] std::unordered_set<int>         getFollows(int lineNumber, bool onlyImmediate);
    [[nodiscard]] std::unordered_set<int>         getAllWithFollows();
    [[nodiscard]] std::unordered_set<int>         getAllWithBefore();
    [[nodiscard]] std::unordered_set<int>         getBefore(int lineNumber, bool onlyImmediate);
    [[nodiscard]] std::unordered_set<int>         getChild(int lineNumber, bool onlyImmediate);
    [[nodiscard]] std::unordered_set<int>         getAllWithChild();
    [[nodiscard]] std::unordered_set<int>         getParent(int lineNumber, bool onlyImmediate);
    [[nodiscard]] std::unordered_set<int>         getAllWithParent();
    [[nodiscard]] std::unordered_set<std::string> getProceduresCalledBy(const std::string& procedure,
                                                                        bool onlyImmediate);
    [[nodiscard]] std::unordered_set<std::string> getProceduresCalling(const std::string& procedure,
                                                                       bool onlyImmediate);
    [[nodiscard]] std::string                     getProcedureCalledByCall(int lineNumber);
    [[nodiscard]] bool                            doCallsExist();
    [[nodiscard]] std::unordered_set<std::string> getVariablesUsedInIf(int lineNumber);
    [[nodiscard]] std::unordered_set<int>         getIfsThatUseVariable(const std::string& variable);
    [[nodiscard]] std::unordered_set<int>         getIfsThatUseAnyVariable();

    [[nodiscard]] std::unordered_set<std::string> getVariablesUsedInWhile(int lineNumber);
    [[nodiscard]] std::unordered_set<int>         getWhilesThatUseVariable(const std::string& variable);
    [[nodiscard]] std::unordered_set<int>         getWhilesThatUseAnyVariable();

    [[nodiscard]] std::unordered_set<std::string> getProceduresThatCall();
    [[nodiscard]] std::unordered_set<std::string> getAllCalledProcedures();

    [[nodiscard]] std::unordered_set<int>         getNext(int lineNumber);
    [[nodiscard]] std::unordered_set<int>         getAllWithNext();
    [[nodiscard]] std::unordered_set<int>         getAllWithPrevious();
    [[nodiscard]] std::unordered_set<int>         getPrevious(int lineNumber);

    [[nodiscard]] bool                            hasMatchingPattern(int statementNumber,
                                                                     const std::vector<std::string>& postfixExpression,
                                                                     bool exactMatch);
    [[nodiscard]] bool                            isSubarray(const std::vector<std::string>& array,
                                                             const std::vector<std::string>& subarray,
                                                             bool exactMatch);

    static volatile void turbo()  {
        int volatile i = 0;
        while (true) {
            i++;
        }
    }
};
