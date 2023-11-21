#pragma once

#include <memory>
#include <string>
#include "Strategy.h"
#include "source_processor/ast/ASTNode.h"
#include "source_processor/storage_writer/writer_interface/IRelationshipWriter.h"

class AssignStmtStrategy : public Strategy {
    void execute(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                 std::string currProcName) override;
};
