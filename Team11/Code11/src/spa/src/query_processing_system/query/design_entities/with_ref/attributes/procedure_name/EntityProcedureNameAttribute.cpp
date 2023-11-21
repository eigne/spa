#include "EntityProcedureNameAttribute.h"

#include <utility>

EntityProcedureNameAttribute::EntityProcedureNameAttribute(
        std::shared_ptr<EntityWithProcedureName> synonym) {
    this->synonym = std::move(synonym);
}

std::unordered_set<std::string>
EntityProcedureNameAttribute::getRelationships(const std::shared_ptr<ISelectable>& other,
                                                  const std::string& thisValue) {
    auto synonymValues = synonym->getSynonymValuesFromProcedureName(thisValue);
    std::unordered_set<std::string> res;
    for (const auto& synonymValue : synonymValues) {
        auto valueRes = synonym->getRelationships(other, synonymValue);
        res.insert(valueRes.begin(), valueRes.end());
    }
    return res;
}

std::shared_ptr<ISynonym> EntityProcedureNameAttribute::getSynonym() {
    return synonym;
}

std::unordered_set<std::string> EntityProcedureNameAttribute::stringifyPossibleValues() {
    return synonym->getProcedureNameAttributes();
}

std::string EntityProcedureNameAttribute::getSelectedValue(const std::string& synonymValue) {
    return synonym->getCorrespondingProcedureName(synonymValue);
}

std::unordered_map<std::string, std::unordered_set<std::string>>
EntityProcedureNameAttribute::getAttributeAndSynonymValues() {
    auto procedureNames = synonym->getProcedureNameAttributes();
    std::unordered_map<std::string, std::unordered_set<std::string>> res;
    for (const auto& procedureName : procedureNames) {
        res.insert({procedureName, synonym->getSynonymValuesFromProcedureName(procedureName)});
    }
    return res;
}
