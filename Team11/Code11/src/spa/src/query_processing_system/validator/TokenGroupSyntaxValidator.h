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
#include "query_processing_system/tokenizer/token_groups/clause/such_that/UsesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ModifiesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ParentTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/FollowsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/CallsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/NextTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/AffectsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/InvalidClauseTokenGroup.h"

class TokenGroupSyntaxValidator : public SyntaxValidator {
private:
    bool hasBOOLEAN = false;

    const std::unordered_set<std::string> designEntityList {
        "stmt", "read", "print", "call", "while", "if", "assign", "variable", "constant", "procedure"
    };
    const std::unordered_set<std::string> relRefList {
        "Uses", "Follows", "Parent", "Modifies", "Follows*", "Parent*", "Calls", "Calls*", "Next", "Next*", "Affects"
    };

    bool safelyAdvanceIterator(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end);
    bool readArgument(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator &end,
                      std::string& res);
    bool validateQuotationIdent(const std::string& ref);
    bool validateStatementRefs(const std::string& ref);
    bool validateEntityRefs(const std::string& ref);
    bool validateUsesModifiesRef1(const std::string& refOne);
    bool validateUsesAndModifies(const std::string &refOne, const std::string &refTwo);
    bool validateSuchThatRelationship(std::vector<std::string>::iterator &it,
                                      std::vector<std::string>::iterator &end,
                                      std::string &relRef, std::string &refOne, std::string &refTwo);
    bool validateSynonymAttribute(std::vector<std::string, std::allocator<std::string> >::iterator &it,
                                  std::vector<std::string, std::allocator<std::string> >::iterator &end,
                                  std::string &attrName);
    bool validateRef(std::vector<std::string, std::allocator<std::string>>::iterator &it,
                     std::vector<std::string, std::allocator<std::string>>::iterator &end,
                     std::string& ref, std::string& refAttrName, bool& isRefInteger);

    bool visitUsesAndModifies(const std::shared_ptr<SuchThatTokenGroup> &suchThatTokenGroup);
    bool visitParentFollowsNextAffects(const std::shared_ptr<SuchThatTokenGroup> &suchThatTokenGroup);

public:
    TokenGroupSyntaxValidator() = default;

    bool visit(const std::shared_ptr<DeclarationTokenGroup>& declarationTokenGroup);

    bool visit(const std::shared_ptr<SelectTokenGroup>& selectTokenGroup);

    bool visit(const std::shared_ptr<PatternTokenGroup>& patternTokenGroup);

    bool visit(const std::shared_ptr<UsesTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<ModifiesTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<ParentTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<FollowsTokenGroup>& suchThatTokenGroup);

    bool visit(const std::shared_ptr<CallsTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<NextTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<AffectsTokenGroup>& suchThatTokenGroup);
    bool visit(const std::shared_ptr<WithTokenGroup>& withTokenGroup);

    bool visit(const std::shared_ptr<InvalidClauseTokenGroup>& invalidTokenGroup);
};
