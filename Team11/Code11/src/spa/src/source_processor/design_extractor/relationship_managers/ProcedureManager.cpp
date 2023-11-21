#include "ProcedureManager.h"

std::unordered_map<std::string, int> ProcedureManager::procedureCallCount;
std::unordered_map<std::string, std::unordered_set<int> > ProcedureManager::statementsInProcAffectedByCall;

void ProcedureManager::initializeProcedureManager() {
    ProcedureManager::procedureCallCount.clear();
    ProcedureManager::statementsInProcAffectedByCall.clear();
}

void ProcedureManager::incrementCallCount(const std::string& procName, const std::string& currProc,
                                          std::unordered_map<std::string, std::unordered_set<std::string> > calls) {
    // only add if procedure called is unique
    // e.g. if procedure A calls procedure B twice, only increment once
    if (!calls[currProc].count(procName))
        ProcedureManager::procedureCallCount[currProc]++;
}

void ProcedureManager::addStatementAffected(const std::string& procName, int stmtNum) {
    if (ProcedureManager::statementsInProcAffectedByCall.find(procName) ==
        ProcedureManager::statementsInProcAffectedByCall.end())
        ProcedureManager::statementsInProcAffectedByCall[procName] = std::unordered_set<int>();

    ProcedureManager::statementsInProcAffectedByCall[procName].insert(stmtNum);
}

std::vector<std::string> ProcedureManager::getTopoSortedDAG(std::unordered_map<std::string,
                                                                               std::unordered_set<std::string> > reversedCalls,
                                                            const std::shared_ptr<IRelationshipWriter>& pkb)  {
    std::queue<std::string> procedureQueue;
    std::vector<std::string> procedureDAG;

    for (auto& procedure : ProcedureManager::procedureCallCount) {
        if (procedure.second == 0)
            procedureQueue.push(procedure.first);
    }

    // If there are no procedures with 0 outgoing calls, then there is defintely a cycle
    if (procedureQueue.empty())
        throw DesignExtractorException(CYCLIC_CALL_ERROR);

    while (!procedureQueue.empty()) {
        std::string procedureName = procedureQueue.front();
        procedureQueue.pop();
        procedureDAG.push_back(procedureName);

        // process procedures that don't have any outgoing calls first so that all their relationship are captured by their callers
        for (auto& procedure : reversedCalls[procedureName]) {
            // Non-existent procedure check
            if (!pkb->containsProcedure(procedure))
                throw DesignExtractorException(NON_EXISTENT_PROCEDURE_ERROR);

            ProcedureManager::procedureCallCount[procedure]--;
            if (ProcedureManager::procedureCallCount[procedure] == 0)
                procedureQueue.push(procedure);
        }
    }

    return procedureDAG;
}
