#include <algorithm>
#include <execution>
#include "ProcedureWildcard.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsProcedure.h"

std::unordered_set<std::string> ProcedureWildcard::allPossibleProcedures = {};

void ProcedureWildcard::queryAbstractions(const std::shared_ptr<IVisitsProcedure>& v) {
    v->visit(shared_from_this());
}

bool ProcedureWildcard::intersectWithEntity(const std::shared_ptr<IEntity> &entity,
                                         const std::unordered_map<std::string,
                                                 std::unordered_set<std::string> > &valuePairs) {
    return std::any_of(allPossibleProcedures.begin(), allPossibleProcedures.end(), [&](const auto& procedureName) {
        return valuePairs.find(procedureName) != valuePairs.end();
    });
}

bool ProcedureWildcard::intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                            const std::unordered_map<std::string,
                                                    std::unordered_set<int> > &valuePairs) {
    return std::any_of(allPossibleProcedures.begin(), allPossibleProcedures.end(), [&](const auto& procedureName) {
        return valuePairs.find(procedureName) != valuePairs.end();
    });
}

bool ProcedureWildcard::differenceWithEntity(const std::shared_ptr<IEntity> &entity,
                                            const std::unordered_map<std::string,
                                                    std::unordered_set<std::string>> &valuePairs) {
    auto allNames = entity->getNames();

    // check if any of the possible values will still have a remaining corresponding value
    for (const auto& procedureName : allPossibleProcedures) {
        if (valuePairs.find(procedureName) != valuePairs.end()) {
            if (!entity->isSynonym()) {
                return false;
            }

            if (valuePairs.at(procedureName).size() < allNames.size()) {
                return true;
            }

            for (const auto& name : allNames) {
                if (valuePairs.at(procedureName).find(name) == valuePairs.at(procedureName).end()) {
                    return true;
                }
            }
        } else {
            return true;
        }
    }
    return false;
}

bool ProcedureWildcard::differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                                const std::unordered_map<std::string,
                                                        std::unordered_set<int>> &valuePairs) {
    auto allStatementNumbers = statement->getStatementNumbers();

    // check if any of the possible values will still have a remaining corresponding value
    for (const auto& procedureName : allPossibleProcedures) {
        if (valuePairs.find(procedureName) != valuePairs.end()) {
            if (!statement->isSynonym()) {
                return false;
            }

            if (valuePairs.at(procedureName).size() < allStatementNumbers.size()) {
                return true;
            }

            for (auto stmt : allStatementNumbers) {
                if (valuePairs.at(procedureName).find(stmt) == valuePairs.at(procedureName).end()) {
                    return true;
                }
            }
        } else {
            return true;
        }
    }
    return false;
}

std::unordered_set<std::string> ProcedureWildcard::getNames() {
    return allPossibleProcedures;
}
