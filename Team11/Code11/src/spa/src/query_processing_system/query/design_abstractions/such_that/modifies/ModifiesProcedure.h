#pragma once

#include <memory>
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/IProcedure.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/such_that/modifies/ModifiesProcedureVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class ModifiesProcedure : public SuchThat, public std::enable_shared_from_this<ModifiesProcedure> {
public:
    std::shared_ptr<IProcedure> procedureThatModifies;
    std::shared_ptr<IVariable> variableModified;

    ModifiesProcedure(const std::shared_ptr<IProcedure>& procedureThatModifies,
                      const std::shared_ptr<IVariable>& variableModified,
                      bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateModifiesProcedure(const std::shared_ptr<IModifiesProcedureReader>& reader);
    int getPriority() override;
};
