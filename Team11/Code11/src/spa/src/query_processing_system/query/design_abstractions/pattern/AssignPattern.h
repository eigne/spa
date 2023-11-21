#pragma once

#include <memory>
#include <string>
#include "query_processing_system/query/design_abstractions/pattern/Pattern.h"
#include "query_processing_system/query/design_entities/entity_type/variable/IVariable.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/AssignSynonym.h"
#include "commons/ExpressionParser.h"
#include "query_processing_system/query_evaluator/storage_reader/IStorageReader.h"
#include "query_processing_system/query_evaluator/design_entity_visitor/pattern/AssignPatternVisitor.h"

class AssignPattern : public Pattern, public std::enable_shared_from_this<AssignPattern> {
public:
    std::shared_ptr<AssignSynonym> assignment;
    std::shared_ptr<IVariable> variable;
    std::vector<std::string> postfixExpression;
    bool hasWildcard;

    AssignPattern(const std::shared_ptr<AssignSynonym>& assignment, const std::shared_ptr<IVariable>& variable,
                  const std::string& expression, bool hasWildcard, bool isNegated);

    bool evaluate(const std::shared_ptr<IStorageReader>& reader) override;
    bool evaluateAssignPattern(const std::shared_ptr<IAssignPatternReader>& reader);
    int getPriority() override;
};
