
#include <memory>
#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(std::shared_ptr<StorageWriter>& storageWriter) {
    entityExtractor = std::make_unique<EntityExtractor>(storageWriter->getEntityWriter());
    relationshipExtractor = std::make_unique<RelationshipExtractor>(storageWriter->getRelationshipWriter());
}

void DesignExtractor::extract(std::shared_ptr<ProgramNode>& node) {
    entityExtractor->extract(node);
    relationshipExtractor->extract(node);
}
