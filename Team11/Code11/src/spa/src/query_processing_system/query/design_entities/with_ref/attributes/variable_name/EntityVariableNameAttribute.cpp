#include "EntityVariableNameAttribute.h"

#include <utility>

EntityVariableNameAttribute::EntityVariableNameAttribute(
        std::shared_ptr<EntityWithVariableName> synonym) {
    this->synonym = std::move(synonym);
}

std::unordered_set<std::string>
EntityVariableNameAttribute::getRelationships(const std::shared_ptr<ISelectable>& other,
                                                  const std::string& thisValue) {
    auto synonymValues = synonym->getSynonymValuesFromVariableName(thisValue);
    std::unordered_set<std::string> res;
    for (const auto& synonymValue : synonymValues) {
        auto valueRes = synonym->getRelationships(other, synonymValue);
        res.insert(valueRes.begin(), valueRes.end());
    }
    return res;
}

std::shared_ptr<ISynonym> EntityVariableNameAttribute::getSynonym() {
    return synonym;
}

std::unordered_set<std::string> EntityVariableNameAttribute::stringifyPossibleValues() {
    return synonym->getVariableNameAttributes();
}

std::string EntityVariableNameAttribute::getSelectedValue(const std::string& synonymValue) {
    return synonym->getCorrespondingVariableName(synonymValue);
}

std::unordered_map<std::string, std::unordered_set<std::string>>
EntityVariableNameAttribute::getAttributeAndSynonymValues() {
    auto variableNames = synonym->getVariableNameAttributes();
    std::unordered_map<std::string, std::unordered_set<std::string>> res;
    for (const auto& variableName : variableNames) {
        res.insert({variableName, synonym->getSynonymValuesFromVariableName(variableName)});
    }
    return res;
}
