#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

class IRelationshipWriter {
public:
    virtual void insertModifiesS(int stmtNo, const std::string& varName) = 0;
    virtual void insertModifiesP(const std::string& procName, const std::string& varName) = 0;
    virtual void insertUsesS(int stmtNo, const std::string& varName) = 0;
    virtual void insertUsesP(const std::string& procName, const std::string& varName) = 0;
    virtual void insertFollows(int prev, int next) = 0;
    virtual void insertFollowsT(int prev, int next) = 0;
    virtual void insertParents(int parent, int child) = 0;
    virtual void insertParentsT(int parent, int child) = 0;
    virtual void insertCalls(const std::string& caller, const std::string& callee) = 0;
    virtual void insertCallsT(const std::string& caller, const std::string& callee) = 0;
    virtual void insertNext(int prev, int next) = 0;

    virtual bool containsProcedure(const std::string& procName) = 0;
    virtual std::unordered_map<std::string, std::unordered_set<std::string> > getCalls() = 0;
    virtual std::unordered_map<std::string, std::unordered_set<std::string> > getCallReversedMap() = 0;
    virtual std::unordered_map<std::string, std::unordered_set<std::string> > getCallsT() = 0;
    virtual std::unordered_map<std::string, std::unordered_set<std::string> > getUsesP() = 0;
    virtual std::unordered_map<std::string, std::unordered_set<std::string> > getModifiesP() = 0;
};
