#pragma once

#include <memory>
#include "writer_interface/IEntityWriter.h"
#include "writer_interface/IRelationshipWriter.h"
#include "program_knowledge_base/facades/PKBSP.h"

class StorageWriter {
private:
    std::shared_ptr<IEntityWriter> entityWriter;
    std::shared_ptr<IRelationshipWriter> relationshipWriter;

public:
    explicit StorageWriter(std::shared_ptr<PKBSP>& writer);

    std::shared_ptr<IEntityWriter> getEntityWriter();
    std::shared_ptr<IRelationshipWriter> getRelationshipWriter();
};
