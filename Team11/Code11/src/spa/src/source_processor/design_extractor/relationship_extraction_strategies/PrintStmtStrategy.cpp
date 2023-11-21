#include "PrintStmtStrategy.h"
#include "source_processor/ast/statements/PrintNode.h"
#include "source_processor/design_extractor/relationship_managers/ParentManager.h"

void PrintStmtStrategy::execute(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                                std::string currentProcedureName) {
    auto printNode = std::dynamic_pointer_cast<PrintNode>(node);
    auto v = printNode->getVar();
    insertUsesRelationship(pkb, v->getVarName(), v->getAssociatedLineNum(), currentProcedureName);
}
