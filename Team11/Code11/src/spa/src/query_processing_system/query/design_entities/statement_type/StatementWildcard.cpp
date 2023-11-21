#include <algorithm>
#include <execution>
#include "StatementWildcard.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"

std::unordered_set<int> StatementWildcard::allPossibleStatements = {};

bool StatementWildcard::intersectWithEntity(const std::shared_ptr<IEntity> &entity,
                                            const std::unordered_map<int,
                                                                     std::unordered_set<std::string> > &valuePairs) {
    return std::any_of(allPossibleStatements.begin(), allPossibleStatements.end(), [&](const auto& stmt) {
        return valuePairs.find(stmt) != valuePairs.end();
    });
}

bool StatementWildcard::intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                               const std::unordered_map<int, std::unordered_set<int> > &valuePairs) {
    return std::any_of(allPossibleStatements.begin(), allPossibleStatements.end(), [&](const auto& stmt) {
        return valuePairs.find(stmt) != valuePairs.end();
    });
}

bool StatementWildcard::differenceWithEntity(const std::shared_ptr<IEntity> &entity,
                                             const std::unordered_map<int,
                                                                      std::unordered_set<std::string> > &valuePairs) {
    auto allNames = entity->getNames();

    // check if any of the possible values will still have a remaining corresponding value
    for (auto stmt : allPossibleStatements) {
        if (valuePairs.find(stmt) != valuePairs.end()) {
            if (!entity->isSynonym()) {
                return false;
            }

            if (valuePairs.at(stmt).size() < allNames.size()) {
                return true;
            }

            for (const auto& name : allNames) {
                if (valuePairs.at(stmt).find(name) == valuePairs.at(stmt).end()) {
                    return true;
                }
            }
        } else {
            return true;
        }
    }
    return false;
}

bool StatementWildcard::differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                                const std::unordered_map<int, std::unordered_set<int> > &valuePairs) {
    auto allStatementNumbers = statement->getStatementNumbers();

    // check if any of the possible values will still have a remaining corresponding value
    for (auto thisStmt : allPossibleStatements) {
        if (valuePairs.find(thisStmt) != valuePairs.end()) {
            if (!statement->isSynonym()) {
                return false;
            }

            if (valuePairs.at(thisStmt).size() < allStatementNumbers.size()) {
                return true;
            }

            for (auto stmt : allStatementNumbers) {
                if (valuePairs.at(thisStmt).find(stmt) == valuePairs.at(thisStmt).end()) {
                    return true;
                }
            }
        } else {
            return true;
        }
    }
    return false;
}

bool StatementWildcard::isSynonym() {
    return false;
}

void StatementWildcard::queryAbstractions(const std::shared_ptr<IVisitsStatement> &v) {
    v->visit(shared_from_this());
}

std::unordered_set<int> StatementWildcard::getStatementNumbers() {
    return allPossibleStatements;
}

int StatementWildcard::getValueCount() {
    return allPossibleStatements.size();
}

int StatementWildcard::getRelationshipCount() {
    return 1;
}
