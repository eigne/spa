#include "RelationshipWriter.h"

RelationshipWriter::RelationshipWriter(std::shared_ptr<PKBSP>& pkb) {
    this->pkb = pkb;
}

void RelationshipWriter::insertUsesS(int stmtNo, const std::string& variableName) {
    pkb->insertUses(stmtNo, variableName);
}

void RelationshipWriter::insertModifiesS(int stmtNo, const std::string& variableName) {
    pkb->insertModifies(stmtNo, variableName);
}

void RelationshipWriter::insertUsesP(const std::string& procName, const std::string& variableName) {
    pkb->insertUses(procName, variableName);
}

void RelationshipWriter::insertModifiesP(const std::string& procName, const std::string& variableName) {
    pkb->insertModifies(procName, variableName);
}

void RelationshipWriter::insertFollows(int prev, int next) {
    pkb->insertFollows(prev, next);
}

void RelationshipWriter::insertFollowsT(int prev, int next) {
    pkb->insertFollowsStar(prev, next);
}

void RelationshipWriter::insertParents(int parent, int child) {
    pkb->insertParents(parent, child);
}

void RelationshipWriter::insertParentsT(int parent, int child) {
    pkb->insertParentsStar(parent, child);
}

void RelationshipWriter::insertCalls(const std::string& caller, const std::string& callee) {
    pkb->insertCalls(caller, callee);
}

void RelationshipWriter::insertCallsT(const std::string& caller, const std::string& callee) {
    pkb->insertCallsStar(caller, callee);
}

void RelationshipWriter::insertNext(int prev, int next) {
    pkb->insertNext(prev, next);
}

// Read from PKB
bool RelationshipWriter::containsProcedure(const std::string& procName) {
    return pkb->containsProcedure(procName);
}

std::unordered_map<std::string, std::unordered_set<std::string> > RelationshipWriter::getCalls() {
    return pkb->getCallsMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > RelationshipWriter::getCallsT() {
    return pkb->getCallsStarMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > RelationshipWriter::getUsesP() {
    return pkb->getUsesProceduresMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > RelationshipWriter::getModifiesP() {
    return pkb->getModifiesProceduresMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > RelationshipWriter::getCallReversedMap() {
    return pkb->getCallsReversedMap();
}
