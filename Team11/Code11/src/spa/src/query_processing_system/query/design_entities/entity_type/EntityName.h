#pragma once

#include <string>
#include <utility>
#include "query_processing_system/query/design_entities/entity_type/IEntity.h"

class EntityName : public virtual IEntity {
protected:
    std::string entityName;

public:
    explicit EntityName(std::string name) : entityName(std::move(name)) {
    }
    std::string getName();
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
    std::unordered_set<std::string> getNames() override;
    int getRelationshipCount() override;
    int getValueCount() override;
};
