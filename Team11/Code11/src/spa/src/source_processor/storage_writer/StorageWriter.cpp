#include <memory>
#include "StorageWriter.h"
#include "source_processor/storage_writer/writer/EntityWriter.h"
#include "source_processor/storage_writer/writer/RelationshipWriter.h"


StorageWriter::StorageWriter(std::shared_ptr<PKBSP>& writer) {
    entityWriter = std::make_shared<EntityWriter>(writer);
    relationshipWriter = std::make_shared<RelationshipWriter>(writer);
}

std::shared_ptr<IEntityWriter> StorageWriter::getEntityWriter() {
    return entityWriter;
}

std::shared_ptr<IRelationshipWriter> StorageWriter::getRelationshipWriter() {
    return relationshipWriter;
}
