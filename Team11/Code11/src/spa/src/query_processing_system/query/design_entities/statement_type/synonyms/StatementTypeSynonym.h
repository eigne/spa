#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include "query_processing_system/query/design_entities/ISynonym.h"
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"
#include "query_processing_system/query/design_entities/entity_type/IEntity.h"

class StatementTypeSynonym : public virtual ISynonym, public IStatement,
        public std::enable_shared_from_this<StatementTypeSynonym> {
private:
    void removeRelationships(
        const std::unordered_set<int>& statementNumbers,
        std::unordered_map<std::shared_ptr<IEntity>,
                           std::unordered_map<std::string, std::unordered_set<int> > >& removedEntityRelationships,
        std::unordered_map<std::shared_ptr<IStatement>,
                           std::unordered_map<int, std::unordered_set<int> > >& removedStatementRelationships);
protected:
    std::unordered_set<int> possibleStatementNumbers;

    std::unordered_set<std::shared_ptr<IStatement> > relatedStatements;
// each statement number has a set of corresponding statement numbers for that other statement
    std::unordered_map<int,
                       std::unordered_map<std::shared_ptr<IStatement>,
                                          std::unordered_set<int> > > statementRelationships;

    std::unordered_set<std::shared_ptr<IEntity> > relatedEntities;
// each statement number has a set of corresponding entity names for that other entity
    std::unordered_map<int,
                       std::unordered_map<std::shared_ptr<IEntity>,
                                          std::unordered_set<std::string> > > entityRelationships;
public:
    void setStatementNumbers(const std::unordered_set<int>& numbers);
    std::unordered_set<int> getStatementNumbers() override;

    // IStatement
    bool intersectWithEntity(const std::shared_ptr<IEntity> &entity,
                             const std::unordered_map<int, std::unordered_set<std::string> >& valuePairs) override;
    bool intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                const std::unordered_map<int, std::unordered_set<int> >& valuePairs) override;
    bool differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                 const std::unordered_map<int, std::unordered_set<int> >& valuePairs) override;
    bool differenceWithEntity(const std::shared_ptr<IEntity>& entity,
                              const std::unordered_map<int, std::unordered_set<std::string> >& valuePairs) override;
    bool isSynonym() override;
    int getValueCount() override;
    int getRelationshipCount() override;

    // ISynonym
    std::shared_ptr<ISynonym> getSynonym() override;
    std::unordered_set<std::string> stringifyPossibleValues() override;
    std::unordered_set<std::string> getRelationships(const std::shared_ptr<ISelectable>& other,
                                                     const std::string& thisValue) override;
    void queryAbstractions(const std::shared_ptr<IVisitsStatement>& v) override;
    std::string getSynonymType() override;
};
