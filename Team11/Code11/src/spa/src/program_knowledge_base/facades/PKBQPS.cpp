#include "PKBQPS.h"

std::unordered_set<int> PKBQPS::getAllReads() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllReads();
}

std::unordered_set<int> PKBQPS::getAllPrints() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllPrints();
}

std::unordered_set<int> PKBQPS::getAllAssigns() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllAssigns();
}

std::unordered_set<int> PKBQPS::getAllWhiles() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllWhiles();
}

std::unordered_set<int> PKBQPS::getAllIfs() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllIfs();
}

std::unordered_set<int> PKBQPS::getAllStatements() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllStatements();
}

std::unordered_set<int> PKBQPS::getAllCalls() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllCalls();
}

std::unordered_set<std::string> PKBQPS::getAllProcedures() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllProcedures();
}

std::unordered_set<std::string> PKBQPS::getAllVariables() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllVariables();
}

std::unordered_set<std::string> PKBQPS::getAllConstants() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllConstants();
}

std::unordered_set<std::string> PKBQPS::getVariablesUsedInStatement(int lineNumber) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ");" << std::endl;
#endif
    return pkb->getVariablesUsedInStatement(lineNumber);
}

std::unordered_set<std::string> PKBQPS::getVariablesUsedInProcedure(const std::string& procedureName) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << procedureName << '"' << ");" << std::endl;
#endif
    return pkb->getVariablesUsedInProcedure(procedureName);
}

std::unordered_set<int> PKBQPS::getStatementsThatUseVariable(const std::string& variable) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << variable << '"' << ");" << std::endl;
#endif
    return pkb->getStatementsThatUseVariable(variable);
}

std::unordered_set<int> PKBQPS::getStatementsThatUseAnyVariable() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getStatementsThatUseAnyVariable();
}

std::unordered_set<std::string> PKBQPS::getProceduresThatUseVariable(const std::string& variable) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << variable << '"' << ");" << std::endl;
#endif
    return pkb->getProceduresThatUseVariable(variable);
}

std::unordered_set<std::string> PKBQPS::getProceduresThatUseAnyVariable() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getProceduresThatUseAnyVariable();
}

std::unordered_set<std::string> PKBQPS::getVariablesModifiedInStatement(int lineNumber) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ");" << std::endl;
#endif
    return pkb->getVariablesModifiedInStatement(lineNumber);
}

std::unordered_set<std::string> PKBQPS::getVariablesModifiedInProcedure(const std::string& procedureName) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << procedureName << '"' << ");" << std::endl;
#endif
    return pkb->getVariablesModifiedInProcedure(procedureName);
}

std::unordered_set<int> PKBQPS::getStatementsThatModifyVariable(const std::string& variable) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << variable << '"' << ");" << std::endl;
#endif
    return pkb->getStatementsThatModifyVariable(variable);
}

std::unordered_set<int> PKBQPS::getStatementsThatModifyAnyVariable() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getStatementsThatModifyAnyVariable();
}

std::unordered_set<std::string> PKBQPS::getProceduresThatModifyVariable(const std::string& variable) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << variable << '"' << ");" << std::endl;
#endif
    return pkb->getProceduresThatModifyVariable(variable);
}

std::unordered_set<std::string> PKBQPS::getProceduresThatModifyAnyVariable() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getProceduresThatModifyAnyVariable();
}

std::unordered_set<int> PKBQPS::getFollows(int lineNumber, bool onlyImmediate) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ", " << onlyImmediate << ");" << std::endl;
#endif
    return pkb->getFollows(lineNumber, onlyImmediate);
}

std::unordered_set<int> PKBQPS::getAllWithFollows() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllWithFollows();
}

std::unordered_set<int> PKBQPS::getAllWithBefore() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllWithBefore();
}

std::unordered_set<int> PKBQPS::getBefore(int lineNumber, bool onlyImmediate) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ", " << onlyImmediate << ");" << std::endl;
#endif
    return pkb->getBefore(lineNumber, onlyImmediate);
}

std::unordered_set<int> PKBQPS::getChild(int lineNumber, bool onlyImmediate) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ", " << onlyImmediate << ");" << std::endl;
#endif
    return pkb->getChild(lineNumber, onlyImmediate);
}

std::unordered_set<int> PKBQPS::getAllWithChild() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllWithChild();
}

std::unordered_set<int> PKBQPS::getParent(int lineNumber, bool onlyImmediate) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ", " << onlyImmediate << ");" << std::endl;
#endif
    return pkb->getParent(lineNumber, onlyImmediate);
}

std::unordered_set<int> PKBQPS::getAllWithParent() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllWithParent();
}

std::unordered_set<std::string> PKBQPS::getProceduresCalledBy(const std::string& procedure, bool onlyImmediate) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << procedure << '"' << ", " << onlyImmediate << ");" << std::endl;
#endif
    return pkb->getProceduresCalledBy(procedure, onlyImmediate);
}

std::unordered_set<std::string> PKBQPS::getProceduresCalling(const std::string& procedure, bool onlyImmediate) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << procedure << '"' << ", " << onlyImmediate << ");" << std::endl;
#endif
    return pkb->getProceduresCalling(procedure, onlyImmediate);
}

bool PKBQPS::doCallsExist() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->doCallsExist();
}

std::string PKBQPS::getProcedureCalledByCall(int lineNumber) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ");" << std::endl;
#endif
    return pkb->getProcedureCalledByCall(lineNumber);
}

std::unordered_set<std::string> PKBQPS::getVariablesUsedInIf(int lineNumber) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ");" << std::endl;
#endif
    return pkb->getVariablesUsedInIf(lineNumber);
}

std::unordered_set<int> PKBQPS::getIfsThatUseVariable(const std::string& variable) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << variable << ");" << std::endl;
#endif
    return pkb->getIfsThatUseVariable(variable);
}

std::unordered_set<int> PKBQPS::getIfsThatUseAnyVariable() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getIfsThatUseAnyVariable();
}

std::unordered_set<std::string> PKBQPS::getVariablesUsedInWhile(int lineNumber) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ");" << std::endl;
#endif
    return pkb->getVariablesUsedInWhile(lineNumber);
}

std::unordered_set<int> PKBQPS::getWhilesThatUseVariable(const std::string& variable) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << '"' << variable << '"' << ");" << std::endl;
#endif
    return pkb->getWhilesThatUseVariable(variable);
}

std::unordered_set<int> PKBQPS::getWhilesThatUseAnyVariable() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getWhilesThatUseAnyVariable();
}

std::unordered_set<std::string> PKBQPS::getProceduresThatCall() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getProceduresThatCall();
}

std::unordered_set<std::string> PKBQPS::getAllCalledProcedures() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllCalledProcedures();
}

std::unordered_set<int> PKBQPS::getNext(int lineNumber) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ");" << std::endl;
#endif
    return pkb->getNext(lineNumber);
}

std::unordered_set<int> PKBQPS::getAllWithNext() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllWithNext();
}

std::unordered_set<int> PKBQPS::getAllWithPrevious() {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << "();" << std::endl;
#endif
    return pkb->getAllWithPrevious();
}

std::unordered_set<int> PKBQPS::getPrevious(int lineNumber) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ");" << std::endl;
#endif
    return pkb->getPrevious(lineNumber);
}

bool PKBQPS::hasMatchingPattern(int lineNumber, const std::vector<std::string>& postfixExpression,
                                bool exactMatch) {
#ifdef QPS_LOGGING_ENABLED
    std::cerr << "pkbQPS->" << __func__ << '(' << lineNumber << ", {";
    for (int i{0}; i < postfixExpression.size(); ++i) {
        std::cerr << '"' << postfixExpression[i];
        if (i != postfixExpression.size() - 1) {
            std::cerr << ", ";
        }
    }
    std::cerr << '}' << ", " << exactMatch << ");" << std::endl;
#endif
    return pkb->hasMatchingPattern(lineNumber, postfixExpression, exactMatch);
}
