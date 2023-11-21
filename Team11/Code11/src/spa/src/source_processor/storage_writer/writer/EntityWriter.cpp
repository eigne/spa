#include "EntityWriter.h"

EntityWriter::EntityWriter(std::shared_ptr<PKBSP>& writer) {
    this->kbWriter = writer;
}

void EntityWriter::insertProcedure(std::string procedureName) {
    kbWriter->insertProcedure(procedureName);
}

void EntityWriter::insertVariable(std::string varName) {
    kbWriter->insertVariable(varName);
}

void EntityWriter::insertConstant(std::string value) {
    kbWriter->insertConstant(value);
}

void EntityWriter::insertRead(std::shared_ptr<ReadNode> node) {
    std::string varName = node->getVar()->getVarName();
    kbWriter->insertRead(node->getLineNum(), varName);
}

void EntityWriter::insertPrint(std::shared_ptr<PrintNode> node) {
    std::string varName = node->getVar()->getVarName();
    kbWriter->insertPrint(node->getLineNum(), varName);
}

void EntityWriter::insertAssign(std::shared_ptr<AssignNode> node) {
    std::string varName = node->getLeft()->getVarName();
    kbWriter->insertAssign(node->getLineNum(), varName);
}

void EntityWriter::insertCall(std::shared_ptr<CallNode> node) {
    std::string procName = node->getProc();
    kbWriter->insertCall(node->getLineNum(), procName);
}

void EntityWriter::insertIf(std::shared_ptr<IfNode> node) {
    kbWriter->insertIf(node->getLineNum(), node->getCond()->getVariables());
}

void EntityWriter::insertWhile(std::shared_ptr<WhileNode> node) {
    kbWriter->insertWhile(node->getLineNum(), node->getCond()->getVariables());
}

void EntityWriter::insertPattern(std::shared_ptr<AssignNode> node) {
    kbWriter->insertPattern(node->getLineNum(), node->getRight()->getSortedOp());
}

// Read from PKB
bool EntityWriter::containsProcedure(std::string procedureName) {
    return kbWriter->containsProcedure(procedureName);
}
