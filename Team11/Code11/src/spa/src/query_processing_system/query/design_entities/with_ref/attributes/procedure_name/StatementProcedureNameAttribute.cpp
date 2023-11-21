#include "StatementProcedureNameAttribute.h"

#include <utility>

StatementProcedureNameAttribute::StatementProcedureNameAttribute(
        std::shared_ptr<StatementWithProcedureName> synonym) {
    this->synonym = std::move(synonym);
}

std::unordered_set<std::string>
StatementProcedureNameAttribute::getRelationships(const std::shared_ptr<ISelectable>& other,
                                                  const std::string& thisValue) {
    auto synonymValues = synonym->getSynonymValuesFromProcedureName(thisValue);
    std::unordered_set<std::string> res;
    for (const auto& synonymValue : synonymValues) {
        auto valueRes = synonym->getRelationships(other, std::to_string(synonymValue));
        res.insert(valueRes.begin(), valueRes.end());
    }
    return res;
}

std::shared_ptr<ISynonym> StatementProcedureNameAttribute::getSynonym() {
    return synonym;
}

std::unordered_set<std::string> StatementProcedureNameAttribute::stringifyPossibleValues() {
    return synonym->getProcedureNameAttributes();
}

std::string StatementProcedureNameAttribute::getSelectedValue(const std::string& synonymValue) {
    return synonym->getCorrespondingProcedureName(stoi(synonymValue));
}

std::unordered_map<std::string, std::unordered_set<int>>
StatementProcedureNameAttribute::getAttributeAndSynonymValues() {
    auto procedureNames = synonym->getProcedureNameAttributes();
    std::unordered_map<std::string, std::unordered_set<int>> res;
    for (const auto& procedureName : procedureNames) {
        res.insert({procedureName, synonym->getSynonymValuesFromProcedureName(procedureName)});
    }
    return res;
}
