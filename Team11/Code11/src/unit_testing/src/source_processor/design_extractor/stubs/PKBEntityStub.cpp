#include "PKBEntityStub.h"

void PKBEntityStub::insertProcedure(std::string procedure) {
	procedures.insert(procedure);
}

void PKBEntityStub::insertVariable(std::string variable) {
	variables.insert(variable);
}

void PKBEntityStub::insertConstant(std::string constant) {
	constants.insert(constant);
}

void PKBEntityStub::insertRead(std::shared_ptr<ReadNode> node) {
	readStatements.insert(node->getLineNum());
	statements.insert(node->getLineNum());
	readVariables.insert(node->getVar()->getVarName());
}

void PKBEntityStub::insertPrint(std::shared_ptr<PrintNode> node) {
	printStatements.insert(node->getLineNum());
	statements.insert(node->getLineNum());
	printVariables.insert(node->getVar()->getVarName());
}

void PKBEntityStub::insertAssign(std::shared_ptr<AssignNode> node) {
	assignStatements.insert(node->getLineNum());
	statements.insert(node->getLineNum());
	assignVariables.insert(node->getLeft()->getVarName());
}

void PKBEntityStub::insertCall(std::shared_ptr<CallNode> node) {
	callStatements.insert(node->getLineNum());
}

void PKBEntityStub::insertIf(std::shared_ptr<IfNode> node) {
	ifStatements.insert(node->getLineNum());
	statements.insert(node->getLineNum());
}

void PKBEntityStub::insertWhile(std::shared_ptr<WhileNode> node) {
	whileStatements.insert(node->getLineNum());
	statements.insert(node->getLineNum());
}

void PKBEntityStub::insertPattern(std::shared_ptr<AssignNode> node) {
	// bypass problem of hashing vector of strings without custom hash function
	patterns.insert(node);
}

bool PKBEntityStub::containsProcedure(std::string procedureName) {
    return procedures.find(procedureName) != procedures.end();
}
