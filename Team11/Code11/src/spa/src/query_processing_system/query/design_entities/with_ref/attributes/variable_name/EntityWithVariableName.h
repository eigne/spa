#pragma once

#include <string>
#include <unordered_map>
#include "query_processing_system/query/design_entities/entity_type/EntityTypeSynonym.h"

class EntityWithVariableName : public virtual EntityTypeSynonym {
protected:
    std::unordered_map<std::string, std::string> variableNameMap;
public:
    void setVariableNameAttributes(const std::unordered_map<std::string, std::string>& values);
    std::string getCorrespondingVariableName(const std::string& synonymValue);
    std::unordered_set<std::string> getVariableNameAttributes();
    std::unordered_set<std::string> getSynonymValuesFromVariableName(const std::string& variableName);
};
