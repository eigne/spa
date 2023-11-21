#include "PKBSP.h"

void PKBSP::insertRead(int lineNumber, const std::string& entityRead) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '(' << lineNumber << ", " << '"' << entityRead << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertRead(lineNumber, entityRead);
}

void PKBSP::insertPrint(int lineNumber, const std::string& entityPrinted) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__  << '(' << lineNumber << ", " << '"' << entityPrinted << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertPrint(lineNumber, entityPrinted);
}

void PKBSP::insertAssign(int lineNumber, const std::string& entityAssigned) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '(' << lineNumber << ", " << '"' << entityAssigned << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertAssign(lineNumber, entityAssigned);
}

void PKBSP::insertWhile(int lineNumber, const std::vector<std::string>& entitiesInWhile) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__  << "(" << lineNumber << ", {";
    for (int i{0}; i < entitiesInWhile.size(); ++i) {
        std::cerr << '"' << entitiesInWhile[i] << '"';
        if (i != entitiesInWhile.size() - 1) {
            std::cerr << ", ";
        }
    }
    std::cerr << '}' << ')' << ';' << std::endl;
#endif
    pkb->insertWhile(lineNumber, entitiesInWhile);
}

void PKBSP::insertIf(int lineNumber, const std::vector<std::string>& entitiesInIf) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__  << "(" << lineNumber << ", {";
    for (int i{0}; i < entitiesInIf.size(); ++i) {
        std::cerr << '"' << entitiesInIf[i] << '"';
        if (i != entitiesInIf.size() - 1) {
            std::cerr << ", ";
        }
    }
    std::cerr << '}' << ')' << ';' << std::endl;
#endif
    pkb->insertIf(lineNumber, entitiesInIf);
}

void PKBSP::insertProcedure(const std::string& procedureName) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << '"' << procedureName << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertProcedure(procedureName);
}

void PKBSP::insertVariable(const std::string& variable) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << '"' << variable << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertVariable(variable);
}

void PKBSP::insertCall(int lineNumber, const std::string& procedureCalled) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << lineNumber << ", " << '"' << procedureCalled << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertCall(lineNumber, procedureCalled);
}

void PKBSP::insertCalls(const std::string& caller, const std::string& procedureCalled) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << '"' << caller << '"' << ", " << '"' << procedureCalled << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertCalls(caller, procedureCalled);
}

void PKBSP::insertCallsStar(const std::string& caller, const std::string& procedureCalled) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << '"' << caller << '"' << ", " << '"' << procedureCalled << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertCallsStar(caller, procedureCalled);
}

void PKBSP::insertConstant(const std::string& constant) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << '"' << constant << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertConstant(constant);
}

void PKBSP::insertUses(int statementNumber, const std::string& variable) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << statementNumber << ", " << '"' << variable << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertUses(statementNumber, variable);
}

void PKBSP::insertUses(const std::string& procedureName, const std::string& variable) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << '"' << procedureName << '"' << ", " << '"' << variable << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertUses(procedureName, variable);
}

void PKBSP::insertModifies(int statementNumber, const std::string& variable) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << statementNumber << ", " << '"' << variable << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertModifies(statementNumber, variable);
}

void PKBSP::insertModifies(const std::string& procedureName, const std::string& variable) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << '"' << procedureName << '"' << ", " << '"' << variable << '"' << ')' << ';' << std::endl;
#endif
    pkb->insertModifies(procedureName, variable);
}

void PKBSP::insertFollows(int before, int after) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << before << ", " << after << ')' << ';' << std::endl;
#endif
    pkb->insertFollows(before, after);
}

void PKBSP::insertFollowsStar(int before, int after) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << before << ", " << after << ')' << ';' << std::endl;
#endif
    pkb->insertFollowsStar(before, after);
}

void PKBSP::insertParents(int parent, int child) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << parent << ", " << child << ')' << ';' << std::endl;
#endif
    pkb->insertParents(parent, child);
}

void PKBSP::insertParentsStar(int parent, int child) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << parent << ", " << child << ')' << ';' << std::endl;
#endif
    pkb->insertParentsStar(parent, child);
}

void PKBSP::insertNext(int previous, int next) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << previous << ", " << next << ')' << ';' << std::endl;
#endif
    pkb->insertNext(previous, next);
}

void PKBSP::insertPattern(int statementNumber, const std::vector<std::string>& postfixExpression) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << statementNumber << ", " << '{';
    for (int i{0}; i < postfixExpression.size(); ++i) {
        std::cerr << '"' << postfixExpression[i] << '"';
        if (i != postfixExpression.size() - 1) {
            std::cerr << ", ";
        }
    }
    std::cerr << '}';
    std::cerr << ')' << ';' << std::endl;
#endif
    pkb->insertPattern(statementNumber, postfixExpression);
}

bool PKBSP::containsProcedure(const std::string& procedureName) {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << '"' << procedureName << '"' << ')' << ';' << std::endl;
#endif
    return pkb->containsProcedure(procedureName);
}

std::unordered_map<std::string, std::unordered_set<std::string> > PKBSP::getCallsMap() {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << ')' << ';' << std::endl;
#endif
    return pkb->getCallsMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > PKBSP::getCallsStarMap() {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << ')' << ';' << std::endl;
#endif
    return pkb->getCallsStarMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > PKBSP::getCallsReversedMap() {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << ')' << ';' << std::endl;
#endif
    return pkb->getCallsReversedMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > PKBSP::getCallsStarReversedMap() {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << ')' << ';' << std::endl;
#endif
    return pkb->getCallsStarReversedMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > PKBSP::getUsesProceduresMap() {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << ')' << ';' << std::endl;
#endif
    return pkb->getUsesProcedureMap();
}

std::unordered_map<std::string, std::unordered_set<std::string> > PKBSP::getModifiesProceduresMap() {
#ifdef SP_LOGGING_ENABLED
    std::cerr << "pkbSP->" << __func__ << '('  << ')' << ';' << std::endl;
#endif
    return pkb->getModifiesProcedureMap();
}
