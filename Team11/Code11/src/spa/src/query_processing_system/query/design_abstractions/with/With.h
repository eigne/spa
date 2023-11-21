#pragma once

#include <memory>
#include <string>
#include "query_processing_system/query/design_abstractions/Clause.h"
#include "query_processing_system/query/design_entities/with_ref/WithRef.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/with/WithVisitor.h"

class With : public Clause, public std::enable_shared_from_this<With> {
private:
    std::shared_ptr<WithRef> ref1;
    std::shared_ptr<WithRef> ref2;
public:
    With(std::shared_ptr<WithRef> ref1, std::shared_ptr<WithRef> ref2, bool isNegated);
    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateWith();
    int getPriority() override;
};

