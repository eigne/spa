#include "EntityStatementNumberAttribute.h"

#include <utility>

EntityStatementNumberAttribute::EntityStatementNumberAttribute(
        std::shared_ptr<EntityWithStatementNumber> synonym) {
    this->synonym = std::move(synonym);
}

std::unordered_set<std::string>
EntityStatementNumberAttribute::getRelationships(const std::shared_ptr<ISelectable>& other,
                                               const std::string& thisValue) {
    auto synonymValues = synonym->getSynonymValuesFromStatementNumber(thisValue);
    std::unordered_set<std::string> res;
    for (const auto& synonymValue : synonymValues) {
        auto valueRes = synonym->getRelationships(other, synonymValue);
        res.insert(valueRes.begin(), valueRes.end());
    }
    return res;
}

std::shared_ptr<ISynonym> EntityStatementNumberAttribute::getSynonym() {
    return synonym;
}

std::unordered_set<std::string> EntityStatementNumberAttribute::stringifyPossibleValues() {
    return synonym->getStatementNumberAttributes();
}

std::string EntityStatementNumberAttribute::getSelectedValue(const std::string& synonymValue) {
    return synonym->getCorrespondingStatementNumber(synonymValue);
}

std::unordered_map<std::string, std::unordered_set<std::string>>
EntityStatementNumberAttribute::getAttributeAndSynonymValues() {
    auto statementNumbers = synonym->getStatementNumberAttributes();
    std::unordered_map<std::string, std::unordered_set<std::string>> res;
    for (const auto& statementNumber : statementNumbers) {
        res.insert({statementNumber, synonym->getSynonymValuesFromStatementNumber(statementNumber)});
    }
    return res;
}
