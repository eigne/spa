#include "EntityWithValue.h"

void EntityWithValue::setValueAttributes(const std::unordered_map<std::string, std::string> &values) {
    this->valueMap = values;
}

std::unordered_set<std::string> EntityWithValue::getValueAttributes() {
    auto currentNames = this->getNames();

    std::unordered_set<std::string> res;
    for (const auto& synonymName : currentNames) {
        res.insert(valueMap.at(synonymName));
    }
    return res;
}

std::unordered_set<std::string>
EntityWithValue::getSynonymValuesFromValue(const std::string &Value) {
    auto currentNames = this->getNames();

    std::unordered_set<std::string> res;
    for (const auto& synonymName : currentNames) {
        if (valueMap.at(synonymName) == Value) {
            res.insert(synonymName);
        }
    }
    return res;
}

std::string EntityWithValue::getCorrespondingValue(const std::string &synonymValue) {
    return valueMap.at(synonymValue);
}

