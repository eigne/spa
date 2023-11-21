#include "Entities.h"
#include "program_knowledge_base/exceptions/PKBException.h"
#include "program_knowledge_base/utility/PKBStatics.h"

void Entities::insertRead(int lineNumber, const std::string& entityRead) {
    reads.insertBoth(lineNumber, entityRead);
}

void Entities::insertPrint(int lineNumber, const std::string& entityPrinted) {
    prints.insertBoth(lineNumber, entityPrinted);
}

void Entities::insertAssign(int lineNumber, const std::string& entityAssigned) {
    assigns.insertBoth(lineNumber, entityAssigned);
}

void Entities::insertWhile(int lineNumber, const std::vector<std::string>& entitiesInWhile) {
    if (entitiesInWhile.empty()) {
        whiles.insertEmpty(lineNumber);
        return;
    }
    for (const auto& entity : entitiesInWhile) {
        whiles.insertBoth(lineNumber, entity);
    }
}

void Entities::insertIf(int lineNumber, const std::vector<std::string>& entitiesInIf) {
    if (entitiesInIf.empty()) {
        ifs.insertEmpty(lineNumber);
        return;
    }
    for (const auto& entity : entitiesInIf) {
        ifs.insertBoth(lineNumber, entity);
    }
}

void Entities::insertProcedure(const std::string& procedureName) {
    procedures.insertElement(procedureName);
}

void Entities::insertVariable(const std::string& variable) {
    variables.insertElement(variable);
}

void Entities::insertCall(int lineNumber, const std::string& procedureCalled) {
    calls.insertBoth(lineNumber, procedureCalled);
}

void Entities::insertConstant(const std::string& constant) {
    constants.insertElement(constant);
}

bool Entities::containsProcedure(const std::string& procedureName) {
    return procedures.findElement(procedureName);
}

std::unordered_set<int> Entities::getAllReads() {
    return reads.getAllForwardKeys();
}

std::unordered_set<int> Entities::getAllPrints() {
    return prints.getAllForwardKeys();
}

std::unordered_set<int> Entities::getAllAssigns() {
    return assigns.getAllForwardKeys();
}

std::unordered_set<int> Entities::getAllWhiles() {
    return whiles.getAllForwardKeys();
}

std::unordered_set<int> Entities::getAllIfs() {
    return ifs.getAllForwardKeys();
}

std::unordered_set<int> Entities::getAllStatements() {
    std::unordered_set<int> statements;
    std::unordered_set<int> readsStatements{ getAllReads() };
    std::unordered_set<int> printsStatements{ getAllPrints() };
    std::unordered_set<int> assignStatements{ getAllAssigns() };
    std::unordered_set<int> whileStatements{ getAllWhiles() };
    std::unordered_set<int> ifStatements{ getAllIfs() };
    std::unordered_set<int> callStatements{ getAllCalls() };

    statements.insert(readsStatements.begin(), readsStatements.end());
    statements.insert(printsStatements.begin(), printsStatements.end());
    statements.insert(assignStatements.begin(), assignStatements.end());
    statements.insert(callStatements.begin(), callStatements.end());
    statements.insert(ifStatements.begin(), ifStatements.end());
    statements.insert(whileStatements.begin(), whileStatements.end());
    return statements;
}

std::unordered_set<int> Entities::getAllCalls() {
    return calls.getAllForwardKeys();
}

bool Entities::doCallsExist() {
    return !calls.empty();
}

std::unordered_set<std::string> Entities::getAllProcedures() {
    return procedures.getAllElements();
}

std::unordered_set<std::string> Entities::getAllVariables() {
    return variables.getAllElements();
}

std::unordered_set<std::string> Entities::getAllConstants() {
    return constants.getAllElements();
}

std::string Entities::getProcedureCalledByCall(int statementNumber) {
    auto procedureCalled{calls.lookupForward(statementNumber)};
    if (procedureCalled.empty()) {
        throw PKBException{PKBErrors::CALLNOTFOUND};
    }
    return *procedureCalled.begin();
}

std::unordered_set<std::string> Entities::getVariablesUsedInIf(int lineNumber) {
    return ifs.lookupForward(lineNumber);
}

std::unordered_set<int> Entities::getIfsThatUseVariable(const std::string& variable) {
    return ifs.lookupBackward(variable);
}

std::unordered_set<int> Entities::getIfsThatUseAnyVariable() {
    std::unordered_set<int> result;
    for (const auto& variableUsedByOneOrMoreIfs : ifs.getAllBackwardKeys()) {
        auto ifsThatUseVariable{ifs.lookupBackward(variableUsedByOneOrMoreIfs)};
        result.insert(ifsThatUseVariable.begin(), ifsThatUseVariable.end());
    }
    return result;
}

std::unordered_set<std::string> Entities::getVariablesUsedInWhile(int lineNumber) {
    return whiles.lookupForward(lineNumber);
}

std::unordered_set<int> Entities::getWhilesThatUseVariable(const std::string& variable) {
    return whiles.lookupBackward(variable);
}

std::unordered_set<int> Entities::getWhilesThatUseAnyVariable() {
    std::unordered_set<int> result;
    for (const auto& variableUsedByOneOrMoreWhiles : whiles.getAllBackwardKeys()) {
        auto whilesThatUseVariable{whiles.lookupBackward(variableUsedByOneOrMoreWhiles)};
        result.insert(whilesThatUseVariable.begin(), whilesThatUseVariable.end());
    }
    return result;
}

