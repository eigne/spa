#include "EntityWithVariableName.h"

void EntityWithVariableName::setVariableNameAttributes(const std::unordered_map<std::string, std::string> &values) {
    this->variableNameMap = values;
}

std::unordered_set<std::string> EntityWithVariableName::getVariableNameAttributes() {
    auto currentNames = this->getNames();

    std::unordered_set<std::string> res;
    for (const auto& synonymName : currentNames) {
        res.insert(variableNameMap.at(synonymName));
    }
    return res;
}

std::unordered_set<std::string>
EntityWithVariableName::getSynonymValuesFromVariableName(const std::string &variableName) {
    auto currentNames = this->getNames();

    std::unordered_set<std::string> res;
    for (const auto& synonymName : currentNames) {
        if (variableNameMap.at(synonymName) == variableName) {
            res.insert(synonymName);
        }
    }
    return res;
}

std::string EntityWithVariableName::getCorrespondingVariableName(const std::string &synonymValue) {
    return variableNameMap.at(synonymValue);
}

