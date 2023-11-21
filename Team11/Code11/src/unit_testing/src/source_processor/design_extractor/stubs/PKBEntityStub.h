#pragma once

#include <string>
#include <memory>
#include <unordered_set>
#include "source_processor/storage_writer/writer_interface/IEntityWriter.h"
#include "source_processor/ast/statements/AssignNode.h"
#include "source_processor/ast/statements/CallNode.h"
#include "source_processor/ast/statements/IfNode.h"
#include "source_processor/ast/statements/PrintNode.h"
#include "source_processor/ast/statements/ReadNode.h"
#include "source_processor/ast/statements/WhileNode.h"

class PKBEntityStub : public IEntityWriter {
public:
	std::unordered_set<std::string> procedures;
	std::unordered_set<int> statements;
	std::unordered_set<int> readStatements;
	std::unordered_set<int> printStatements;
	std::unordered_set<int> assignStatements;
	std::unordered_set<int> ifStatements;
	std::unordered_set<int> whileStatements;
	std::unordered_set<int> callStatements;
	std::unordered_set<std::string> variables;
	std::unordered_set<std::string> constants;
	std::unordered_set<std::string> readVariables;
	std::unordered_set<std::string> printVariables;
	std::unordered_set<std::string> assignVariables;
	std::unordered_set<std::shared_ptr<AssignNode>> patterns;

	void insertProcedure(std::string procedure) override;
	void insertVariable(std::string varName) override;
	void insertConstant(std::string value) override;

	void insertRead(std::shared_ptr<ReadNode> node) override;
	void insertPrint(std::shared_ptr<PrintNode> node) override;
	void insertAssign(std::shared_ptr<AssignNode> node) override;
	void insertCall(std::shared_ptr<CallNode> node) override;
	void insertIf(std::shared_ptr<IfNode> node) override;
	void insertWhile(std::shared_ptr<WhileNode> node) override;
	void insertPattern(std::shared_ptr<AssignNode> node) override;

	bool containsProcedure(std::string procedureName) override;
};
