#include "QPSStorageReader.h"

QPSStorageReader::QPSStorageReader(const std::shared_ptr<PKBQPS>& pkb) {
    this->pkb = pkb;
}

std::unordered_set<int> QPSStorageReader::getAllStatements() {
    return pkb->getAllStatements();
}

std::unordered_set<int> QPSStorageReader::getAllReads() {
    return pkb->getAllReads();
}

std::unordered_set<int> QPSStorageReader::getAllPrints() {
    return pkb->getAllPrints();
}

std::unordered_set<int> QPSStorageReader::getAllCalls() {
    return pkb->getAllCalls();
}

std::unordered_set<int> QPSStorageReader::getAllWhiles() {
    return pkb->getAllWhiles();
}

std::unordered_set<int> QPSStorageReader::getAllIfs() {
    return pkb->getAllIfs();
}

std::unordered_set<int> QPSStorageReader::getAllAssigns() {
    return pkb->getAllAssigns();
}

std::unordered_set<std::string> QPSStorageReader::getAllVariables() {
    return pkb->getAllVariables();
}

std::unordered_set<std::string> QPSStorageReader::getAllConstants() {
    return pkb->getAllConstants();
}

std::unordered_set<std::string> QPSStorageReader::getAllProcedures() {
    return pkb->getAllProcedures();
}

std::unordered_set<std::string> QPSStorageReader::getVariablesUsedInStatement(int statementNumber) {
    return pkb->getVariablesUsedInStatement(statementNumber);
}

std::unordered_set<int> QPSStorageReader::getStatementsThatUseVariable(std::string variableName) {
    return pkb->getStatementsThatUseVariable(variableName);
}

std::unordered_set<int> QPSStorageReader::getStatementsThatUseAnyVariable() {
    return pkb->getStatementsThatUseAnyVariable();
}

std::unordered_set<std::string> QPSStorageReader::getVariablesUsedInProcedure(std::string procedureName) {
    return pkb->getVariablesUsedInProcedure(procedureName);
}

std::unordered_set<std::string> QPSStorageReader::getProceduresThatUseVariable(std::string variableName) {
    return pkb->getProceduresThatUseVariable(variableName);
}

std::unordered_set<std::string> QPSStorageReader::getProceduresThatUseAnyVariable() {
    return pkb->getProceduresThatUseAnyVariable();
}

std::unordered_set<std::string> QPSStorageReader::getVariablesModifiedInStatement(int statementNumber) {
    return pkb->getVariablesModifiedInStatement(statementNumber);
}

std::unordered_set<int> QPSStorageReader::getStatementsThatModifyVariable(std::string variableName) {
    return pkb->getStatementsThatModifyVariable(variableName);
}

std::unordered_set<int> QPSStorageReader::getStatementsThatModifyAnyVariable() {
    return pkb->getStatementsThatModifyAnyVariable();
}

std::unordered_set<std::string> QPSStorageReader::getVariablesModifiedInProcedure(std::string procedureName) {
    return pkb->getVariablesModifiedInProcedure(procedureName);
}

std::unordered_set<std::string> QPSStorageReader::getProceduresThatModifyVariable(std::string variableName) {
    return pkb->getProceduresThatModifyVariable(variableName);
}

std::unordered_set<std::string> QPSStorageReader::getProceduresThatModifyAnyVariable() {
    return pkb->getProceduresThatModifyAnyVariable();
}

std::unordered_set<int> QPSStorageReader::getFollows(int statementNumber, bool onlyImmediate) {
    return pkb->getFollows(statementNumber, onlyImmediate);
}

std::unordered_set<int> QPSStorageReader::getBefore(int statementNumber, bool onlyImmediate) {
    return pkb->getBefore(statementNumber, onlyImmediate);
}

std::unordered_set<int> QPSStorageReader::getAllWithFollows() {
    return pkb->getAllWithFollows();
}

std::unordered_set<int> QPSStorageReader::getAllWithBefore() {
    return pkb->getAllWithBefore();
}

std::unordered_set<int> QPSStorageReader::getParent(int statementNumber, bool onlyImmediate) {
    return pkb->getParent(statementNumber, onlyImmediate);
}

std::unordered_set<int> QPSStorageReader::getChild(int statementNumber, bool onlyImmediate) {
    return pkb->getChild(statementNumber, onlyImmediate);
}

std::unordered_set<int> QPSStorageReader::getAllWithParent() {
    return pkb->getAllWithParent();
}

std::unordered_set<int> QPSStorageReader::getAllWithChild() {
    return pkb->getAllWithChild();
}

std::unordered_set<std::string> QPSStorageReader::getProceduresCalledBy(std::string procedureName, bool onlyImmediate) {
    return pkb->getProceduresCalledBy(procedureName, onlyImmediate);
}

std::unordered_set<std::string> QPSStorageReader::getProceduresCalling(std::string procedureName, bool onlyImmediate) {
    return pkb->getProceduresCalling(procedureName, onlyImmediate);
}

std::unordered_set<std::string> QPSStorageReader::getProceduresThatCall() {
    return pkb->getProceduresThatCall();
}

std::unordered_set<std::string> QPSStorageReader::getAllCalledProcedures() {
    return pkb->getAllCalledProcedures();
}

bool QPSStorageReader::hasMatchingPattern(int statementNumber, std::vector<std::string> postfixExpression,
                                          bool exactMatch) {
    return pkb->hasMatchingPattern(statementNumber, postfixExpression, exactMatch);
}

std::unordered_set<std::string> QPSStorageReader::getVariablesUsedInIf(int lineNumber) {
    return pkb->getVariablesUsedInIf(lineNumber);
}

std::unordered_set<int> QPSStorageReader::getIfsThatUseVariable(std::string variable) {
    return pkb->getIfsThatUseVariable(variable);
}

std::unordered_set<int> QPSStorageReader::getIfsThatUseAnyVariable() {
    return pkb->getIfsThatUseAnyVariable();
}

std::unordered_set<std::string> QPSStorageReader::getVariablesUsedInWhile(int lineNumber) {
    return pkb->getVariablesUsedInWhile(lineNumber);
}

std::unordered_set<int> QPSStorageReader::getWhilesThatUseVariable(std::string variable) {
    return pkb->getWhilesThatUseVariable(variable);
}

std::unordered_set<int> QPSStorageReader::getWhilesThatUseAnyVariable() {
    return pkb->getWhilesThatUseAnyVariable();
}

std::string QPSStorageReader::getProcedureCalledByCall(int statementNumber) {
    return pkb->getProcedureCalledByCall(statementNumber);
}

std::unordered_set<int> QPSStorageReader::getNext(int statementNumber) {
    return pkb->getNext(statementNumber);
}

std::unordered_set<int> QPSStorageReader::getPrevious(int statementNumber) {
    return pkb->getPrevious(statementNumber);
}
