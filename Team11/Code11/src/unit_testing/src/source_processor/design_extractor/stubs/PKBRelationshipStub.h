#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "source_processor/storage_writer/writer_interface/IRelationshipWriter.h"

class PKBRelationshipStub : public IRelationshipWriter {
public:
	std::unordered_map<int, std::unordered_set<std::string>> modifiesS;
	std::unordered_map<std::string, std::unordered_set<std::string>> modifiesP;
	std::unordered_map<int, std::unordered_set<std::string>> usesS;
	std::unordered_map<std::string, std::unordered_set<std::string>> usesP;
	std::unordered_map<int, std::unordered_set<int>> follows;
	std::unordered_map<int, std::unordered_set<int>> parents;
	std::unordered_map<int, std::unordered_set<int>> followsT;
	std::unordered_map<int, std::unordered_set<int>> parentsT;
	std::unordered_map<int, std::unordered_set<int>> next;
	std::unordered_map<std::string, std::unordered_set<std::string>> calls;
	std::unordered_map<std::string, std::unordered_set<std::string>> callsT;
	std::unordered_set<std::string> procedures;
	
	void insertModifiesS(int stmtNo, const std::string& varName) override;
	void insertModifiesP(const std::string& procName, const std::string& varName) override;
	void insertUsesS(int stmtNo, const std::string& varName) override;
	void insertUsesP(const std::string& procName, const std::string& varName) override;
	void insertFollows(int prev, int next) override;
	void insertFollowsT(int prev, int next) override;
	void insertParents(int parent, int child) override;
	void insertParentsT(int parent, int child) override;
	void insertCalls(const std::string& caller, const std::string& callee) override;
	void insertCallsT(const std::string& caller, const std::string& callee) override;
	void insertNext(int prev, int next) override;

	bool containsProcedure(const std::string& procName) override;
	std::unordered_map<std::string, std::unordered_set<std::string>> getCalls() override;
	std::unordered_map<std::string, std::unordered_set<std::string>> getCallReversedMap() override;
	std::unordered_map<std::string, std::unordered_set<std::string>> getCallsT() override;
	std::unordered_map<std::string, std::unordered_set<std::string>> getUsesP() override;
	std::unordered_map<std::string, std::unordered_set<std::string>> getModifiesP() override;
};
