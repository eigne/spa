#include "CallStmtStrategy.h"
#include "source_processor/ast/statements/CallNode.h"
#include "source_processor/design_extractor/relationship_managers/ProcedureManager.h"
#include "source_processor/design_extractor/relationship_managers/ParentManager.h"

void CallStmtStrategy::execute(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                               std::string currentProcedureName) {
    auto callNode = std::dynamic_pointer_cast<CallNode>(node);

    // Self-call check
    if (currentProcedureName == callNode->getProc()) {
        throw DesignExtractorException(RECURSIVE_CALL_ERROR);
    }

    if (ProcedureManager::procedureCallCount.find(currentProcedureName) == ProcedureManager::procedureCallCount.end()) {
        ProcedureManager::procedureCallCount[currentProcedureName] = 0;
    }

    // Increment call count of caller
    ProcedureManager::incrementCallCount(callNode->getProc(), currentProcedureName, pkb->getCalls());

    // Keep track of calling statements as well as parents of calling statements
    // Parents need to be tracked as well since a call statement can be nested within a while/if statement
    // and the parent will also modifies/use a variable that is modified/used by the procedure in the call statement
    ProcedureManager::addStatementAffected(callNode->getProc(), callNode->getLineNum());
    for (auto parent : ParentManager::getParentStack()) {
        ProcedureManager::addStatementAffected(callNode->getProc(), parent);
    }

    pkb->insertCalls(currentProcedureName, callNode->getProc());
}
