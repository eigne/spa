#include "RelationshipExtractor.h"

#include <utility>
#include "source_processor/design_extractor/relationship_extraction_strategies/PrintStmtStrategy.h"
#include "source_processor/design_extractor/relationship_extraction_strategies/ReadStmtStrategy.h"
#include "source_processor/design_extractor/relationship_extraction_strategies/AssignStmtStrategy.h"
#include "source_processor/design_extractor/relationship_extraction_strategies/IfStmtStrategy.h"
#include "source_processor/design_extractor/relationship_extraction_strategies/WhileStmtStrategy.h"
#include "source_processor/design_extractor/relationship_extraction_strategies/CallStmtStrategy.h"
#include "source_processor/design_extractor/relationship_managers/FollowsManager.h"
#include "source_processor/design_extractor/relationship_managers/ParentManager.h"
#include "source_processor/design_extractor/relationship_managers/ProcedureManager.h"
#include "source_processor/design_extractor/relationship_managers/FlowManager.h"

RelationshipExtractor::RelationshipExtractor(std::shared_ptr<IRelationshipWriter> writer) {
    relationshipWriter = std::move(writer);
}

void RelationshipExtractor::extract(std::shared_ptr<ProgramNode>& root) {
    ProcedureManager::initializeProcedureManager();
    BaseExtractor::extract(root);
    interlinkRelationships();
}

void RelationshipExtractor::interlinkRelationships() {
    auto topoSortedDAG =
        ProcedureManager::getTopoSortedDAG(relationshipWriter->getCallReversedMap(), relationshipWriter);

    // If after topo sorting we do not have all the procedures, then there must be a cyclic call
    if (topoSortedDAG.size() != ProcedureManager::procedureCallCount.size())
        throw DesignExtractorException(CYCLIC_CALL_ERROR);

    for (auto& procedure : topoSortedDAG) {
        interlinkStatementRelationships(procedure);
        interlinkProcedureRelationships(procedure);
    }
}

void RelationshipExtractor::interlinkStatementRelationships(std::string& procedureName) {
    auto usesPKnowledgeBase = relationshipWriter->getUsesP();
    auto modifiesPKnowledgeBase = relationshipWriter->getModifiesP();

    for (auto& varName : usesPKnowledgeBase[procedureName]) {
        for (auto& stmtNum : ProcedureManager::statementsInProcAffectedByCall[procedureName]) {
            relationshipWriter->insertUsesS(stmtNum, varName);
        }
    }

    for (auto& varName : modifiesPKnowledgeBase[procedureName]) {
        for (auto& stmtNum : ProcedureManager::statementsInProcAffectedByCall[procedureName]) {
            relationshipWriter->insertModifiesS(stmtNum, varName);
        }
    }
}

void RelationshipExtractor::interlinkProcedureRelationships(std::string& procedureName) {
    auto callReversedMap = relationshipWriter->getCallReversedMap();
    auto usesPKnowledgeBase = relationshipWriter->getUsesP();
    auto modifiesPKnowledgeBase = relationshipWriter->getModifiesP();
    auto callsT = relationshipWriter->getCallsT();

    for (auto& caller : callReversedMap[procedureName]) {
        relationshipWriter->insertCallsT(caller, procedureName);

        for (auto& callee : callsT[procedureName]) {
            relationshipWriter->insertCallsT(caller, callee);
        }

        for (auto& varName : usesPKnowledgeBase[procedureName]) {
            relationshipWriter->insertUsesP(caller, varName);
        }

        for (auto& varName : modifiesPKnowledgeBase[procedureName]) {
            relationshipWriter->insertModifiesP(caller, varName);
        }
    }
}

void RelationshipExtractor::extractProcedure(std::shared_ptr<ProcedureNode>& node) {
    currentProcedureName = node->getName();
    FollowsManager::clearFollowsStack();
    ParentManager::clearParentStack();
    FlowManager::initializeFlowManager();
    if (ProcedureManager::procedureCallCount.find(currentProcedureName) == ProcedureManager::procedureCallCount.end())
        ProcedureManager::procedureCallCount[currentProcedureName] = 0;
    BaseExtractor::extractProcedure(node);
}

void RelationshipExtractor::extractStatementList(std::vector<std::shared_ptr<StatementNode> > stmtList) {
    FollowsManager::addNewNestingLevel();
    BaseExtractor::extractStatementList(stmtList);
    FollowsManager::removeLastNestingLevel();
}

void RelationshipExtractor::extractStatement(std::shared_ptr<StatementNode>& node) {
    FollowsManager::addFollows(node->getLineNum(), relationshipWriter);
    ParentManager::addChild(node->getLineNum(), relationshipWriter);

    FlowManager::addFlow(node->getLineNum(), relationshipWriter);
    FlowManager::resetFlow(node->getLineNum());
    node->accept(*this);
}

void RelationshipExtractor::visit(std::shared_ptr<IfNode>& ifNode) {
    context.setStrategy(std::make_unique<IfStmtStrategy>());
    context.executeStrategy(ifNode, relationshipWriter, currentProcedureName);
    extractStatementList(*ifNode->getThenStmtLst());
    int thenLastStmt = FlowManager::prevStatements.back();
    FlowManager::resetFlow(ifNode->getLineNum());
    extractStatementList(*ifNode->getElseStmtLst());
    int elseLastStmt = FlowManager::prevStatements.back();
    FlowManager::prevStatements.push_back(thenLastStmt);
    FlowManager::prevStatements.push_back(elseLastStmt);
    ParentManager::removeLastParent();
}

void RelationshipExtractor::visit(std::shared_ptr<WhileNode>& whileNode) {
    context.setStrategy(std::make_unique<WhileStmtStrategy>());
    context.executeStrategy(whileNode, relationshipWriter, currentProcedureName);
    extractStatementList(*whileNode->getStmtLst());
    FlowManager::addFlow(whileNode->getLineNum(), relationshipWriter);
    FlowManager::resetFlow(whileNode->getLineNum());
    ParentManager::removeLastParent();
}

void RelationshipExtractor::visit(std::shared_ptr<CallNode>& callNode) {
    context.setStrategy(std::make_unique<CallStmtStrategy>());
    context.executeStrategy(callNode, relationshipWriter, currentProcedureName);
}

void RelationshipExtractor::visit(std::shared_ptr<AssignNode>& assignNode) {
    context.setStrategy(std::make_unique<AssignStmtStrategy>());
    context.executeStrategy(assignNode, relationshipWriter, currentProcedureName);
}

void RelationshipExtractor::visit(std::shared_ptr<PrintNode>& printNode) {
    context.setStrategy(std::make_unique<PrintStmtStrategy>());
    context.executeStrategy(printNode, relationshipWriter, currentProcedureName);
}

void RelationshipExtractor::visit(std::shared_ptr<ReadNode>& readNode) {
    context.setStrategy(std::make_unique<ReadStmtStrategy>());
    context.executeStrategy(readNode, relationshipWriter, currentProcedureName);
}
