#include "EntityTypeSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"

void EntityTypeSynonym::setNames(const std::unordered_set<std::string>& names) {
    this->possibleEntityNames = names;
    this->relatedStatements = {};
    this->statementRelationships = {};
    this->relatedEntities = {shared_from_this()};
    this->entityRelationships = {};

    for (const auto& name : names) {
        statementRelationships.insert({name, {}});
        entityRelationships.insert({name, {{shared_from_this(), {name}}}});
    }
}

std::unordered_set<std::string> EntityTypeSynonym::getNames() {
    return possibleEntityNames;
}

void EntityTypeSynonym::removeRelationships(
    const std::unordered_set<std::string>& entityNames,
    std::unordered_map<std::shared_ptr<IEntity>,
                       std::unordered_map<std::string, std::unordered_set<std::string> > >& removedEntityRelationships,
    std::unordered_map<std::shared_ptr<IStatement>,
                       std::unordered_map<int, std::unordered_set<std::string> > >& removedStatementRelationships) {
    for (const auto& entityName : entityNames) {
        possibleEntityNames.erase(entityName);

        // remove entity relationships
        auto entitiesToRemove = entityRelationships.at(entityName);
        for (const auto& toRemove : entitiesToRemove) {
            auto entity = toRemove.first;
            auto strings = toRemove.second;
            if (removedEntityRelationships.find(entity) == removedEntityRelationships.end()) {
                removedEntityRelationships.insert({entity, {}});
            }

            auto& removedRelationships = removedEntityRelationships.at(entity);
            for (const auto& string : strings) {
                if (removedRelationships.find(string) == removedRelationships.end()) {
                    removedRelationships.insert({string, {entityName}});
                } else {
                    auto& set = removedRelationships.at(string);
                    set.insert(entityName);
                }
            }
        }
        entityRelationships.erase(entityName);

        // remove statement relationships
        auto statementsToRemove = statementRelationships.at(entityName);
        for (const auto& toRemove : statementsToRemove) {
            auto relatedStatement = toRemove.first;
            auto stmtNums = toRemove.second;
            if (removedStatementRelationships.find(relatedStatement) == removedStatementRelationships.end()) {
                removedStatementRelationships.insert({relatedStatement, {}});
            }

            auto& removedRelationships = removedStatementRelationships.at(relatedStatement);
            for (auto stmtNum : stmtNums) {
                if (removedRelationships.find(stmtNum) == removedRelationships.end()) {
                    removedRelationships.insert({stmtNum, {entityName}});
                } else {
                    auto& set = removedRelationships.at(stmtNum);
                    set.insert(entityName);
                }
            }
        }
        statementRelationships.erase(entityName);
    }
}

bool EntityTypeSynonym::intersectWithEntity(const std::shared_ptr<IEntity>& entity,
                                            const std::unordered_map<std::string,
                                                                     std::unordered_set<std::string> >& valuePairs) {
    std::unordered_map<std::shared_ptr<IEntity>,
                       std::unordered_map<std::string, std::unordered_set<std::string> > > removedEntityRelationships;
    for (const auto& relatedEntity : relatedEntities) {
        removedEntityRelationships.insert({relatedEntity, {}});
    }
    removedEntityRelationships.insert({entity, {}});

    std::unordered_map<std::shared_ptr<IStatement>,
                       std::unordered_map<int, std::unordered_set<std::string> > > removedStatementRelationships;
    for (const auto& relatedStatement : relatedStatements) {
        removedStatementRelationships.insert({relatedStatement, {}});
    }

    // remove old possibilities not in the new possibilities
    std::unordered_set<std::string> excludedEntityNames;
    for (const auto& oldPossibility : possibleEntityNames) {
        if (valuePairs.find(oldPossibility) == valuePairs.end()) {
            excludedEntityNames.insert(oldPossibility);
        }
    }
    removeRelationships(excludedEntityNames, removedEntityRelationships, removedStatementRelationships);

    bool isNewRelationship = relatedEntities.find(entity) == relatedEntities.end();
    if (entity->isSynonym()) {
        if (isNewRelationship) {
            relatedEntities.insert(entity);
            for (const auto& valuePair : valuePairs) {
                if (entityRelationships.find(valuePair.first) != entityRelationships.end()) {
                    entityRelationships.at(valuePair.first).insert({entity, valuePair.second});
                }
            }
        } else {
            std::unordered_set<std::string> removedNames;

            for (auto& currentRelationship : entityRelationships) {
                auto currentPossibilities = currentRelationship.second.at(entity);
                auto newPossibilities = valuePairs.at(currentRelationship.first); // set of new

                std::unordered_set<std::string> intersect;
                for (const auto &possibility : newPossibilities) {
                    if (currentPossibilities.find(possibility) != currentPossibilities.end()) {
                        intersect.insert(possibility);
                    }
                }
                if (intersect.empty()) {
                    removedNames.insert(currentRelationship.first);
                } else {
                    currentRelationship.second[entity] = intersect;
                }
            }

            removeRelationships(removedNames, removedEntityRelationships, removedStatementRelationships);
        }
    }

    for (const auto& removed : removedEntityRelationships) {
        if (removed.second.empty()) continue;
        (removed.first)->differenceWithEntity(shared_from_this(), removed.second);
    }
    for (const auto& removed : removedStatementRelationships) {
        if (removed.second.empty()) continue;
        (removed.first)->differenceWithEntity(shared_from_this(), removed.second);
    }

    if (entity->isSynonym() && isNewRelationship) {
        // for all related entities/synonyms, have them intersect with "entity"
        for (const auto& relatedEntity : relatedEntities) {
            if (relatedEntity == entity || relatedEntity == shared_from_this()) continue;
            std::unordered_map<std::string, std::unordered_set<std::string>> forwardIntersect;
            std::unordered_map<std::string, std::unordered_set<std::string>> backwardIntersect;

            for (const auto& value : entityRelationships) {
                for (const auto& otherValue : value.second.at(relatedEntity)) {
                    for (const auto& toIntersectValue : value.second.at(entity)) {
                        if (forwardIntersect.find(otherValue) == forwardIntersect.end()) {
                            forwardIntersect.insert({otherValue, {}});
                        }
                        forwardIntersect.at(otherValue).insert(toIntersectValue);

                        if (backwardIntersect.find(toIntersectValue) == backwardIntersect.end()) {
                            backwardIntersect.insert({toIntersectValue, {}});
                        }
                        backwardIntersect.at(toIntersectValue).insert(otherValue);
                    }
                }
            }
            relatedEntity->intersectWithEntity(entity, forwardIntersect);
            entity->intersectWithEntity(relatedEntity, backwardIntersect);
        }
        for (const auto& relatedStatement : relatedStatements) {
            std::unordered_map<int, std::unordered_set<std::string>> forwardIntersect;
            std::unordered_map<std::string, std::unordered_set<int>> backwardIntersect;

            for (const auto& value : statementRelationships) {
                for (const auto& otherValue : value.second.at(relatedStatement)) {
                    for (const auto& toIntersectValue : entityRelationships.at(value.first).at(entity)) {
                        if (forwardIntersect.find(otherValue) == forwardIntersect.end()) {
                            forwardIntersect.insert({otherValue, {}});
                        }
                        forwardIntersect.at(otherValue).insert(toIntersectValue);

                        if (backwardIntersect.find(toIntersectValue) == backwardIntersect.end()) {
                            backwardIntersect.insert({toIntersectValue, {}});
                        }
                        backwardIntersect.at(toIntersectValue).insert(otherValue);
                    }
                }
            }
            relatedStatement->intersectWithEntity(entity, forwardIntersect);
            entity->intersectWithStatement(relatedStatement, backwardIntersect);
        }
    }

    return !possibleEntityNames.empty();
}

bool EntityTypeSynonym::intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                               const std::unordered_map<std::string,
                                                                        std::unordered_set<int> >& valuePairs) {
    std::unordered_map<std::shared_ptr<IEntity>,
                       std::unordered_map<std::string, std::unordered_set<std::string> > > removedEntityRelationships;
    for (const auto& relatedEntity : relatedEntities) {
        removedEntityRelationships.insert({relatedEntity, {}});
    }

    std::unordered_map<std::shared_ptr<IStatement>,
                       std::unordered_map<int, std::unordered_set<std::string> > > removedStatementRelationships;
    for (const auto& relatedStatement : relatedStatements) {
        removedStatementRelationships.insert({relatedStatement, {}});
    }
    removedStatementRelationships.insert({statement, {}});

    // remove old possibilities not in the new possibilities
    std::unordered_set<std::string> excludedEntityNames;
    for (const auto& oldPossibility : possibleEntityNames) {
        if (valuePairs.find(oldPossibility) == valuePairs.end()) {
            excludedEntityNames.insert(oldPossibility);
        }
    }
    removeRelationships(excludedEntityNames, removedEntityRelationships, removedStatementRelationships);

    bool isNewRelationship = relatedStatements.find(statement) == relatedStatements.end();
    if (statement->isSynonym()) {
        if (isNewRelationship) {
            relatedStatements.insert(statement);
            for (const auto& valuePair : valuePairs) {
                if (statementRelationships.find(valuePair.first) != statementRelationships.end()) {
                    statementRelationships.at(valuePair.first).insert({statement, valuePair.second});
                }
            }
        } else {
            std::unordered_set<std::string> removedNames;

            for (auto& currentRelationship : statementRelationships) {
                auto currentPossibilities = currentRelationship.second.at(statement);
                auto newPossibilities = valuePairs.at(currentRelationship.first); // set of new

                std::unordered_set<int> intersect;
                for (const auto &possibility : newPossibilities) {
                    if (currentPossibilities.find(possibility) != currentPossibilities.end()) {
                        intersect.insert(possibility);
                    }
                }
                if (intersect.empty()) {
                    removedNames.insert(currentRelationship.first);
                } else {
                    currentRelationship.second[statement] = intersect;
                }
            }

            removeRelationships(removedNames, removedEntityRelationships, removedStatementRelationships);
        }
    }

    for (const auto& removed : removedEntityRelationships) {
        if (removed.second.empty()) continue;
        (removed.first)->differenceWithEntity(shared_from_this(), removed.second);
    }
    for (const auto& removed : removedStatementRelationships) {
        if (removed.second.empty()) continue;
        (removed.first)->differenceWithEntity(shared_from_this(), removed.second);
    }

    if (statement->isSynonym() && isNewRelationship) {
        // for all related entities/synonyms, have them intersect with "statement"
        for (const auto& relatedEntity : relatedEntities) {
            if (relatedEntity == shared_from_this()) continue;
            std::unordered_map<std::string, std::unordered_set<int>> forwardIntersect;
            std::unordered_map<int, std::unordered_set<std::string>> backwardIntersect;

            for (const auto& value : entityRelationships) {
                for (const auto& otherValue : value.second.at(relatedEntity)) {
                    for (const auto& toIntersectValue : statementRelationships.at(value.first).at(statement)) {
                        if (forwardIntersect.find(otherValue) == forwardIntersect.end()) {
                            forwardIntersect.insert({otherValue, {}});
                        }
                        forwardIntersect.at(otherValue).insert(toIntersectValue);

                        if (backwardIntersect.find(toIntersectValue) == backwardIntersect.end()) {
                            backwardIntersect.insert({toIntersectValue, {}});
                        }
                        backwardIntersect.at(toIntersectValue).insert(otherValue);
                    }
                }
            }
            relatedEntity->intersectWithStatement(statement, forwardIntersect);
            statement->intersectWithEntity(relatedEntity, backwardIntersect);
        }
        for (const auto& relatedStatement : relatedStatements) {
            if (relatedStatement == statement) continue;
            std::unordered_map<int, std::unordered_set<int>> forwardIntersect;
            std::unordered_map<int, std::unordered_set<int>> backwardIntersect;
            for (const auto& value : statementRelationships) {
                for (const auto& otherValue : value.second.at(relatedStatement)) {
                    for (const auto& toIntersectValue : value.second.at(statement)) {
                        if (forwardIntersect.find(otherValue) == forwardIntersect.end()) {
                            forwardIntersect.insert({otherValue, {}});
                        }
                        forwardIntersect.at(otherValue).insert(toIntersectValue);

                        if (backwardIntersect.find(toIntersectValue) == backwardIntersect.end()) {
                            backwardIntersect.insert({toIntersectValue, {}});
                        }
                        backwardIntersect.at(toIntersectValue).insert(otherValue);
                    }
                }
            }
            relatedStatement->intersectWithStatement(statement, forwardIntersect);
            statement->intersectWithStatement(relatedStatement, backwardIntersect);
        }
    }

    return !possibleEntityNames.empty();
}

bool EntityTypeSynonym::differenceWithEntity(const std::shared_ptr<IEntity> &entity,
                                             const std::unordered_map<std::string,
                                                                      std::unordered_set<std::string> > &valuePairs) {
    std::unordered_map<std::string, std::unordered_set<std::string> > toIntersect;
    for (const auto& entityName : possibleEntityNames) {
        if (entity->isSynonym()) {
            std::unordered_set<std::string> toIntersectNames = entity->getNames();
            if (valuePairs.find(entityName) != valuePairs.end()) {
                for (const auto& name : valuePairs.at(entityName)) {
                    if (toIntersectNames.find(name) != toIntersectNames.end()) {
                        toIntersectNames.erase(name);
                    }
                }
            }
            if (!toIntersectNames.empty()) {
                toIntersect.insert({entityName, toIntersectNames});
            }
        } else {
            if (valuePairs.find(entityName) == valuePairs.end()) {
                toIntersect.insert({entityName, {""}});
            }
        }
    }
    return intersectWithEntity(entity, toIntersect);
}

bool EntityTypeSynonym::differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                                const std::unordered_map<std::string,
                                                                         std::unordered_set<int> > &valuePairs) {
    std::unordered_map<std::string, std::unordered_set<int> > toIntersect;
    for (const auto& entityName : possibleEntityNames) {
        if (statement->isSynonym()) {
            std::unordered_set<int> toIntersectNumbers = statement->getStatementNumbers();
            if (valuePairs.find(entityName) != valuePairs.end()) {
                for (const auto& number : valuePairs.at(entityName)) {
                    if (toIntersectNumbers.find(number) != toIntersectNumbers.end()) {
                        toIntersectNumbers.erase(number);
                    }
                }
            }
            if (!toIntersectNumbers.empty()) {
                toIntersect.insert({entityName, toIntersectNumbers});
            }
        } else {
            if (valuePairs.find(entityName) == valuePairs.end()) {
                toIntersect.insert({entityName, {0}});
            }
        }
    }
    return intersectWithStatement(statement, toIntersect);
}

bool EntityTypeSynonym::isSynonym() {
    return true;
}

std::unordered_set<std::string> EntityTypeSynonym::getRelationships(const std::shared_ptr<ISelectable>& other,
                                                                    const std::string& thisValue) {
    if (other == shared_from_this()) {
        return {thisValue};
    }

    std::unordered_set<std::string> relationships;
    auto otherSynonym = other->getSynonym();
    if (otherSynonym->getSynonymType() == "statement") {
        auto selectSynonym = std::dynamic_pointer_cast<IStatement>(otherSynonym);
        auto thisValueRelationships = statementRelationships.at(thisValue);
        if (thisValueRelationships.find(selectSynonym) == thisValueRelationships.end()) {
            auto otherValues = other->stringifyPossibleValues();
            return {otherValues.begin(), otherValues.end()};
        } else {
            auto otherValues = thisValueRelationships.at(selectSynonym);
            for (auto value : otherValues) {
                relationships.insert(other->getSelectedValue(std::to_string(value)));
            }
        }
    } else {
        auto entitySynonym = std::dynamic_pointer_cast<IEntity>(otherSynonym);
        auto thisValueRelationships = entityRelationships.at(thisValue);
        if (thisValueRelationships.find(entitySynonym) == thisValueRelationships.end()) {
            auto otherValues = other->stringifyPossibleValues();
            return {otherValues.begin(), otherValues.end()};
        } else {
            auto otherValues = thisValueRelationships.at(entitySynonym);
            for (const auto& value : otherValues) {
                relationships.insert(other->getSelectedValue(value));
            }
        }
    }
    return relationships;
}

std::shared_ptr<ISynonym> EntityTypeSynonym::getSynonym() {
    return shared_from_this();
}

std::unordered_set<std::string> EntityTypeSynonym::stringifyPossibleValues() {
    return possibleEntityNames;
}

std::string EntityTypeSynonym::getSynonymType() {
    return "entity";
}

int EntityTypeSynonym::getValueCount() {
    return possibleEntityNames.size();
}

int EntityTypeSynonym::getRelationshipCount() {
    return relatedStatements.size() + relatedEntities.size();
}
