#pragma once

#include <memory>
#include <string>
#include "source_processor/ast/ASTNode.h"
#include "source_processor/storage_writer/writer_interface/IRelationshipWriter.h"

class Strategy {
public:
    virtual void execute(std::shared_ptr<ASTNode> node, std::shared_ptr<IRelationshipWriter> pkb,
                         std::string currProcName) = 0;
    void insertUsesRelationship(const std::shared_ptr<IRelationshipWriter>& pkb, const std::string& varName, int lineNum,
                                const std::string& currProcName);
    void insertModifiesRelationship(const std::shared_ptr<IRelationshipWriter>& pkb, const std::string& varName, int lineNum,
                                    const std::string& currProcName);
};
