#pragma once

#include <memory>
#include <iostream>
#include <utility>
#include "program_knowledge_base/PKB.h"
#include "program_knowledge_base/utility/PKBLogging.h"

class PKBSP {
private:
    std::shared_ptr<PKB> pkb;

public:
    explicit PKBSP(std::shared_ptr<PKB> pkb) : pkb{std::move(pkb)}
    {}

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

    void insertUses(int statementNumber, const std::string& variable);
    void insertUses(const std::string& procedureName, const std::string& variable);
    void insertModifies(int statementNumber, const std::string& variable);
    void insertModifies(const std::string& procedureName, const std::string& variable);
    void insertFollows(int before, int after);
    void insertFollowsStar(int before, int after);
    void insertParents(int parent, int child);
    void insertParentsStar(int parent, int child);
    void insertNext(int previous, int next);
    void insertPattern(int statementNumber, const std::vector<std::string>& postfixExpression);

    [[nodiscard]] bool containsProcedure(const std::string& procedureName);
    [[nodiscard]] std::unordered_map<std::string, std::unordered_set<std::string> > getCallsMap();
    [[nodiscard]] std::unordered_map<std::string, std::unordered_set<std::string> > getCallsStarMap();
    [[nodiscard]] std::unordered_map<std::string, std::unordered_set<std::string> > getCallsReversedMap();
    [[nodiscard]] std::unordered_map<std::string, std::unordered_set<std::string> > getCallsStarReversedMap();
    [[nodiscard]] std::unordered_map<std::string, std::unordered_set<std::string> > getUsesProceduresMap();
    [[nodiscard]] std::unordered_map<std::string, std::unordered_set<std::string> > getModifiesProceduresMap();
};
