#include "EntityExtractor.h"

#include <utility>

EntityExtractor::EntityExtractor(std::shared_ptr<IEntityWriter> writer) {
    entityWriter = std::move(writer);
}

void EntityExtractor::extract(std::shared_ptr<ProgramNode>& node) {
    BaseExtractor::extract(node);
}

void EntityExtractor::extractProcedure(std::shared_ptr<ProcedureNode>& node) {
    // check for duplicated procedure name semantic error
    if (entityWriter->containsProcedure(node->getName())) {
        throw DesignExtractorException(DUPLICATED_PROCEDURE_ERROR);
    }

    entityWriter->insertProcedure(node->getName());
    BaseExtractor::extractProcedure(node);
}

void EntityExtractor::extractStatement(std::shared_ptr<StatementNode>& node) {
    node->accept(*this);
}

void EntityExtractor::extractConstantsFromConditionNode(const std::shared_ptr<ConditionNode>& conditionNode) {
    for (const auto& constants : conditionNode->getConstants()) {
        entityWriter->insertConstant(constants);
    }
}

void EntityExtractor::extractVariablesFromConditionNode(const std::shared_ptr<ConditionNode>& conditionNode) {
    for (const auto& variables : conditionNode->getVariables()) {
        entityWriter->insertVariable(variables);
    }
}

void EntityExtractor::visit(std::shared_ptr<IfNode>& ifNode) {
    entityWriter->insertIf(ifNode);
    extractConstantsFromConditionNode(ifNode->getCond());
    extractVariablesFromConditionNode(ifNode->getCond());

    extractStatementList(*ifNode->getThenStmtLst());
    extractStatementList(*ifNode->getElseStmtLst());
}

void EntityExtractor::visit(std::shared_ptr<WhileNode>& whileNode) {
    entityWriter->insertWhile(whileNode);
    extractConstantsFromConditionNode(whileNode->getCond());
    extractVariablesFromConditionNode(whileNode->getCond());

    extractStatementList(*whileNode->getStmtLst());
}

void EntityExtractor::visit(std::shared_ptr<CallNode>& callNode) {
    entityWriter->insertCall(callNode);
}

void EntityExtractor::visit(std::shared_ptr<AssignNode>& assignNode) {
    entityWriter->insertAssign(assignNode);
    entityWriter->insertVariable(assignNode->getLeft()->getVarName());
    entityWriter->insertPattern(assignNode);

    for (const auto& constants : assignNode->getConstants()) {
        entityWriter->insertConstant(constants);
    }

    for (const auto& variables : assignNode->getRightVariables()) {
        entityWriter->insertVariable(variables);
    }
}

void EntityExtractor::visit(std::shared_ptr<PrintNode>& printNode) {
    entityWriter->insertPrint(printNode);
    entityWriter->insertVariable(printNode->getVar()->getVarName());
}

void EntityExtractor::visit(std::shared_ptr<ReadNode>& readNode) {
    entityWriter->insertRead(readNode);
    entityWriter->insertVariable(readNode->getVar()->getVarName());
}
