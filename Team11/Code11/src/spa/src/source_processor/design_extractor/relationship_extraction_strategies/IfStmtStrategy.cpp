#include "IfStmtStrategy.h"
#include "source_processor/ast/statements/IfNode.h"
#include "source_processor/design_extractor/relationship_managers/ParentManager.h"

void IfStmtStrategy::execute(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                             std::string currentProcedureName) {
    auto ifNode = std::dynamic_pointer_cast<IfNode>(node);
    auto variables = ifNode->getCond()->getVariables();
    for (auto& v : variables) {
        insertUsesRelationship(pkb, v, ifNode->getLineNum(), currentProcedureName);
    }

    ParentManager::addStatementToStack(ifNode->getLineNum());
}
