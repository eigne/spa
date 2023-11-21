#pragma once

#include <string>
#include <unordered_map>
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"

class StatementWithValue : public virtual StatementTypeSynonym {
protected:
    std::unordered_map<int, std::string> valueMap;
public:
    void setValueAttributes(const std::unordered_map<int, std::string>& values);
    std::string getCorrespondingValue(int synonymValue);
    std::unordered_set<std::string> getValueAttributes();
    std::unordered_set<int> getSynonymValuesFromValue(const std::string& value);
};
