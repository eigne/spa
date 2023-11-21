#pragma once

#include <memory>
#include "query_processing_system/query/design_abstractions/such_that/SuchThat.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/IProcedure.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/such_that/calls/CallsVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class Calls : public SuchThat, public std::enable_shared_from_this<Calls> {
public:
    bool hasStar;
    std::shared_ptr<IProcedure> callingProcedure;
    std::shared_ptr<IProcedure> calledProcedure;

    Calls(const std::shared_ptr<IProcedure>& callingProcedure,
          const std::shared_ptr<IProcedure>& calledProcedure,
          bool hasStar, bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateCall(const std::shared_ptr<ICallsReader>& reader);
    int getPriority() override;
};
