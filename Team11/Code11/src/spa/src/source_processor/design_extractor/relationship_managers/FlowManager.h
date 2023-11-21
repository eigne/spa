#pragma once

#include <memory>
#include <vector>
#include "source_processor/storage_writer/writer_interface/IRelationshipWriter.h"

class FlowManager {
public:
    // using a vector here instead of just an int because of if statements
    static std::vector<int> prevStatements;

    static void addFlow(int stmtIndex, const std::shared_ptr<IRelationshipWriter>& pkb);
    static void resetFlow(int stmtIndex);
    static void initializeFlowManager();
};
