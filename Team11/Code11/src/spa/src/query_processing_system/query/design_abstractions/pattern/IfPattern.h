#pragma once

#include <memory>
#include <string>
#include "query_processing_system/query/design_abstractions/pattern/Pattern.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/IfSynonym.h"
#include "commons/ExpressionParser.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/pattern/IfPatternVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class IfPattern : public Pattern, public std::enable_shared_from_this<IfPattern> {
public:
    std::shared_ptr<IfSynonym> ifSynonym;
    std::shared_ptr<IVariable> variable;

    IfPattern(const std::shared_ptr<IfSynonym>& ifSynonym, const std::shared_ptr<IVariable>& variable, bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateIfPattern(const std::shared_ptr<IIfPatternReader>& reader);
    int getPriority() override;
};
