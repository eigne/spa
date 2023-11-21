#include "StatementAttributeRef.h"

#include <utility>
#include "query_processing_system/query_evaluator/design_entity_visitor/with/WithVisitor.h"

StatementAttributeRef::StatementAttributeRef(std::shared_ptr<IStatementAttribute> attribute) {
    this->attribute = std::move(attribute);
}

std::unordered_map<std::string, std::unordered_set<int>> StatementAttributeRef::getAttributeMap() {
    return attribute->getAttributeAndSynonymValues();
}

bool StatementAttributeRef::intersectWithEntity(const std::shared_ptr<IEntity> &other,
                                             const std::unordered_map<std::string, std::unordered_set<std::string>> &valuePairs) {
    auto syn = std::dynamic_pointer_cast<StatementTypeSynonym>(attribute->getSynonym());
    std::unordered_map<int, std::unordered_set<std::string>> toIntersect;
    auto synonymAttributeMap = attribute->getAttributeAndSynonymValues();

    for (const auto& valuePair : valuePairs) {
        if (synonymAttributeMap.find(valuePair.first) == synonymAttributeMap.end()) {
            continue;
        }

        auto synonymValues = synonymAttributeMap.at(valuePair.first);
        for (auto synonymValue : synonymValues) {
            toIntersect.insert({synonymValue, valuePair.second});
        }
    }

    return syn->intersectWithEntity(other, toIntersect);
}

bool StatementAttributeRef::intersectWithStatement(const std::shared_ptr<IStatement> &other,
                                                const std::unordered_map<std::string, std::unordered_set<int>> &valuePairs) {
    auto syn = std::dynamic_pointer_cast<StatementTypeSynonym>(attribute->getSynonym());
    std::unordered_map<int, std::unordered_set<int>> toIntersect;
    auto synonymAttributeMap = attribute->getAttributeAndSynonymValues();

    for (const auto& valuePair : valuePairs) {
        if (synonymAttributeMap.find(valuePair.first) == synonymAttributeMap.end()) {
            continue;
        }

        auto synonymValues = synonymAttributeMap.at(valuePair.first);
        for (auto synonymValue : synonymValues) {
            toIntersect.insert({synonymValue, valuePair.second});
        }
    }

    return syn->intersectWithStatement(other, toIntersect);
}

bool StatementAttributeRef::differenceWithEntity(const std::shared_ptr<IEntity> &other,
                                              const std::unordered_map<std::string, std::unordered_set<std::string>> &valuePairs) {
    auto syn = std::dynamic_pointer_cast<StatementTypeSynonym>(attribute->getSynonym());
    std::unordered_map<int, std::unordered_set<std::string>> toIntersect;
    auto synonymAttributeMap = attribute->getAttributeAndSynonymValues();

    for (const auto& valuePair : valuePairs) {
        if (synonymAttributeMap.find(valuePair.first) == synonymAttributeMap.end()) {
            continue;
        }

        auto synonymValues = synonymAttributeMap.at(valuePair.first);
        for (auto synonymValue : synonymValues) {
            toIntersect.insert({synonymValue, valuePair.second});
        }
    }

    return syn->differenceWithEntity(other, toIntersect);
}

bool StatementAttributeRef::differenceWithStatement(const std::shared_ptr<IStatement> &other,
                                                 const std::unordered_map<std::string, std::unordered_set<int>> &valuePairs) {
    auto syn = std::dynamic_pointer_cast<StatementTypeSynonym>(attribute->getSynonym());
    std::unordered_map<int, std::unordered_set<int>> toIntersect;
    auto synonymAttributeMap = attribute->getAttributeAndSynonymValues();

    for (const auto& valuePair : valuePairs) {
        if (synonymAttributeMap.find(valuePair.first) == synonymAttributeMap.end()) {
            continue;
        }

        auto synonymValues = synonymAttributeMap.at(valuePair.first);
        for (auto synonymValue : synonymValues) {
            toIntersect.insert({synonymValue, valuePair.second});
        }
    }

    return syn->differenceWithStatement(other, toIntersect);
}

bool StatementAttributeRef::visit(WithVisitor& visitor, const std::shared_ptr<WithRef>& other) {
    return visitor.visit(shared_from_this(), other);
}

int StatementAttributeRef::getValueCount() {
    auto syn = std::dynamic_pointer_cast<StatementTypeSynonym>(attribute->getSynonym());
    return syn->getValueCount();
}

int StatementAttributeRef::getRelationshipCount() {
    auto syn = std::dynamic_pointer_cast<StatementTypeSynonym>(attribute->getSynonym());
    return syn->getRelationshipCount();
}
