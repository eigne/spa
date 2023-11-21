#pragma once

#include <memory>
#include "source_processor/storage_writer/writer_interface/IRelationshipWriter.h"
#include "program_knowledge_base/facades/PKBSP.h"

class RelationshipWriter : public IRelationshipWriter {
private:
    std::shared_ptr<PKBSP> pkb;
public:
    explicit RelationshipWriter(std::shared_ptr<PKBSP>& writer);

    void insertUsesS(int stmtNo, const std::string& variableName) override;
    void insertUsesP(const std::string& procName, const std::string& variableName) override;
    void insertModifiesS(int stmtNo, const std::string& variableName) override;
    void insertModifiesP(const std::string& procName, const std::string& variableName) override;
    void insertFollows(int prev, int next) override;
    void insertFollowsT(int prev, int next) override;
    void insertParents(int parent, int child) override;
    void insertParentsT(int parent, int child) override;
    void insertCalls(const std::string& caller, const std::string& callee) override;
    void insertCallsT(const std::string& caller, const std::string& callee) override;
    void insertNext(int prev, int next) override;

    bool containsProcedure(const std::string& procName) override;
    std::unordered_map<std::string, std::unordered_set<std::string> > getCalls() override;
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallReversedMap() override;
    std::unordered_map<std::string, std::unordered_set<std::string> > getCallsT() override;
    std::unordered_map<std::string, std::unordered_set<std::string> > getUsesP() override;
    std::unordered_map<std::string, std::unordered_set<std::string> > getModifiesP() override;
};
