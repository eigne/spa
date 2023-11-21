#pragma once

#include <memory>
#include <string>
#include "query_processing_system/query/design_abstractions/pattern/Pattern.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/WhileSynonym.h"
#include "commons/ExpressionParser.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/pattern/WhilePatternVisitor.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"

class WhilePattern : public Pattern, public std::enable_shared_from_this<WhilePattern> {
public:
    std::shared_ptr<WhileSynonym> whileSynonym;
    std::shared_ptr<IVariable> variable;

    WhilePattern(const std::shared_ptr<WhileSynonym>& whileSynonym, const std::shared_ptr<IVariable>& variable,
                 bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateWhilePattern(const std::shared_ptr<IWhilePatternReader>& reader);
    int getPriority() override;
};
