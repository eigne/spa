#include "StatementValueAttribute.h"

#include <utility>

StatementValueAttribute::StatementValueAttribute(
        std::shared_ptr<StatementWithValue> synonym) {
    this->synonym = std::move(synonym);
}

std::unordered_set<std::string>
StatementValueAttribute::getRelationships(const std::shared_ptr<ISelectable>& other,
                                                  const std::string& thisValue) {
    auto synonymValues = synonym->getSynonymValuesFromValue(thisValue);
    std::unordered_set<std::string> res;
    for (const auto& synonymValue : synonymValues) {
        auto valueRes = synonym->getRelationships(other, std::to_string(synonymValue));
        res.insert(valueRes.begin(), valueRes.end());
    }
    return res;
}

std::shared_ptr<ISynonym> StatementValueAttribute::getSynonym() {
    return synonym;
}

std::unordered_set<std::string> StatementValueAttribute::stringifyPossibleValues() {
    return synonym->getValueAttributes();
}

std::string StatementValueAttribute::getSelectedValue(const std::string& synonymValue) {
    return synonym->getCorrespondingValue(stoi(synonymValue));
}

std::unordered_map<std::string, std::unordered_set<int>>
StatementValueAttribute::getAttributeAndSynonymValues() {
    auto values = synonym->getValueAttributes();
    std::unordered_map<std::string, std::unordered_set<int>> res;
    for (const auto& value : values) {
        res.insert({value, synonym->getSynonymValuesFromValue(value)});
    }
    return res;
}
