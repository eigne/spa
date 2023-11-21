#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <string>

class IVisitsStatement;
class IEntity;

class IStatement {
public:
    virtual bool intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                        const std::unordered_map<int, std::unordered_set<int> >& valuePairs) = 0;
    virtual bool intersectWithEntity(const std::shared_ptr<IEntity>& entity,
                                     const std::unordered_map<int, std::unordered_set<std::string> >& valuePairs) = 0;
    virtual bool differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                         const std::unordered_map<int, std::unordered_set<int> >& valuePairs) = 0;
    virtual bool differenceWithEntity(const std::shared_ptr<IEntity>& entity,
                                      const std::unordered_map<int, std::unordered_set<std::string> >& valuePairs) = 0;
    virtual bool isSynonym() = 0;
    virtual void queryAbstractions(const std::shared_ptr<IVisitsStatement>& v) = 0;
    virtual std::unordered_set<int> getStatementNumbers() = 0;
    virtual int getRelationshipCount() = 0;
    virtual int getValueCount() = 0;
};
