#include "RelationshipContext.h"

#include <utility>

void RelationshipContext::setStrategy(std::unique_ptr<Strategy> strategy) {
    this->strategy = std::move(strategy);
}

void RelationshipContext::executeStrategy(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                                          std::string currProcName) {
    strategy->execute(std::move(node), std::move(pkb), std::move(currProcName));
}
