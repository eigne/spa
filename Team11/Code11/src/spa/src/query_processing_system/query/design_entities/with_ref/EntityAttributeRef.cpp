#include "EntityAttributeRef.h"

#include <utility>
#include "query_processing_system/query_evaluator/design_entity_visitor/with/WithVisitor.h"

EntityAttributeRef::EntityAttributeRef(std::shared_ptr<IEntityAttribute> attribute) {
    this->attribute = std::move(attribute);
}

std::unordered_map<std::string, std::unordered_set<std::string>> EntityAttributeRef::getAttributeMap() {
    return attribute->getAttributeAndSynonymValues();
}

bool EntityAttributeRef::intersectWithEntity(const std::shared_ptr<IEntity> &other,
                                             const std::unordered_map<std::string, std::unordered_set<std::string>> &valuePairs) {
    auto syn = std::dynamic_pointer_cast<EntityTypeSynonym>(attribute->getSynonym());
    std::unordered_map<std::string, std::unordered_set<std::string>> toIntersect;
    auto synonymAttributeMap = attribute->getAttributeAndSynonymValues();

    for (const auto& valuePair : valuePairs) {
        if (synonymAttributeMap.find(valuePair.first) == synonymAttributeMap.end()) {
            continue;
        }

        auto synonymValues = synonymAttributeMap.at(valuePair.first);
        for (const auto& synonymValue : synonymValues) {
            toIntersect.insert({synonymValue, valuePair.second});
        }
    }

    return syn->intersectWithEntity(other, toIntersect);
}

bool EntityAttributeRef::intersectWithStatement(const std::shared_ptr<IStatement> &other,
                                             const std::unordered_map<std::string, std::unordered_set<int>> &valuePairs) {
    auto syn = std::dynamic_pointer_cast<EntityTypeSynonym>(attribute->getSynonym());
    std::unordered_map<std::string, std::unordered_set<int>> toIntersect;
    auto synonymAttributeMap = attribute->getAttributeAndSynonymValues();

    for (const auto& valuePair : valuePairs) {
        if (synonymAttributeMap.find(valuePair.first) == synonymAttributeMap.end()) {
            continue;
        }

        auto synonymValues = synonymAttributeMap.at(valuePair.first);
        for (const auto& synonymValue : synonymValues) {
            toIntersect.insert({synonymValue, valuePair.second});
        }
    }

    return syn->intersectWithStatement(other, toIntersect);
}

bool EntityAttributeRef::differenceWithEntity(const std::shared_ptr<IEntity> &other,
                                              const std::unordered_map<std::string, std::unordered_set<std::string>> &valuePairs) {
    auto syn = std::dynamic_pointer_cast<EntityTypeSynonym>(attribute->getSynonym());
    std::unordered_map<std::string, std::unordered_set<std::string>> toIntersect;
    auto synonymAttributeMap = attribute->getAttributeAndSynonymValues();

    for (const auto& valuePair : valuePairs) {
        if (synonymAttributeMap.find(valuePair.first) == synonymAttributeMap.end()) {
            continue;
        }

        auto synonymValues = synonymAttributeMap.at(valuePair.first);
        for (const auto& synonymValue : synonymValues) {
            toIntersect.insert({synonymValue, valuePair.second});
        }
    }

    return syn->differenceWithEntity(other, toIntersect);
}

bool EntityAttributeRef::differenceWithStatement(const std::shared_ptr<IStatement> &other,
                                                 const std::unordered_map<std::string, std::unordered_set<int>> &valuePairs) {
    auto syn = std::dynamic_pointer_cast<EntityTypeSynonym>(attribute->getSynonym());
    std::unordered_map<std::string, std::unordered_set<int>> toIntersect;
    auto synonymAttributeMap = attribute->getAttributeAndSynonymValues();

    for (const auto& valuePair : valuePairs) {
        if (synonymAttributeMap.find(valuePair.first) == synonymAttributeMap.end()) {
            continue;
        }

        auto synonymValues = synonymAttributeMap.at(valuePair.first);
        for (const auto& synonymValue : synonymValues) {
            toIntersect.insert({synonymValue, valuePair.second});
        }
    }

    return syn->differenceWithStatement(other, toIntersect);
}

bool EntityAttributeRef::visit(WithVisitor& visitor, const std::shared_ptr<WithRef>& other) {
    return visitor.visit(shared_from_this(), other);
}

int EntityAttributeRef::getValueCount() {
    auto syn = std::dynamic_pointer_cast<EntityTypeSynonym>(attribute->getSynonym());
    return syn->getValueCount();
}

int EntityAttributeRef::getRelationshipCount() {
    auto syn = std::dynamic_pointer_cast<EntityTypeSynonym>(attribute->getSynonym());
    return syn->getRelationshipCount();
}
