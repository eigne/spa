#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/statement_type/IStatement.h"

class StatementLineNumber : public IStatement, public std::enable_shared_from_this<StatementLineNumber> {
private:
    int lineNumber;

public:
    explicit StatementLineNumber(int lineNumber);

    int getStatementLineNumber() const;
    bool intersectWithStatement(const std::shared_ptr<IStatement> &statement,
                                const std::unordered_map<int, std::unordered_set<int> >& valuePairs) override;
    bool intersectWithEntity(const std::shared_ptr<IEntity>& entity,
                             const std::unordered_map<int, std::unordered_set<std::string> >& valuePairs) override;
    bool differenceWithStatement(const std::shared_ptr<IStatement> &statement,
                                 const std::unordered_map<int, std::unordered_set<int> >& valuePairs) override;
    bool differenceWithEntity(const std::shared_ptr<IEntity>& entity,
                              const std::unordered_map<int, std::unordered_set<std::string> >& valuePairs) override;
    bool isSynonym() override;
    void queryAbstractions(const std::shared_ptr<IVisitsStatement>& v) override;
    std::unordered_set<int> getStatementNumbers() override;
    int getRelationshipCount() override;
    int getValueCount() override;
};
