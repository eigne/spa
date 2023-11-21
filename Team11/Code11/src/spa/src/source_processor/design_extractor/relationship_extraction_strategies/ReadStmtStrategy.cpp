#include "ReadStmtStrategy.h"
#include "source_processor/ast/statements/ReadNode.h"
#include "source_processor/design_extractor/relationship_managers/ParentManager.h"

void ReadStmtStrategy::execute(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                               std::string currProcName) {
    auto readNode = std::dynamic_pointer_cast<ReadNode>(node);
    auto v = readNode->getVar();
    insertModifiesRelationship(pkb, v->getVarName(), v->getAssociatedLineNum(), currProcName);
}
