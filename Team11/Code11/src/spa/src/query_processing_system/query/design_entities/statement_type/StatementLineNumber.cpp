#include <algorithm>
#include <execution>
#include "StatementLineNumber.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/interfaces/visit/IVisitsStatement.h"

StatementLineNumber::StatementLineNumber(int lineNumber) {
    this->lineNumber = lineNumber;
}

int StatementLineNumber::getStatementLineNumber() const {
    return lineNumber;
}

bool StatementLineNumber::intersectWithEntity(const std::shared_ptr<IEntity> &entity,
                                              const std::unordered_map<int,
                                                                       std::unordered_set<std::string> > &valuePairs) {
    return valuePairs.find(lineNumber) != valuePairs.end();
}

bool StatementLineNumber::intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                                 const std::unordered_map<int, std::unordered_set<int> > &valuePairs) {
    return valuePairs.find(lineNumber) != valuePairs.end();
}

bool StatementLineNumber::differenceWithEntity(const std::shared_ptr<IEntity> &entity,
                                               const std::unordered_map<int,
                                                                        std::unordered_set<std::string> > &valuePairs) {
    if (valuePairs.find(lineNumber) == valuePairs.end()) {
        return true;
    }

    if (!entity->isSynonym()) {
        return false;
    }

    auto allNames = entity->getNames();
    if (valuePairs.at(lineNumber).size() < allNames.size()) {
        return true;
    }

    // check if any of the possible values will still have a remaining corresponding value
    return std::any_of(allNames.begin(), allNames.end(), [&](const auto& stmt) {
        return valuePairs.at(lineNumber).find(stmt) == valuePairs.at(lineNumber).end();
    });
}

bool StatementLineNumber::differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                                  const std::unordered_map<int, std::unordered_set<int> > &valuePairs) {
    if (valuePairs.find(lineNumber) == valuePairs.end()) {
        return true;
    }

    if (!statement->isSynonym()) {
        return false;
    }

    auto allStatementNumbers = statement->getStatementNumbers();
    if (valuePairs.at(lineNumber).size() < allStatementNumbers.size()) {
        return true;
    }

    // check if any of the possible values will still have a remaining corresponding value
    return std::any_of(allStatementNumbers.begin(), allStatementNumbers.end(), [&](const auto& stmt) {
        return valuePairs.at(lineNumber).find(stmt) == valuePairs.at(lineNumber).end();
    });
}

bool StatementLineNumber::isSynonym() {
    return false;
}

void StatementLineNumber::queryAbstractions(const std::shared_ptr<IVisitsStatement> &v) {
    v->visit(shared_from_this());
}

std::unordered_set<int> StatementLineNumber::getStatementNumbers() {
    return {lineNumber};
}

int StatementLineNumber::getValueCount() {
    return 1;
}

int StatementLineNumber::getRelationshipCount() {
    return 1;
}
