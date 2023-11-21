#pragma once

#include <string>
#include <unordered_map>
#include "query_processing_system/query/design_entities/entity_type/EntityTypeSynonym.h"

class EntityWithProcedureName : public virtual EntityTypeSynonym {
protected:
    std::unordered_map<std::string, std::string> procedureNameMap;
public:
    void setProcedureNameAttributes(const std::unordered_map<std::string, std::string>& values);
    std::string getCorrespondingProcedureName(const std::string& synonymValue);
    std::unordered_set<std::string> getProcedureNameAttributes();
    std::unordered_set<std::string> getSynonymValuesFromProcedureName(const std::string& procedureName);
};
