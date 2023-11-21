#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "query_processing_system/query/design_entities/entity_type/IEntity.h"
#include "query_processing_system/query/design_entities/ISynonym.h"
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"


class EntityTypeSynonym : public virtual ISynonym, public virtual IEntity,
    public std::enable_shared_from_this<EntityTypeSynonym> {
private:
    void removeRelationships(
        const std::unordered_set<std::string>& entityNames,
        std::unordered_map<std::shared_ptr<IEntity>,
                           std::unordered_map<std::string,
                                              std::unordered_set<std::string> > >& removedEntityRelationships,
        std::unordered_map<std::shared_ptr<IStatement>,
                           std::unordered_map<int, std::unordered_set<std::string> > >& removedStatementRelationships);
protected:
    std::unordered_set<std::string> possibleEntityNames;

    std::unordered_set<std::shared_ptr<IStatement> > relatedStatements;
    // each statement number has a set of corresponding statement numbers for that other statement
    std::unordered_map<std::string,
                       std::unordered_map<std::shared_ptr<IStatement>,
                                          std::unordered_set<int> > > statementRelationships;

    std::unordered_set<std::shared_ptr<IEntity> > relatedEntities;
    // each statement number has a set of corresponding entity names for that other entity
    std::unordered_map<std::string,
                       std::unordered_map<std::shared_ptr<IEntity>,
                                          std::unordered_set<std::string> > > entityRelationships;
public:
    void setNames(const std::unordered_set<std::string>& names);
    std::unordered_set<std::string> getNames() override;

    // IEntity
    bool intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                const std::unordered_map<std::string, std::unordered_set<int> >& valuePairs) override;
    bool intersectWithEntity(const std::shared_ptr<IEntity>& entity,
                             const std::unordered_map<std::string,
                                                      std::unordered_set<std::string> >& valuePairs) override;
    bool differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                 const std::unordered_map<std::string, std::unordered_set<int> >& valuePairs) override;
    bool differenceWithEntity(const std::shared_ptr<IEntity>& entity,
                              const std::unordered_map<std::string,
                                                       std::unordered_set<std::string> >& valuePairs) override;
    bool isSynonym() override;
    int getValueCount() override;
    int getRelationshipCount() override;

    // ISynonym
    std::shared_ptr<ISynonym> getSynonym() override;
    std::unordered_set<std::string> getRelationships(const std::shared_ptr<ISelectable>& other,
                                                     const std::string& thisValue) override;
    std::unordered_set<std::string> stringifyPossibleValues() override;
    std::string getSynonymType() override;
};
