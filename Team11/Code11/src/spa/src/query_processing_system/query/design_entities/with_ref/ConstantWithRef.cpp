#include "ConstantWithRef.h"

#include <utility>
#include <algorithm>
#include <execution>
#include "query_processing_system/query_evaluator/design_entity_visitor/with/WithVisitor.h"

ConstantWithRef::ConstantWithRef(std::string value) {
    this->value = std::move(value);
}

bool ConstantWithRef::intersectWithEntity(const std::shared_ptr<IEntity> &other,
                                          const std::unordered_map<std::string, std::unordered_set<std::string>> &valuePairs) {
    return valuePairs.find(value) != valuePairs.end();
}

bool ConstantWithRef::intersectWithStatement(const std::shared_ptr<IStatement> &other,
                                             const std::unordered_map<std::string, std::unordered_set<int>> &valuePairs) {
    return valuePairs.find(value) != valuePairs.end();
}

bool ConstantWithRef::differenceWithEntity(const std::shared_ptr<IEntity> &other,
                                           const std::unordered_map<std::string, std::unordered_set<std::string>> &valuePairs) {
    if (valuePairs.find(value) == valuePairs.end()) {
        return true;
    }

    auto allNames = other->getNames();
    if (valuePairs.at(value).size() < allNames.size()) {
        return true;
    }

    // check if any of the possible values will still have a remaining corresponding value
    return std::any_of(allNames.begin(), allNames.end(), [&](const auto& name) {
        return valuePairs.at(value).find(name) == valuePairs.at(value).end();
    });
}

bool ConstantWithRef::differenceWithStatement(const std::shared_ptr<IStatement> &other,
                                              const std::unordered_map<std::string, std::unordered_set<int>> &valuePairs) {
    if (valuePairs.find(value) == valuePairs.end()) {
        return true;
    }

    auto statements = other->getStatementNumbers();
    if (valuePairs.at(value).size() < statements.size()) {
        return true;
    }

    // check if any of the possible values will still have a remaining corresponding value
    return std::any_of(statements.begin(), statements.end(), [&](const auto& statement) {
        return valuePairs.at(value).find(statement) == valuePairs.at(value).end();
    });
}

bool ConstantWithRef::visit(WithVisitor& visitor, const std::shared_ptr<WithRef>& other) {
    return visitor.visit(shared_from_this(), other);
}

int ConstantWithRef::getValueCount() {
    return 1;
}

int ConstantWithRef::getRelationshipCount() {
    return 1;
}
