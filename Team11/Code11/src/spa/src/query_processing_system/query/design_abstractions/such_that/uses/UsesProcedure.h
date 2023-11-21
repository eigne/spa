#pragma once

#include <memory>
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/IProcedure.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/such_that/uses/UsesProcedureVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class UsesProcedure : public SuchThat, public std::enable_shared_from_this<UsesProcedure> {
public:
    std::shared_ptr<IProcedure> procedureThatUses;
    std::shared_ptr<IVariable> variableUsed;

    UsesProcedure(const std::shared_ptr<IProcedure>& procedureThatUses,
                  const std::shared_ptr<IVariable>& variableUsed,
                  bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateUsesProcedure(const std::shared_ptr<IUsesProcedureReader>& reader);
    int getPriority() override;
};
