#pragma once

#include <memory>
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"
#include "program_knowledge_base/facades/PKBQPS.h"

class QPSStorageReader : public IStorageReader {
private:
    std::shared_ptr<PKBQPS> pkb;

public:
    explicit QPSStorageReader(const std::shared_ptr<PKBQPS>& pkb);

    std::unordered_set<int> getAllStatements() override;
    std::unordered_set<int> getAllReads() override;
    std::unordered_set<int> getAllPrints() override;
    std::unordered_set<int> getAllCalls() override;
    std::unordered_set<int> getAllWhiles() override;
    std::unordered_set<int> getAllIfs() override;
    std::unordered_set<int> getAllAssigns() override;

    std::unordered_set<std::string> getAllVariables() override;
    std::unordered_set<std::string> getAllConstants() override;
    std::unordered_set<std::string> getAllProcedures() override;

    std::unordered_set<std::string> getVariablesUsedInStatement(int statementNumber) override;
    std::unordered_set<int> getStatementsThatUseVariable(std::string variableName) override;
    std::unordered_set<int> getStatementsThatUseAnyVariable() override;

    std::unordered_set<std::string> getVariablesUsedInProcedure(std::string procedureName) override;
    std::unordered_set<std::string> getProceduresThatUseVariable(std::string variableName) override;
    std::unordered_set<std::string> getProceduresThatUseAnyVariable() override;

    std::unordered_set<std::string> getVariablesModifiedInStatement(int statementNumber) override;
    std::unordered_set<int> getStatementsThatModifyVariable(std::string variableName) override;
    std::unordered_set<int> getStatementsThatModifyAnyVariable() override;

    std::unordered_set<std::string> getVariablesModifiedInProcedure(std::string procedureName) override;
    std::unordered_set<std::string> getProceduresThatModifyVariable(std::string variableName) override;
    std::unordered_set<std::string> getProceduresThatModifyAnyVariable() override;

    std::unordered_set<int> getFollows(int statementNumber, bool onlyImmediate) override;
    std::unordered_set<int> getBefore(int statementNumber, bool onlyImmediate) override;
    std::unordered_set<int> getAllWithFollows() override;
    std::unordered_set<int> getAllWithBefore() override;

    std::unordered_set<int> getParent(int statementNumber, bool onlyImmediate) override;
    std::unordered_set<int> getChild(int statementNumber, bool onlyImmediate) override;
    std::unordered_set<int> getAllWithParent() override;
    std::unordered_set<int> getAllWithChild() override;

    std::unordered_set<std::string> getProceduresCalledBy(std::string procedureName, bool onlyImmediate) override;
    std::unordered_set<std::string> getProceduresCalling(std::string procedureName, bool onlyImmediate) override;
    std::unordered_set<std::string> getProceduresThatCall() override;
    std::unordered_set<std::string> getAllCalledProcedures() override;

    bool hasMatchingPattern(int statementNumber, std::vector<std::string> postfixExpression, bool exactMatch) override;
    std::unordered_set<std::string> getVariablesUsedInIf(int lineNumber) override;
    std::unordered_set<int> getIfsThatUseVariable(std::string variable) override;
    std::unordered_set<int> getIfsThatUseAnyVariable() override;

    std::unordered_set<std::string> getVariablesUsedInWhile(int lineNumber) override;
    std::unordered_set<int> getWhilesThatUseVariable(std::string variable) override;
    std::unordered_set<int> getWhilesThatUseAnyVariable() override;

    std::unordered_set<int> getNext(int statementNumber) override;
    std::unordered_set<int> getPrevious(int statementNumber) override;

    std::string getProcedureCalledByCall(int statementNumber) override;
};
