#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "commons/validator/SyntaxValidator.h"

#include "query_processing_system/tokenizer/token_groups/DeclarationTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/SelectTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/PatternTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/WithTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/SuchThatTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/FollowsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/UsesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ModifiesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ParentTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/CallsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/NextTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/AffectsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/InvalidClauseTokenGroup.h"

class TokenGroupSemanticValidator : public SyntaxValidator {
private:
    enum SynonymType { VARIABLE, PROCEDURE, STATEMENT, ASSIGNMENT , IF, WHILE};
    enum AttributeType { STATEMENT_NUMBER, VARIABLE_NAME, PROCEDURE_NAME, VALUE };
    enum AttributeTypeGeneric {INTEGER, NAME};

    const std::unordered_map<std::string, std::vector<SynonymType> > designEntityList {
        {"stmt", {SynonymType::STATEMENT}},
        {"read", {SynonymType::STATEMENT}},
        {"print", {SynonymType::STATEMENT}},
        {"call", {SynonymType::STATEMENT}},
        {"while", {SynonymType::STATEMENT, SynonymType::WHILE}},
        {"if", {SynonymType::STATEMENT, SynonymType::IF}},
        {"assign", {SynonymType::STATEMENT, SynonymType::ASSIGNMENT}},
        {"variable", {SynonymType::VARIABLE}},
        {"constant", {}},
        {"procedure", {SynonymType::PROCEDURE}}
    };

    const std::unordered_map<std::string, std::vector<AttributeType> > attributeMap {
        {"stmt", {AttributeType::STATEMENT_NUMBER}},
        {"read", {AttributeType::STATEMENT_NUMBER, AttributeType::VARIABLE_NAME}},
        {"print", {AttributeType::STATEMENT_NUMBER, AttributeType::VARIABLE_NAME}},
        {"call", {AttributeType::STATEMENT_NUMBER, AttributeType::PROCEDURE_NAME}},
        {"while", {AttributeType::STATEMENT_NUMBER}},
        {"if", {AttributeType::STATEMENT_NUMBER}},
        {"assign", {AttributeType::STATEMENT_NUMBER}},
        {"variable", {AttributeType::VARIABLE_NAME}},
        {"constant", {AttributeType::VALUE}},
        {"procedure", {AttributeType::PROCEDURE_NAME}}
    };

    std::unordered_map<std::string, std::vector<SynonymType> > synonymMap;
    std::unordered_map<std::string, std::string> synonymMapWithType;

    bool validateSynonymType(const std::string& ref, SynonymType requiredType);
    bool validateSynonymAttribute(const std::string& synonymType, const std::string& attributeType);

    bool validateStatementStatement(const std::shared_ptr<SuchThatTokenGroup> &suchThatTokenGroup);
    bool validateModifiesAndUses(const std::shared_ptr<SuchThatTokenGroup> &suchThatTokenGroup);
    bool validateAssignPattern(const std::shared_ptr<PatternTokenGroup> &patternTokenGroup);
    bool validateIfPattern(const std::shared_ptr<PatternTokenGroup> &patternTokenGroup);
    bool validateWhilePattern(const std::shared_ptr<PatternTokenGroup> &patternTokenGroup);

    bool findType(AttributeTypeGeneric &refOneRes, std::string &ref, std::string &refAttrName, bool &isRefInteger);

public:
    TokenGroupSemanticValidator();

    bool visit(const std::shared_ptr<DeclarationTokenGroup>& declarationTokenGroup);

    bool visit(const std::shared_ptr<SelectTokenGroup>& selectTokenGroup);

    bool visit(const std::shared_ptr<PatternTokenGroup>& patternTokenGroup);

    bool visit(const std::shared_ptr<FollowsTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<UsesTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<ModifiesTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<ParentTokenGroup>& suchThatTokenGroup);

    bool visit(const std::shared_ptr<CallsTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<NextTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<AffectsTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<WithTokenGroup>& withTokenGroup);

    bool visit(const std::shared_ptr<InvalidClauseTokenGroup>& invalidTokenGroup);
};
