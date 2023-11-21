#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>
#include <memory>
#include "source_processor/exceptions/DesignExtractorException.h"
#include "source_processor/exceptions/ExceptionMessage.h"
#include "source_processor/storage_writer/writer_interface/IRelationshipWriter.h"

class ProcedureManager {
public:
// stores the number of times a procedure is called (other procedures call it)
    static std::unordered_map<std::string, int> procedureCallCount;
    static std::unordered_map<std::string, std::unordered_set<int> > statementsInProcAffectedByCall;

    static void initializeProcedureManager();
    static void incrementCallCount(const std::string& procName, const std::string& currProc,
                                   std::unordered_map<std::string,
                                                      std::unordered_set<std::string> > calls);
    static void addStatementAffected(const std::string& procName, int stmtNum);
    static std::vector<std::string> getTopoSortedDAG(std::unordered_map<std::string,
                                                                        std::unordered_set<std::string> > reversedCalls,
                                                     const std::shared_ptr<IRelationshipWriter>& pkb);
};
