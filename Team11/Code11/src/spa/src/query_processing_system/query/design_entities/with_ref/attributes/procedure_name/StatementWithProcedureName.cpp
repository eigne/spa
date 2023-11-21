#include "StatementWithProcedureName.h"

void StatementWithProcedureName::setProcedureNameAttributes(const std::unordered_map<int, std::string> &values) {
    this->procedureNameMap = values;
}

std::unordered_set<std::string> StatementWithProcedureName::getProcedureNameAttributes() {
    auto currentStatements = getStatementNumbers();

    std::unordered_set<std::string> res;
    for (auto statement : currentStatements) {
        res.insert(procedureNameMap.at(statement));
    }
    return res;
}

std::unordered_set<int>
StatementWithProcedureName::getSynonymValuesFromProcedureName(const std::string &procedureName) {
    auto currentStatements = getStatementNumbers();

    std::unordered_set<int> res;
    for (auto statement : currentStatements) {
        if (procedureNameMap.at(statement) == procedureName) {
            res.insert(statement);
        }
    }
    return res;
}

std::string StatementWithProcedureName::getCorrespondingProcedureName(int synonymValue) {
    return procedureNameMap.at(synonymValue);
}
