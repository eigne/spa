#pragma once

#include <vector>
#include <memory>
#include "commons/tokenizer/tokens/Token.h"
#include "token_groups/DeclarationTokenGroup.h"

#include "token_groups/SelectTokenGroup.h"
#include "token_groups/TokenGroup.h"
#include "token_groups/clause/PatternTokenGroup.h"
#include "token_groups/clause/WithTokenGroup.h"
#include "token_groups/clause/ClauseTokenGroup.h"
#include "token_groups/clause/InvalidClauseTokenGroup.h"
#include "token_groups/clause/such_that/CallsTokenGroup.h"
#include "token_groups/clause/such_that/FollowsTokenGroup.h"
#include "token_groups/clause/such_that/ModifiesTokenGroup.h"
#include "token_groups/clause/such_that/NextTokenGroup.h"
#include "token_groups/clause/such_that/ParentTokenGroup.h"
#include "token_groups/clause/such_that/SuchThatTokenGroup.h"
#include "token_groups/clause/such_that/UsesTokenGroup.h"
#include "token_groups/clause/such_that/AffectsTokenGroup.h"

typedef std::vector<std::shared_ptr<Token> > TokenList;

class TokenSplitter {
private:
    std::shared_ptr<Token> suchToken = std::make_shared<Token>("such");
    std::shared_ptr<Token> thatToken = std::make_shared<Token>("that");
    std::shared_ptr<Token> patternToken = std::make_shared<Token>("pattern");
    std::shared_ptr<Token> withToken = std::make_shared<Token>("with");
    enum TokenGroupType {SuchThat, Pattern, With, None};
    std::unordered_set<std::string> endWithClauseStrings = {"such", "and"};
    std::vector<TokenList> splitIntoDeclarationsAndSelect(TokenList queryTokens);
    std::vector<DeclarationTokenGroup> splitDeclarations(const TokenList& declarationTokens);
    SelectTokenGroup getSelectTokens(TokenList selectAndClauseTokens);
    std::vector<std::shared_ptr<ClauseTokenGroup> > getClauseTokens(TokenList selectAndClauseTokens,
                                                                    int numberOfSelectTokens);
public:
    std::vector<std::shared_ptr<TokenGroup> > splitIntoTokenGroups(TokenList queryTokens);
};
