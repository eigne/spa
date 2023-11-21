#include <algorithm>
#include <execution>
#include "VariableWildcard.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsVariable.h"

std::unordered_set<std::string> VariableWildcard::allPossibleVariables = {};

void VariableWildcard::queryAbstractions(const std::shared_ptr<IVisitsVariable>& v) {
    v->visit(shared_from_this());
}

bool VariableWildcard::intersectWithEntity(const std::shared_ptr<IEntity> &entity,
                                            const std::unordered_map<std::string,
                                                    std::unordered_set<std::string> > &valuePairs) {
    return std::any_of(allPossibleVariables.begin(), allPossibleVariables.end(), [&](const auto& variable) {
        return valuePairs.find(variable) != valuePairs.end();
    });
}

bool VariableWildcard::intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                               const std::unordered_map<std::string,
                                                       std::unordered_set<int> > &valuePairs) {
    return std::any_of(allPossibleVariables.begin(), allPossibleVariables.end(), [&](const auto& variable) {
        return valuePairs.find(variable) != valuePairs.end();
    });
}

bool VariableWildcard::differenceWithEntity(const std::shared_ptr<IEntity> &entity,
                                             const std::unordered_map<std::string,
                                                     std::unordered_set<std::string>> &valuePairs) {
    auto allNames = entity->getNames();

    // check if any of the possible values will still have a remaining corresponding value
    for (const auto& variable : allPossibleVariables) {
        if (valuePairs.find(variable) != valuePairs.end()) {
            if (!entity->isSynonym()) {
                return false;
            }

            if (valuePairs.at(variable).size() < allNames.size()) {
                return true;
            }

            for (const auto& name : allNames) {
                if (valuePairs.at(variable).find(name) == valuePairs.at(variable).end()) {
                    return true;
                }
            }
        } else {
            return true;
        }
    }
    return false;
}

bool VariableWildcard::differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                                const std::unordered_map<std::string,
                                                        std::unordered_set<int>> &valuePairs) {
    auto allStatementNumbers = statement->getStatementNumbers();

    // check if any of the possible values will still have a remaining corresponding value
    for (const auto& variable : allPossibleVariables) {
        if (valuePairs.find(variable) != valuePairs.end()) {
            if (!statement->isSynonym()) {
                return false;
            }

            if (valuePairs.at(variable).size() < allStatementNumbers.size()) {
                return true;
            }

            for (auto stmt : allStatementNumbers) {
                if (valuePairs.at(variable).find(stmt) == valuePairs.at(variable).end()) {
                    return true;
                }
            }
        } else {
            return true;
        }
    }
    return false;
}

std::unordered_set<std::string> VariableWildcard::getNames() {
    return allPossibleVariables;
}
