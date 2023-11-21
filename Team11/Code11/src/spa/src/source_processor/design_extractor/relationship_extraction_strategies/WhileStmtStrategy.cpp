#include "WhileStmtStrategy.h"
#include "source_processor/ast/statements/WhileNode.h"
#include "source_processor/design_extractor/relationship_managers/ParentManager.h"

void WhileStmtStrategy::execute(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                                std::string currentProcedureName) {
    auto whileNode = std::dynamic_pointer_cast<WhileNode>(node);
    auto variables = whileNode->getCond()->getVariables();
    for (auto& v : variables) {
        insertUsesRelationship(pkb, v, whileNode->getLineNum(), currentProcedureName);
    }

    ParentManager::addStatementToStack(whileNode->getLineNum());
}
