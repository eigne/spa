#pragma once

#include <memory>
#include <iostream>
#include <utility>
#include "program_knowledge_base/PKB.h"
#include "program_knowledge_base/utility/PKBLogging.h"

class PKBQPS {
private:
    std::shared_ptr<PKB> pkb;

public:
    explicit PKBQPS(std::shared_ptr<PKB> pkb) : pkb{std::move(pkb)}
    {}

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
    [[nodiscard]] std::unordered_set<int>         getBefore(int lineNumber, bool onlyImmediate);
    [[nodiscard]] std::unordered_set<int>         getAllWithBefore();
    [[nodiscard]] std::unordered_set<int>         getChild(int lineNumber, bool onlyImmediate);
    [[nodiscard]] std::unordered_set<int>         getAllWithChild();
    [[nodiscard]] std::unordered_set<int>         getParent(int lineNumber, bool onlyImmediate);
    [[nodiscard]] std::unordered_set<int>         getAllWithParent();
    [[nodiscard]] std::unordered_set<std::string> getProceduresCalledBy(const std::string& procedure,
                                                                        bool onlyImmediate);
    [[nodiscard]] std::unordered_set<std::string> getProceduresCalling(const std::string& procedure,
                                                                       bool onlyImmediate);
    [[nodiscard]] bool                            doCallsExist();
    [[nodiscard]] std::string                     getProcedureCalledByCall(int lineNumber);
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

    [[nodiscard]] bool                            hasMatchingPattern(int lineNumber,
                                                                     const std::vector<std::string>& postfixExpression,
                                                                     bool exactMatch);
};
