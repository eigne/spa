#pragma once

#include <unordered_set>
#include <string>
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"
#include "query_processing_system/query/design_entities/entity_type/IEntity.h"

class WithVisitor;

class WithRef {
public:
    virtual bool intersectWithEntity(const std::shared_ptr<IEntity>& other,
                                     const std::unordered_map<std::string,
                                                              std::unordered_set<std::string>>& valuePairs) = 0;
    virtual bool intersectWithStatement(const std::shared_ptr<IStatement>& other,
                                        const std::unordered_map<std::string,
                                                                 std::unordered_set<int>>& valuePairs) = 0;
    virtual bool differenceWithEntity(const std::shared_ptr<IEntity>& other,
                                      const std::unordered_map<std::string,
                                                               std::unordered_set<std::string>>& valuePairs) = 0;
    virtual bool differenceWithStatement(const std::shared_ptr<IStatement>& other,
                                         const std::unordered_map<std::string,
                                                                  std::unordered_set<int>>& valuePairs) = 0;
    virtual bool visit(WithVisitor& visitor, const std::shared_ptr<WithRef>& other) = 0;
    virtual int getValueCount() = 0;
    virtual int getRelationshipCount() = 0;
};
