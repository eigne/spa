#include "StatementWithValue.h"

void StatementWithValue::setValueAttributes(const std::unordered_map<int, std::string> &values) {
    this->valueMap = values;
}

std::unordered_set<std::string> StatementWithValue::getValueAttributes() {
    auto currentStatements = getStatementNumbers();

    std::unordered_set<std::string> res;
    for (auto statement : currentStatements) {
        res.insert(valueMap.at(statement));
    }
    return res;
}

std::unordered_set<int>
StatementWithValue::getSynonymValuesFromValue(const std::string &value) {
    auto currentStatements = getStatementNumbers();

    std::unordered_set<int> res;
    for (auto statement : currentStatements) {
        if (valueMap.at(statement) == value) {
            res.insert(statement);
        }
    }
    return res;
}

std::string StatementWithValue::getCorrespondingValue(int synonymValue) {
    return valueMap.at(synonymValue);
}
