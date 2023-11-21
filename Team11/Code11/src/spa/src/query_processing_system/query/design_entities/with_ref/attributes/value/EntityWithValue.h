#pragma once

#include <string>
#include <unordered_map>
#include "query_processing_system/query/design_entities/entity_type/EntityTypeSynonym.h"

class EntityWithValue : public virtual EntityTypeSynonym {
protected:
    std::unordered_map<std::string, std::string> valueMap;
public:
    void setValueAttributes(const std::unordered_map<std::string, std::string>& values);
    std::string getCorrespondingValue(const std::string& synonymValue);
    std::unordered_set<std::string> getValueAttributes();
    std::unordered_set<std::string> getSynonymValuesFromValue(const std::string& Value);
};
