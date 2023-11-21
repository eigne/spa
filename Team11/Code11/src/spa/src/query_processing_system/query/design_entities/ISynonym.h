#pragma once

#include <string>
#include <memory>
#include <list>
#include "query_processing_system/query/query_selection/ISelectable.h"

class IVisitsSynonym;

class ISynonym : public ISelectable {
public:
    std::string synonymIdentifier;
    virtual void initializePossibleValues(const std::shared_ptr<IVisitsSynonym>& sc) = 0;
    virtual std::string getSynonymType() = 0;
    std::string getSelectedValue(const std::string& synonymValue) override { return synonymValue; }
};
