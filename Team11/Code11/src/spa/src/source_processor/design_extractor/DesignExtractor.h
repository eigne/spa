#pragma once

#include <memory>
#include "source_processor/ast/ProgramNode.h"
#include "source_processor/storage_writer/StorageWriter.h"
#include "source_processor/design_extractor/extractors/EntityExtractor.h"
#include "source_processor/design_extractor/extractors/RelationshipExtractor.h"

class DesignExtractor {
private:
    std::unique_ptr<EntityExtractor> entityExtractor;
    std::unique_ptr<RelationshipExtractor> relationshipExtractor;
public:
    explicit DesignExtractor(std::shared_ptr<StorageWriter>& storageWriter);
    void extract(std::shared_ptr<ProgramNode>& node);
};
