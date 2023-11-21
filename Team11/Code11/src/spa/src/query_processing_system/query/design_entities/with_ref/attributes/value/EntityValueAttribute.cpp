#include "EntityValueAttribute.h"

#include <utility>

EntityValueAttribute::EntityValueAttribute(
        std::shared_ptr<EntityWithValue> synonym) {
    this->synonym = std::move(synonym);
}

std::unordered_set<std::string>
EntityValueAttribute::getRelationships(const std::shared_ptr<ISelectable>& other,
                                               const std::string& thisValue) {
    auto synonymValues = synonym->getSynonymValuesFromValue(thisValue);
    std::unordered_set<std::string> res;
    for (const auto& synonymValue : synonymValues) {
        auto valueRes = synonym->getRelationships(other, synonymValue);
        res.insert(valueRes.begin(), valueRes.end());
    }
    return res;
}

std::shared_ptr<ISynonym> EntityValueAttribute::getSynonym() {
    return synonym;
}

std::unordered_set<std::string> EntityValueAttribute::stringifyPossibleValues() {
    return synonym->getValueAttributes();
}

std::string EntityValueAttribute::getSelectedValue(const std::string& synonymValue) {
    return synonym->getCorrespondingValue(synonymValue);
}

std::unordered_map<std::string, std::unordered_set<std::string>>
EntityValueAttribute::getAttributeAndSynonymValues() {
    auto values = synonym->getValueAttributes();
    std::unordered_map<std::string, std::unordered_set<std::string>> res;
    for (const auto& value : values) {
        res.insert({value, synonym->getSynonymValuesFromValue(value)});
    }
    return res;
}
