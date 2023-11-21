#include "StatementVariableNameAttribute.h"

#include <utility>

StatementVariableNameAttribute::StatementVariableNameAttribute(
        std::shared_ptr<StatementWithVariableName> synonym) {
    this->synonym = std::move(synonym);
}

std::unordered_set<std::string>
StatementVariableNameAttribute::getRelationships(const std::shared_ptr<ISelectable>& other,
                                                  const std::string& thisValue) {
    auto synonymValues = synonym->getSynonymValuesFromVariableName(thisValue);
    std::unordered_set<std::string> res;
    for (const auto& synonymValue : synonymValues) {
        auto valueRes = synonym->getRelationships(other, std::to_string(synonymValue));
        res.insert(valueRes.begin(), valueRes.end());
    }
    return res;
}

std::shared_ptr<ISynonym> StatementVariableNameAttribute::getSynonym() {
    return synonym;
}

std::unordered_set<std::string> StatementVariableNameAttribute::stringifyPossibleValues() {
    return synonym->getVariableNameAttributes();
}

std::string StatementVariableNameAttribute::getSelectedValue(const std::string& synonymValue) {
    return synonym->getCorrespondingVariableName(stoi(synonymValue));
}

std::unordered_map<std::string, std::unordered_set<int>>
StatementVariableNameAttribute::getAttributeAndSynonymValues() {
    auto variableNames = synonym->getVariableNameAttributes();
    std::unordered_map<std::string, std::unordered_set<int>> res;
    for (const auto& variableName : variableNames) {
        res.insert({variableName, synonym->getSynonymValuesFromVariableName(variableName)});
    }
    return res;
}
