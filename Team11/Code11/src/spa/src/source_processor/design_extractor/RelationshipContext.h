#pragma once

#include <memory>
#include "relationship_extraction_strategies/Strategy.h"

class RelationshipContext {
private:
    std::unique_ptr<Strategy> strategy;

public:
    void setStrategy(std::unique_ptr<Strategy> strategy);
    void executeStrategy(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                         std::string currProcName);
};
