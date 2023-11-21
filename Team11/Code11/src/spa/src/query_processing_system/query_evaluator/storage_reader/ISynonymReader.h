#pragma once

#include <unordered_set>
#include <string>
#include "query_processing_system/query_evaluator/storage_reader/IUsesStatementReader.h"
#include "query_processing_system/query_evaluator/storage_reader/IModifiesStatementReader.h"

class ISynonymReader : public virtual IUsesStatementReader, public virtual IModifiesStatementReader {
public:
    virtual std::unordered_set<int> getAllStatements() = 0;
    virtual std::unordered_set<int> getAllReads() = 0;
    virtual std::unordered_set<int> getAllPrints() = 0;
    virtual std::unordered_set<int> getAllCalls() = 0;
    virtual std::unordered_set<int> getAllWhiles() = 0;
    virtual std::unordered_set<int> getAllIfs() = 0;
    virtual std::unordered_set<int> getAllAssigns() = 0;

    virtual std::unordered_set<std::string> getAllVariables() = 0;
    virtual std::unordered_set<std::string> getAllConstants() = 0;
    virtual std::unordered_set<std::string> getAllProcedures() = 0;

    virtual std::string getProcedureCalledByCall(int statementNumber) = 0;
};
