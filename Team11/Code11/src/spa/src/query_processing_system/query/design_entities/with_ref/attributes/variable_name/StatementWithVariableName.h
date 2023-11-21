#pragma once

#include <string>
#include <unordered_map>
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"

class StatementWithVariableName : public virtual StatementTypeSynonym {
protected:
    std::unordered_map<int, std::string> variableNameMap;
public:
    void setVariableNameAttributes(const std::unordered_map<int, std::string>& values);
    std::string getCorrespondingVariableName(int synonymValue);
    std::unordered_set<std::string> getVariableNameAttributes();
    std::unordered_set<int> getSynonymValuesFromVariableName(const std::string& variableName);
};
