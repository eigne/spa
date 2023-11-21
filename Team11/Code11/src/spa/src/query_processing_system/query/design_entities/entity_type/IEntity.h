#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class IStatement;

class IEntity {
public:
    virtual std::unordered_set<std::string> getNames() = 0;
    virtual bool intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                        const std::unordered_map<std::string,
                                                                 std::unordered_set<int> >& valuePairs) = 0;
    virtual bool intersectWithEntity(const std::shared_ptr<IEntity>& entity,
                                     const std::unordered_map<std::string,
                                                              std::unordered_set<std::string> >& valuePairs) = 0;
    virtual bool differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                         const std::unordered_map<std::string,
                                                                  std::unordered_set<int> >& valuePairs) = 0;
    virtual bool differenceWithEntity(const std::shared_ptr<IEntity>& entity,
                                      const std::unordered_map<std::string,
                                                               std::unordered_set<std::string> >& valuePairs) = 0;
    virtual bool isSynonym() = 0;
    virtual int getRelationshipCount() = 0;
    virtual int getValueCount() = 0;
};
