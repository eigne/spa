#include "EntityWithProcedureName.h"

void EntityWithProcedureName::setProcedureNameAttributes(const std::unordered_map<std::string, std::string> &values) {
    this->procedureNameMap = values;
}

std::unordered_set<std::string> EntityWithProcedureName::getProcedureNameAttributes() {
    auto currentNames = this->getNames();

    std::unordered_set<std::string> res;
    for (const auto& synonymName : currentNames) {
        res.insert(procedureNameMap.at(synonymName));
    }
    return res;
}

std::unordered_set<std::string>
EntityWithProcedureName::getSynonymValuesFromProcedureName(const std::string &procedureName) {
    auto currentNames = this->getNames();

    std::unordered_set<std::string> res;
    for (const auto& synonymName : currentNames) {
        if (procedureNameMap.at(synonymName) == procedureName) {
            res.insert(synonymName);
        }
    }
    return res;
}

std::string EntityWithProcedureName::getCorrespondingProcedureName(const std::string &synonymValue) {
    return procedureNameMap.at(synonymValue);
}

