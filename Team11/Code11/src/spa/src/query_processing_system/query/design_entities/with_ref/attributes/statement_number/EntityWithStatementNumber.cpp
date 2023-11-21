#include "EntityWithStatementNumber.h"

void EntityWithStatementNumber::setStatementNumberAttributes(const std::unordered_map<std::string, std::string> &values) {
    this->StatementNumberMap = values;
}

std::unordered_set<std::string> EntityWithStatementNumber::getStatementNumberAttributes() {
    auto currentNames = this->getNames();

    std::unordered_set<std::string> res;
    for (const auto& synonymName : currentNames) {
        res.insert(StatementNumberMap.at(synonymName));
    }
    return res;
}

std::unordered_set<std::string>
EntityWithStatementNumber::getSynonymValuesFromStatementNumber(const std::string &StatementNumber) {
    auto currentNames = this->getNames();

    std::unordered_set<std::string> res;
    for (const auto& synonymName : currentNames) {
        if (StatementNumberMap.at(synonymName) == StatementNumber) {
            res.insert(synonymName);
        }
    }
    return res;
}

std::string EntityWithStatementNumber::getCorrespondingStatementNumber(const std::string &synonymValue) {
    return StatementNumberMap.at(synonymValue);
}

