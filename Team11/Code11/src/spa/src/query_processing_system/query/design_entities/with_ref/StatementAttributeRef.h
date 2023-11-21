#pragma once

#include "query_processing_system/query/design_entities/with_ref/WithRef.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/IStatementAttribute.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementTypeSynonym.h"

class StatementAttributeRef : public WithRef, public std::enable_shared_from_this<StatementAttributeRef> {
public:
    std::shared_ptr<IStatementAttribute> attribute;

    explicit StatementAttributeRef(std::shared_ptr<IStatementAttribute> attribute);
    std::unordered_map<std::string, std::unordered_set<int>> getAttributeMap();
    bool intersectWithEntity(const std::shared_ptr<IEntity>& other,
                             const std::unordered_map<std::string,
                                                      std::unordered_set<std::string>>& valuePairs) override;
    bool intersectWithStatement(const std::shared_ptr<IStatement>& other,
                                const std::unordered_map<std::string,
                                                         std::unordered_set<int>>& valuePairs) override;
    bool differenceWithEntity(const std::shared_ptr<IEntity>& other,
                              const std::unordered_map<std::string,
                                                       std::unordered_set<std::string>>& valuePairs) override;
    bool differenceWithStatement(const std::shared_ptr<IStatement>& other,
                                 const std::unordered_map<std::string,
                                                          std::unordered_set<int>>& valuePairs) override;
    bool visit(WithVisitor& visitor, const std::shared_ptr<WithRef>& other) override;
    int getValueCount() override;
    int getRelationshipCount() override;
};
