#pragma once

#include <unordered_map>
#include <set>
#include <string>
#include <memory>

#include "query_processing_system/tokenizer/token_groups/DeclarationTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/SelectTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/PatternTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/WithTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/SuchThatTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/UsesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ModifiesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/FollowsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ParentTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/CallsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/NextTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/AffectsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/InvalidClauseTokenGroup.h"

#include "query_processing_system/query/design_entities/statement_type/synonyms/StatementSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/ReadSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/PrintSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/CallSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/WhileSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/IfSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/synonyms/AssignSynonym.h"
#include "query_processing_system/query/design_entities/statement_type/StatementLineNumber.h"
#include "query_processing_system/query/design_entities/statement_type/StatementWildcard.h"

#include "query_processing_system/query/design_entities/entity_type/constant/ConstantSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/ProcedureSynonym.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableName.h"
#include "query_processing_system/query/design_entities/entity_type/variable/VariableWildcard.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/ProcedureName.h"
#include "query_processing_system/query/design_entities/entity_type/procedure/ProcedureWildcard.h"

#include "query_processing_system/query/design_abstractions/such_that/uses/UsesStatement.h"
#include "query_processing_system/query/design_abstractions/such_that/uses/UsesProcedure.h"
#include "query_processing_system/query/design_abstractions/such_that/modifies/ModifiesStatement.h"
#include "query_processing_system/query/design_abstractions/such_that/modifies/ModifiesProcedure.h"
#include "query_processing_system/query/design_abstractions/such_that/follows/Follows.h"
#include "query_processing_system/query/design_abstractions/such_that/parent/Parent.h"
#include "query_processing_system/query/design_abstractions/such_that/calls/Calls.h"
#include "query_processing_system/query/design_abstractions/such_that/next/Next.h"
#include "query_processing_system/query/design_abstractions/such_that/affects/Affects.h"
#include "query_processing_system/query/design_abstractions/pattern/AssignPattern.h"
#include "query_processing_system/query/design_abstractions/pattern/IfPattern.h"
#include "query_processing_system/query/design_abstractions/pattern/WhilePattern.h"
#include "query_processing_system/query/design_abstractions/with/With.h"

#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/StatementVariableNameAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/variable_name/EntityVariableNameAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/statement_number/StatementStatementNumberAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/statement_number/EntityStatementNumberAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/value/StatementValueAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/value/EntityValueAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/procedure_name/StatementProcedureNameAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/procedure_name/EntityProcedureNameAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/ConstantWithRef.h"
#include "query_processing_system/query/design_entities/with_ref/EntityAttributeRef.h"
#include "query_processing_system/query/design_entities/with_ref/StatementAttributeRef.h"


class TokenGroupExtractor {
private:
    std::unordered_map<std::string, std::shared_ptr<ISynonym>> synonymMap;
    std::shared_ptr<ISynonym> createSynonym(const std::string& type, const std::string& name);
    std::shared_ptr<ISelectable> createSelectable(const std::shared_ptr<ISynonym>& synonym,
                                                  const std::string& selectionType);
    std::shared_ptr<WithRef> createWithRef(const std::string& ref,
                                           const std::string& attrRef,
                                           bool refIsInteger);
public:
    void visit(const std::shared_ptr<DeclarationTokenGroup>& declarationTokenGroup, Query& q);

    void visit(const std::shared_ptr<SelectTokenGroup>& selectTokenGroup, Query& q);

    void visit(const std::shared_ptr<PatternTokenGroup>& patternTokenGroup, Query& q);
    void visit(const std::shared_ptr<UsesTokenGroup>& usesTokenGroup, Query& q);
    void visit(const std::shared_ptr<ModifiesTokenGroup>& modifiesTokenGroup, Query& q);
    void visit(const std::shared_ptr<FollowsTokenGroup>& followsTokenGroup, Query& q);
    void visit(const std::shared_ptr<ParentTokenGroup>& parentTokenGroup, Query& q);
    void visit(const std::shared_ptr<CallsTokenGroup>& callsTokenGroup, Query& q);
    void visit(const std::shared_ptr<NextTokenGroup>& nextTokenGroup, Query& q);
    void visit(const std::shared_ptr<WithTokenGroup>& withTokenGroup, Query& q);
    void visit(const std::shared_ptr<AffectsTokenGroup>& affectsTokenGroup, Query& q);
    void visit(const std::shared_ptr<InvalidClauseTokenGroup>& invalidTokenGroup, Query& q);
};
