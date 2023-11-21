#pragma once

#include <vector>
#include <memory>
#include "source_processor/storage_writer/writer_interface/IRelationshipWriter.h"

class FollowsManager {
private:
    static inline std::vector<std::vector<int> > followsStack{};

public:
    static void clearFollowsStack();
    static void addNewNestingLevel();
    static void removeLastNestingLevel();
    static void addStatementToStack(int stmtNum);
    static void addFollows(int stmtNum, const std::shared_ptr<IRelationshipWriter>& pkb);
};
