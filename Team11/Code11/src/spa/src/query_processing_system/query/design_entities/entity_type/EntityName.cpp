#include <algorithm>
#include <execution>
#include "EntityName.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"

std::string EntityName::getName() {
    return entityName;
}

bool EntityName::intersectWithEntity(const std::shared_ptr<IEntity> &entity,
                                     const std::unordered_map<std::string,
                                                              std::unordered_set<std::string> > &valuePairs) {
    return valuePairs.find(entityName) != valuePairs.end();
}

bool EntityName::intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                        const std::unordered_map<std::string, std::unordered_set<int> > &valuePairs) {
    return valuePairs.find(entityName) != valuePairs.end();
}

bool EntityName::differenceWithEntity(const std::shared_ptr<IEntity> &entity,
                                      const std::unordered_map<std::string,
                                                               std::unordered_set<std::string> > &valuePairs) {
    if (valuePairs.find(entityName) == valuePairs.end()) {
        return true;
    }

    auto allNames = entity->getNames();
    if (valuePairs.at(entityName).size() < allNames.size()) {
        return true;
    }

    // check if any of the possible values will still have a remaining corresponding value
    return std::any_of(allNames.begin(), allNames.end(), [&](const auto& name) {
        return valuePairs.at(entityName).find(name) == valuePairs.at(entityName).end();
    });
}

bool EntityName::differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                         const std::unordered_map<std::string, std::unordered_set<int> > &valuePairs) {
    if (valuePairs.find(entityName) == valuePairs.end()) {
        return true;
    }

    auto allStatementNumbers = statement->getStatementNumbers();
    if (valuePairs.at(entityName).size() < allStatementNumbers.size()) {
        return true;
    }

    // check if any of the possible values will still have a remaining corresponding value
    return std::any_of(allStatementNumbers.begin(), allStatementNumbers.end(), [&](const auto& stmt) {
        return valuePairs.at(entityName).find(stmt) != valuePairs.at(entityName).end();
    });
}

bool EntityName::isSynonym() {
    return false;
}

std::unordered_set<std::string> EntityName::getNames() {
    return {entityName};
}

int EntityName::getValueCount() {
    return 1;
}

int EntityName::getRelationshipCount() {
    return 1;
}
