#pragma once

#include <string>
#include <unordered_map>
#include "query_processing_system/query/design_entities/entity_type/EntityTypeSynonym.h"

class EntityWithStatementNumber : public virtual EntityTypeSynonym {
protected:
    std::unordered_map<std::string, std::string> StatementNumberMap;
public:
    void setStatementNumberAttributes(const std::unordered_map<std::string, std::string>& values);
    std::string getCorrespondingStatementNumber(const std::string& synonymValue);
    std::unordered_set<std::string> getStatementNumberAttributes();
    std::unordered_set<std::string> getSynonymValuesFromStatementNumber(const std::string& StatementNumber);
};
