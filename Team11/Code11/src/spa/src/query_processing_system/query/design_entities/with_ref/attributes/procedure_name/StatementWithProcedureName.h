#pragma once

#include <string>
#include <unordered_map>
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"

class StatementWithProcedureName : public virtual StatementTypeSynonym {
protected:
    std::unordered_map<int, std::string> procedureNameMap;
public:
    void setProcedureNameAttributes(const std::unordered_map<int, std::string>& values);
    std::string getCorrespondingProcedureName(int synonymValue);
    std::unordered_set<std::string> getProcedureNameAttributes();
    std::unordered_set<int> getSynonymValuesFromProcedureName(const std::string& procedureName);
};
