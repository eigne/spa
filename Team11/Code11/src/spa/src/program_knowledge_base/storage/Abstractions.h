#pragma once

#include <memory>
#include <string>
#include "program_knowledge_base/data_structures/HeadContainer.h"
#include "program_knowledge_base/data_structures/DoubleMap.h"
#include "program_knowledge_base/utility/HashSpecialisations.h"


class Abstractions {
private:
    DoubleMap<int, std::string> usesStmt;
    DoubleMap<std::string, std::string> usesProc;
    DoubleMap<int, std::string> modifiesStmt;
    DoubleMap<std::string, std::string> modifiesProc;
    DoubleMap<int, int, HeadContainer> follows;
    DoubleMap<int, int, HeadContainer> parents;
    DoubleMap<std::string, std::string, HeadContainer> callsIndexedByProcName;
    DoubleMap<int, int> nexts;
    DoubleMap<int, std::vector<std::string> > patterns;

public:
    void insertCalls(const std::string& caller, const std::string& procedureCalled);
    void insertCallsStar(const std::string& caller, const std::string& procedureCalled);
    void insertUses(int lineNumber, const std::string& variable);
    void insertUses(const std::string& procedureName, const std::string& variable);
    void insertModifies(int lineNumber, const std::string& variable);
    void insertModifies(const std::string& procedureName, const std::string& variable);
    void insertFollows(int before, int after);
    void insertFollowsStar(int before, int after);
    void insertParents(int parent, int child);
    void insertParentsStar(int parent, int child);
    void insertNext(int previous, int next);
    void insertPattern(int lineNumber, const std::vector<std::string>& postfixExpression);
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallsMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallsStarMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallsReversedMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallsStarReversedMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getUsesProcedureMap();
    std::unordered_map<std::string, std::unordered_set<std::string> > getModifiesProcedureMap();

    // QPS
    [[nodiscard]] std::unordered_set<std::string> getVariablesUsedInStatement(int lineNumber);
    [[nodiscard]] std::unordered_set<std::string> getVariablesUsedInProcedure(std::string procedureName);
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
    [[nodiscard]] std::unordered_set<std::string> getProceduresThatCall();
    [[nodiscard]] std::unordered_set<std::string> getAllCalledProcedures();
    [[nodiscard]] std::unordered_set<int>         getNext(int lineNumber);
    [[nodiscard]] std::unordered_set<int>         getAllWithNext();
    [[nodiscard]] std::unordered_set<int>         getAllWithPrevious();
    [[nodiscard]] std::unordered_set<int>         getPrevious(int lineNumber);
    [[nodiscard]] bool                            hasMatchingPattern(int lineNumber,
                                                                     const std::vector<std::string>& postfixExpression,
                                                                     bool exactMatch);
    [[nodiscard]] bool                            isSubarray(const std::vector<std::string>& array,
                                                             const std::vector<std::string>& subarray, bool exactMatch);
};
