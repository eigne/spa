#include "PKBRelationshipStub.h"

void PKBRelationshipStub::insertModifiesS(int stmtNo, const std::string& varName) {
    modifiesS[stmtNo].insert(varName);
}

void PKBRelationshipStub::insertModifiesP(const std::string& procName, const std::string& varName) {
    modifiesP[procName].insert(varName);
}

void PKBRelationshipStub::insertUsesS(int stmtNo, const std::string& varName) {
    usesS[stmtNo].insert(varName);
}

void PKBRelationshipStub::insertUsesP(const std::string& procName, const std::string& varName) {
    usesP[procName].insert(varName);
}

void PKBRelationshipStub::insertFollows(int prev, int next) {
    follows[prev].insert(next);
}

void PKBRelationshipStub::insertParents(int parent, int child) {
    parents[parent].insert(child);
}

void PKBRelationshipStub::insertFollowsT(int prev, int next) {
    followsT[prev].insert(next);
}

void PKBRelationshipStub::insertParentsT(int parent, int child) {
    parentsT[parent].insert(child);
}

void PKBRelationshipStub::insertCalls(const std::string& caller, const std::string& callee) {
    calls[caller].insert(callee);
    procedures.insert(caller);
    procedures.insert(callee);
}

void PKBRelationshipStub::insertCallsT(const std::string& caller, const std::string& callee) {
    callsT[caller].insert(callee);
}

void PKBRelationshipStub::insertNext(int prev, int next) {
    this->next[prev].insert(next);
}

bool PKBRelationshipStub::containsProcedure(const std::string& procName) {
    return procedures.find(procName) != procedures.end();
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKBRelationshipStub::getCalls() {
    return calls;
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKBRelationshipStub::getCallsT() {
    return callsT;
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKBRelationshipStub::getUsesP() {
    return usesP;
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKBRelationshipStub::getModifiesP() {
    return modifiesP;
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKBRelationshipStub::getCallReversedMap() {
    std::unordered_map<std::string, std::unordered_set<std::string>> reversedMap;
    for (auto& pair : calls) {
        for (auto& callee : pair.second) {
            reversedMap[callee].insert(pair.first);
        }
    }
    return reversedMap;
}
