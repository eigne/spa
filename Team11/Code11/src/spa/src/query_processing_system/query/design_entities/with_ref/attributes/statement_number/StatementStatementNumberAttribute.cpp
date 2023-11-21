#include "StatementStatementNumberAttribute.h"

#include <utility>

StatementStatementNumberAttribute::StatementStatementNumberAttribute(
        std::shared_ptr<StatementTypeSynonym> synonym) {
    this->synonym = std::move(synonym);
}

std::unordered_set<std::string>
StatementStatementNumberAttribute::getRelationships(const std::shared_ptr<ISelectable>& other,
                                                  const std::string& thisValue) {
    return synonym->getRelationships(other, thisValue);
}

std::shared_ptr<ISynonym> StatementStatementNumberAttribute::getSynonym() {
    return synonym;
}

std::unordered_set<std::string> StatementStatementNumberAttribute::stringifyPossibleValues() {
    auto values = synonym->getStatementNumbers();
    std::unordered_set<std::string> res;
    for (auto value : values) {
        res.insert(std::to_string(value));
    }
    return res;
}

std::string StatementStatementNumberAttribute::getSelectedValue(const std::string& synonymValue) {
    return synonymValue;
}

std::unordered_map<std::string, std::unordered_set<int>>
StatementStatementNumberAttribute::getAttributeAndSynonymValues() {
    auto statementNumbers = synonym->getStatementNumbers();
    std::unordered_map<std::string, std::unordered_set<int>> res;
    for (auto statementNumber : statementNumbers) {
        res.insert({std::to_string(statementNumber), {statementNumber}});
    }
    return res;
}
