#include "StatementWithVariableName.h"

void StatementWithVariableName::setVariableNameAttributes(const std::unordered_map<int, std::string> &values) {
    this->variableNameMap = values;
}

std::unordered_set<std::string> StatementWithVariableName::getVariableNameAttributes() {
    auto currentStatements = getStatementNumbers();

    std::unordered_set<std::string> res;
    for (auto statement : currentStatements) {
        res.insert(variableNameMap.at(statement));
    }
    return res;
}

std::unordered_set<int>
StatementWithVariableName::getSynonymValuesFromVariableName(const std::string &variableName) {
    auto currentStatements = getStatementNumbers();

    std::unordered_set<int> res;
    for (auto statement : currentStatements) {
        if (variableNameMap.at(statement) == variableName) {
            res.insert(statement);
        }
    }
    return res;
}

std::string StatementWithVariableName::getCorrespondingVariableName(int synonymValue) {
    return variableNameMap.at(synonymValue);
}
