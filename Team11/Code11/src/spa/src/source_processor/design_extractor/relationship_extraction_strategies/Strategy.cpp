#include "Strategy.h"
#include "source_processor/design_extractor/relationship_managers/ParentManager.h"

void Strategy::insertUsesRelationship(const std::shared_ptr<IRelationshipWriter>& pkb, const std::string& varName, int lineNum,
                                      const std::string& currProcName) {
    pkb->insertUsesS(lineNum, varName);
    pkb->insertUsesP(currProcName, varName);

    for (auto parent : ParentManager::getParentStack()) {
        pkb->insertUsesS(parent, varName);
    }
}

void Strategy::insertModifiesRelationship(const std::shared_ptr<IRelationshipWriter>& pkb, const std::string& varName, int lineNum,
                                          const std::string& currProcName) {
    pkb->insertModifiesS(lineNum, varName);
    pkb->insertModifiesP(currProcName, varName);

    for (auto parent : ParentManager::getParentStack()) {
        pkb->insertModifiesS(parent, varName);
    }
}
