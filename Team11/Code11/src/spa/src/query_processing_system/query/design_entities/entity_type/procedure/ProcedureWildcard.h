#pragma once

#include <memory>
#include "query_processing_system/query/design_entities/entity_type/EntityWildcard.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/IProcedure.h"

class ProcedureWildcard : public IProcedure, public EntityWildcard,
    public std::enable_shared_from_this<ProcedureWildcard> {
public:
    static std::unordered_set<std::string> allPossibleProcedures;
    void queryAbstractions(const std::shared_ptr<IVisitsProcedure>& v) override;
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
    std::unordered_set<std::string> getNames() override;
};
