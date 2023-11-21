#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "query_processing_system/query/query_selection/ISelectable.h"
#include "query_processing_system/query/design_entities/ISynonym.h"

class IEntityAttribute : public ISelectable {
public:
    virtual std::unordered_map<std::string, std::unordered_set<std::string>> getAttributeAndSynonymValues() = 0;
};
