#include "Abstractions.h"

#include <utility>
#include "commons/ExpressionParser.h"

void Abstractions::insertCalls(const std::string& caller, const std::string& procedureCalled) {
    callsIndexedByProcName.insertBothHeadAndBody(caller, procedureCalled);
}

void Abstractions::insertCallsStar(const std::string& caller, const std::string& procedureCalled) {
    callsIndexedByProcName.insertBoth(caller, procedureCalled);
}

void Abstractions::insertUses(int lineNumber, const std::string& variable) {
    usesStmt.insertBoth(lineNumber, variable);
}

void Abstractions::insertUses(const std::string& procedureName, const std::string& variable) {
    usesProc.insertBoth(procedureName, variable);
}

void Abstractions::insertModifies(int lineNumber, const std::string& variable) {
    modifiesStmt.insertBoth(lineNumber, variable);
}

void Abstractions::insertModifies(const std::string& procedureName, const std::string& variable) {
    modifiesProc.insertBoth(procedureName, variable);
}

void Abstractions::insertFollows(int before, int after) {
    follows.insertBothHeadAndBody(before, after);
}

void Abstractions::insertFollowsStar(int before, int after) {
    follows.insertBoth(before, after);
}

void Abstractions::insertParents(int parent, int child) {
    parents.insertBothHeadAndBody(parent, child);
}

void Abstractions::insertParentsStar(int parent, int child) {
    parents.insertBoth(parent, child);
}

void Abstractions::insertNext(int previous, int next) {
    nexts.insertBoth(previous, next);
}

void Abstractions::insertPattern(int lineNumber, const std::vector<std::string>& postfixExpression) {
    patterns.insertBoth(lineNumber, postfixExpression);
}

std::unordered_map<std::string, std::unordered_set<std::string> > Abstractions::getCallsMap() {
    std::unordered_map<std::string, std::unordered_set<std::string> > result;
    auto callsKeys{ callsIndexedByProcName.getAllForwardKeys() };
    for (const auto& key : callsKeys) {
        result.emplace(key, callsIndexedByProcName.lookupForward(key).getHead());
    }
    return result;
}

std::unordered_map<std::string, std::unordered_set<std::string> > Abstractions::getCallsStarMap() {
    std::unordered_map<std::string, std::unordered_set<std::string> > result;
    auto callsKeys{ callsIndexedByProcName.getAllForwardKeys() };
    for (const auto& key : callsKeys) {
        result.emplace(key, callsIndexedByProcName.lookupForward(key).getBody());
    }
    return result;
}

std::unordered_map<std::string, std::unordered_set<std::string> > Abstractions::getCallsReversedMap() {
    std::unordered_map<std::string, std::unordered_set<std::string> > result;
    auto callsKeys{ callsIndexedByProcName.getAllBackwardKeys() };
    for (const auto& key : callsKeys) {
        result.emplace(key, callsIndexedByProcName.lookupBackward(key).getHead());
    }
    return result;
}

std::unordered_map<std::string, std::unordered_set<std::string> > Abstractions::getCallsStarReversedMap() {
    std::unordered_map<std::string, std::unordered_set<std::string> > result;
    auto callsKeys{ callsIndexedByProcName.getAllBackwardKeys() };
    for (const auto& key : callsKeys) {
        result.emplace(key, callsIndexedByProcName.lookupBackward(key).getBody());
    }
    return result;
}

std::unordered_map<std::string, std::unordered_set<std::string> > Abstractions::getUsesProcedureMap() {
    return usesProc.getForwardMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > Abstractions::getModifiesProcedureMap() {
    return modifiesProc.getForwardMap();
}

std::unordered_set<std::string> Abstractions::getVariablesUsedInStatement(int lineNumber) {
    return usesStmt.lookupForward(lineNumber);
}

std::unordered_set<std::string> Abstractions::getVariablesUsedInProcedure(std::string procedureName) {
    return usesProc.lookupForward(std::move(procedureName));
}

std::unordered_set<int> Abstractions::getStatementsThatUseVariable(const std::string& variable) {
    return usesStmt.lookupBackward(variable);
}

std::unordered_set<int> Abstractions::getStatementsThatUseAnyVariable() {
    return usesStmt.getAllForwardKeys();
}


std::unordered_set<std::string> Abstractions::getProceduresThatUseVariable(const std::string& variable) {
    return usesProc.lookupBackward(variable);
}

std::unordered_set<std::string> Abstractions::getProceduresThatUseAnyVariable() {
    return usesProc.getAllForwardKeys();
}

std::unordered_set<std::string> Abstractions::getVariablesModifiedInStatement(int lineNumber) {
    return modifiesStmt.lookupForward(lineNumber);
}

std::unordered_set<std::string> Abstractions::getVariablesModifiedInProcedure(const std::string& procedureName) {
    return modifiesProc.lookupForward(procedureName);
}

std::unordered_set<int> Abstractions::getStatementsThatModifyVariable(const std::string& variable) {
    return modifiesStmt.lookupBackward(variable);
}

std::unordered_set<int> Abstractions::getStatementsThatModifyAnyVariable() {
    return modifiesStmt.getAllForwardKeys();
}

std::unordered_set<std::string> Abstractions::getProceduresThatModifyVariable(const std::string& variable) {
    return modifiesProc.lookupBackward(variable);
}

std::unordered_set<std::string> Abstractions::getProceduresThatModifyAnyVariable() {
    return modifiesProc.getAllForwardKeys();
}

std::unordered_set<int> Abstractions::getFollows(int lineNumber, bool onlyImmediate) {
    if (onlyImmediate) {
        return follows.lookupForward(lineNumber).getHead();
    }
    return follows.lookupForward(lineNumber).getBody();
}

std::unordered_set<int> Abstractions::getAllWithFollows() {
    return follows.getAllForwardKeys();
}

std::unordered_set<int> Abstractions::getBefore(int lineNumber, bool onlyImmediate) {
    if (onlyImmediate) {
        return follows.lookupBackward(lineNumber).getHead();
    }
    return follows.lookupBackward(lineNumber).getBody();
}

std::unordered_set<int> Abstractions::getAllWithBefore() {
    return follows.getAllBackwardKeys();
}

std::unordered_set<int> Abstractions::getChild(int lineNumber, bool onlyImmediate) {
    if (onlyImmediate) {
        return parents.lookupForward(lineNumber).getHead();
    }
    return parents.lookupForward(lineNumber).getBody();
}

std::unordered_set<int> Abstractions::getAllWithChild() {
    return parents.getAllForwardKeys();
}

std::unordered_set<int> Abstractions::getParent(int lineNumber, bool onlyImmediate) {
    if (onlyImmediate) {
        return parents.lookupBackward(lineNumber).getHead();
    }
    return parents.lookupBackward(lineNumber).getBody();
}

std::unordered_set<int> Abstractions::getAllWithParent() {
    return parents.getAllBackwardKeys();
}

std::unordered_set<std::string> Abstractions::getProceduresCalledBy(const std::string& procedure, bool onlyImmediate) {
    if (onlyImmediate) {
        return callsIndexedByProcName.lookupForward(procedure).getHead();
    }
    return callsIndexedByProcName.lookupForward(procedure).getBody();
}

std::unordered_set<std::string> Abstractions::getProceduresCalling(const std::string& procedure, bool onlyImmediate) {
    if (onlyImmediate) {
        return callsIndexedByProcName.lookupBackward(procedure).getHead();
    }
    return callsIndexedByProcName.lookupBackward(procedure).getBody();
}

std::unordered_set<std::string> Abstractions::getProceduresThatCall() {
    return callsIndexedByProcName.getAllForwardKeys();
}

std::unordered_set<std::string> Abstractions::getAllCalledProcedures() {
    return callsIndexedByProcName.getAllBackwardKeys();
}

std::unordered_set<int> Abstractions::getNext(int lineNumber) {
    return nexts.lookupForward(lineNumber);
}

std::unordered_set<int> Abstractions::getAllWithNext() {
    return nexts.getAllForwardKeys();
}

std::unordered_set<int> Abstractions::getAllWithPrevious() {
    return nexts.getAllBackwardKeys();
}

std::unordered_set<int> Abstractions::getPrevious(int lineNumber) {
    return nexts.lookupBackward(lineNumber);
}

bool Abstractions::hasMatchingPattern(int lineNumber, const std::vector<std::string>& postfixExpression, bool exactMatch) {
    auto postfixSet{patterns.lookupForward(lineNumber) };
    if (postfixSet.empty()) return false;
    auto postfixVector{*postfixSet.begin() };
    return isSubarray(postfixVector, postfixExpression, exactMatch);
}

bool Abstractions::isSubarray(const std::vector<std::string>& array, const std::vector<std::string>& subarray,
                     bool isExactMatch) {
    if (!isExactMatch) {
        return ExpressionParser::isSubarray(array, subarray);
    }
    return array == subarray;
}
