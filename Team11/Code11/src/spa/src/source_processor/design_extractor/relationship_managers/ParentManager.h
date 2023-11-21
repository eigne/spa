#pragma once

#include <vector>
#include <memory>
#include "source_processor/storage_writer/writer_interface/IRelationshipWriter.h"

class ParentManager {
private:
    static inline std::vector<int> parentStack{};

public:
    static const std::vector<int>& getParentStack();
    static void clearParentStack();
    static void removeLastParent();
    static void addStatementToStack(int parent);
    static void addChild(int child, const std::shared_ptr<IRelationshipWriter>& pkb);
};
