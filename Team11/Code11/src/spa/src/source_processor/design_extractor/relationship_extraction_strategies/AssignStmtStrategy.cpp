#include "AssignStmtStrategy.h"
#include "source_processor/ast/statements/AssignNode.h"
#include "source_processor/design_extractor/relationship_managers/ParentManager.h"

void AssignStmtStrategy::execute(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                                 std::string currProcName) {
    auto assignNode = std::dynamic_pointer_cast<AssignNode>(node);
    auto lhs = assignNode->getLeft();
    auto rhs = assignNode->getRight();

    insertModifiesRelationship(pkb, lhs->getVarName(), lhs->getAssociatedLineNum(), currProcName);

    for (const auto& var : rhs->getVariables()) {
        insertUsesRelationship(pkb, var, lhs->getAssociatedLineNum(), currProcName);
    }
}
