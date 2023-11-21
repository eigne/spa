#include "catch.hpp"
#include "query_processing_system/query_extractor/QueryExtractor.h"
#include "query_processing_system/validator/QPSValidator.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/UsesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ModifiesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/FollowsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ParentTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/PatternTokenGroup.h"


TEST_CASE("No clauses one declaration") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor(tokenGroups);
    Query q = extractor.extractQuery();

    REQUIRE(q.getClauses().empty());
    REQUIRE(q.getSynonyms().size() == 1);
}


TEST_CASE("No clauses comma declaration") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(","));
    declarationTokens.push_back(std::make_shared<Token>("b"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("b"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();

    REQUIRE(q.getClauses().empty());
    REQUIRE(q.getSynonyms().size() == 2);
}

TEST_CASE("No clauses single select in bracket") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(","));
    declarationTokens.push_back(std::make_shared<Token>("b"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("<"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    selectTokens.push_back(std::make_shared<Token>(">"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();

    REQUIRE(q.getClauses().empty());
    REQUIRE(q.getSynonyms().size() == 2);
    QuerySelection s = q.getSelected();
    REQUIRE(s.getSelectedSynonyms().size() == 1);
}

TEST_CASE("No clauses multiple select") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(","));
    declarationTokens.push_back(std::make_shared<Token>("b"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("<"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    selectTokens.push_back(std::make_shared<Token>(","));
    selectTokens.push_back(std::make_shared<Token>("b"));
    selectTokens.push_back(std::make_shared<Token>(">"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();

    REQUIRE(q.getClauses().empty());
    REQUIRE(q.getSynonyms().size() == 2);
    QuerySelection s = q.getSelected();
    REQUIRE(s.getSelectedSynonyms().size() == 2);
}


TEST_CASE("UsesS clause, two synonyms") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("assign"));
    declarationTokens1.push_back(std::make_shared<Token>("a"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("variable"));
    declarationTokens2.push_back(std::make_shared<Token>("v"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("v"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Uses"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("a"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("v"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<UsesTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"a", "v"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
}

TEST_CASE("UsesS clause, variable synonym") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("variable"));
    declarationTokens.push_back(std::make_shared<Token>("v"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("v"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Uses"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("1"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("v"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<UsesTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
}

TEST_CASE("UsesS clause, variable wildcard") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Uses"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("a"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("_"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<UsesTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
}

TEST_CASE("UsesS clause, variable string") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Uses"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("a"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("\""));
    stTokens.push_back(std::make_shared<Token>("v"));
    stTokens.push_back(std::make_shared<Token>("\""));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<UsesTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"a"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("UsesP clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("procedure"));
    declarationTokens1.push_back(std::make_shared<Token>("p"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("variable"));
    declarationTokens2.push_back(std::make_shared<Token>("v"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("p"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Uses"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("p"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("v"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<UsesTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"p", "v"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("ModifiesS clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("stmt"));
    declarationTokens1.push_back(std::make_shared<Token>("s"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("variable"));
    declarationTokens2.push_back(std::make_shared<Token>("v"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("s"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Modifies"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("s"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("v"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<ModifiesTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"s", "v"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("ModifiesP clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("procedure"));
    declarationTokens1.push_back(std::make_shared<Token>("p"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("variable"));
    declarationTokens2.push_back(std::make_shared<Token>("v"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("p"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Modifies"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("p"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("v"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<ModifiesTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"p", "v"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Follows clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("print"));
    declarationTokens1.push_back(std::make_shared<Token>("p"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("read"));
    declarationTokens2.push_back(std::make_shared<Token>("r"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("p"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Follows"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("p"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("r"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<FollowsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"p", "r"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("FollowsStar clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("print"));
    declarationTokens1.push_back(std::make_shared<Token>("p"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("read"));
    declarationTokens2.push_back(std::make_shared<Token>("r"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("r"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Follows*"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("p"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("r"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<FollowsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"p", "r"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Parent clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("while"));
    declarationTokens1.push_back(std::make_shared<Token>("w"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("if"));
    declarationTokens2.push_back(std::make_shared<Token>("i"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("w"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Parent"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("w"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("i"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<ParentTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"w", "i"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("ParentStar clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("while"));
    declarationTokens1.push_back(std::make_shared<Token>("w"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("if"));
    declarationTokens2.push_back(std::make_shared<Token>("i"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("i"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Parent*"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("w"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("i"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<ParentTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"w", "i"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Calls clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("procedure"));
    declarationTokens.push_back(std::make_shared<Token>("p"));
    declarationTokens.push_back(std::make_shared<Token>(","));
    declarationTokens.push_back(std::make_shared<Token>("q"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));


    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("p"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Calls"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("p"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("q"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<CallsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"p", "q"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("CallsStar clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("procedure"));
    declarationTokens.push_back(std::make_shared<Token>("p"));
    declarationTokens.push_back(std::make_shared<Token>(","));
    declarationTokens.push_back(std::make_shared<Token>("q"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("p"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Calls*"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("p"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("q"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<CallsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"p", "q"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Pattern clause only wildcard") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> patternTokens;
    patternTokens.push_back(std::make_shared<Token>("pattern"));
    patternTokens.push_back(std::make_shared<Token>("a"));
    patternTokens.push_back(std::make_shared<Token>("("));
    patternTokens.push_back(std::make_shared<Token>("_"));
    patternTokens.push_back(std::make_shared<Token>(","));
    patternTokens.push_back(std::make_shared<Token>("_"));
    patternTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<PatternTokenGroup>(patternTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Pattern clause only string") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> patternTokens;
    patternTokens.push_back(std::make_shared<Token>("pattern"));
    patternTokens.push_back(std::make_shared<Token>("a"));
    patternTokens.push_back(std::make_shared<Token>("("));
    patternTokens.push_back(std::make_shared<Token>("\""));
    patternTokens.push_back(std::make_shared<Token>("v"));
    patternTokens.push_back(std::make_shared<Token>("\""));
    patternTokens.push_back(std::make_shared<Token>(","));
    patternTokens.push_back(std::make_shared<Token>("\""));
    patternTokens.push_back(std::make_shared<Token>("("));
    patternTokens.push_back(std::make_shared<Token>("a"));
    patternTokens.push_back(std::make_shared<Token>("+"));
    patternTokens.push_back(std::make_shared<Token>("b"));
    patternTokens.push_back(std::make_shared<Token>(")"));
    patternTokens.push_back(std::make_shared<Token>("*"));
    patternTokens.push_back(std::make_shared<Token>("c"));
    patternTokens.push_back(std::make_shared<Token>("\""));
    patternTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<PatternTokenGroup>(patternTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"a"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
}

TEST_CASE("Such that and pattern clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("assign"));
    declarationTokens1.push_back(std::make_shared<Token>("a"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("variable"));
    declarationTokens2.push_back(std::make_shared<Token>("v"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Uses"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("a"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("v"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<UsesTokenGroup>(stTokens));

    std::vector<std::shared_ptr<Token>> patternTokens;
    patternTokens.push_back(std::make_shared<Token>("pattern"));
    patternTokens.push_back(std::make_shared<Token>("a"));
    patternTokens.push_back(std::make_shared<Token>("("));
    patternTokens.push_back(std::make_shared<Token>("\""));
    patternTokens.push_back(std::make_shared<Token>("v"));
    patternTokens.push_back(std::make_shared<Token>("\""));
    patternTokens.push_back(std::make_shared<Token>(","));
    patternTokens.push_back(std::make_shared<Token>("_"));
    patternTokens.push_back(std::make_shared<Token>("\""));
    patternTokens.push_back(std::make_shared<Token>("("));
    patternTokens.push_back(std::make_shared<Token>("a"));
    patternTokens.push_back(std::make_shared<Token>("+"));
    patternTokens.push_back(std::make_shared<Token>("b"));
    patternTokens.push_back(std::make_shared<Token>(")"));
    patternTokens.push_back(std::make_shared<Token>("*"));
    patternTokens.push_back(std::make_shared<Token>("c"));
    patternTokens.push_back(std::make_shared<Token>("\""));
    patternTokens.push_back(std::make_shared<Token>("_"));
    patternTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<PatternTokenGroup>(patternTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto uses = q.getClauses()[1]->getSynonyms();
    auto pattern = q.getClauses()[0]->getSynonyms();
    REQUIRE(*uses == std::set<std::string>{"a"});
    REQUIRE(*pattern == std::set<std::string>{"a", "v"});

    REQUIRE(q.getClauses().size() == 2);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
    REQUIRE(!q.getClauses()[1]->isNegated);
}

TEST_CASE("not operator in Uses clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("assign"));
    declarationTokens1.push_back(std::make_shared<Token>("a"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("variable"));
    declarationTokens2.push_back(std::make_shared<Token>("v"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("v"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("not"));
    stTokens.push_back(std::make_shared<Token>("Uses"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("a"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("v"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<UsesTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"a", "v"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(q.getClauses()[0]->isNegated);
}

TEST_CASE("not operator in Follows clause"){
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("print"));
    declarationTokens1.push_back(std::make_shared<Token>("p"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("read"));
    declarationTokens2.push_back(std::make_shared<Token>("r"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("p"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("not"));
    stTokens.push_back(std::make_shared<Token>("Follows"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("p"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("r"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<FollowsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"p", "r"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(q.getClauses()[0]->isNegated);
}

TEST_CASE("not operator in Modifies clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("stmt"));
    declarationTokens1.push_back(std::make_shared<Token>("s"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("variable"));
    declarationTokens2.push_back(std::make_shared<Token>("v"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("s"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("not"));
    stTokens.push_back(std::make_shared<Token>("Modifies"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("s"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("v"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<ModifiesTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"s", "v"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(q.getClauses()[0]->isNegated);
}

TEST_CASE("not operator in Parent clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("while"));
    declarationTokens1.push_back(std::make_shared<Token>("w"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("if"));
    declarationTokens2.push_back(std::make_shared<Token>("i"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("w"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("not"));
    stTokens.push_back(std::make_shared<Token>("Parent"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("w"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("i"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<ParentTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"w", "i"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(q.getClauses()[0]->isNegated);
}

TEST_CASE("not operator in Calls clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("procedure"));
    declarationTokens.push_back(std::make_shared<Token>("p"));
    declarationTokens.push_back(std::make_shared<Token>(","));
    declarationTokens.push_back(std::make_shared<Token>("q"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));


    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("p"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("not"));
    stTokens.push_back(std::make_shared<Token>("Calls"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("p"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("q"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<CallsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"p", "q"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(q.getClauses()[0]->isNegated);
}

TEST_CASE("pattern if clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);
    tokenGroups.push_back(group1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);
    tokenGroups.push_back(group2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("if"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);
    tokenGroups.push_back(group3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("if"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);
    tokenGroups.push_back(group4);

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"if", "v"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
}

TEST_CASE("while pattern clause") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("while"));
    declarationTokens1.push_back(std::make_shared<Token>("w"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("w"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("pattern"));
    stTokens.push_back(std::make_shared<Token>("w"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("\""));
    stTokens.push_back(std::make_shared<Token>("x"));
    stTokens.push_back(std::make_shared<Token>("\""));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("_"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<PatternTokenGroup>(stTokens));

    std::string res =  QPSValidator{tokenGroups}.validate();
    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"w"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
}

TEST_CASE("Next clause extraction") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("stmt"));
    declarationTokens.push_back(std::make_shared<Token>("s"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("s"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Next"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("s"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("5"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<NextTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"s"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Next clause extraction both number") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("BOOLEAN"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Next"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("4"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("5"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<NextTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 0);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Next* clause extraction") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("stmt"));
    declarationTokens.push_back(std::make_shared<Token>("s"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("s"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Next*"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("s"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("5"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<NextTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"s"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
    REQUIRE(!q.getClauses()[0]->isNegated);
    REQUIRE(dynamic_cast<Next&>(*q.getClauses()[0]).hasStar);
}

TEST_CASE("Affects clause extraction") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Affects"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("4"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("a"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<AffectsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"a"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Affects clause extraction reverse synonym pos") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Affects"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("a"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("4"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<AffectsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"a"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Affects clause extraction two numbers") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("BOOLEAN"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Affects"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("4"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("5"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<AffectsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 0);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("Affects clause wildcard") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("assign"));
    declarationTokens.push_back(std::make_shared<Token>("a"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("a"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("such"));
    stTokens.push_back(std::make_shared<Token>("that"));
    stTokens.push_back(std::make_shared<Token>("Affects"));
    stTokens.push_back(std::make_shared<Token>("("));
    stTokens.push_back(std::make_shared<Token>("a"));
    stTokens.push_back(std::make_shared<Token>(","));
    stTokens.push_back(std::make_shared<Token>("_"));
    stTokens.push_back(std::make_shared<Token>(")"));
    tokenGroups.push_back(std::make_shared<AffectsTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"a"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("With clause extraction 2 constants") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("BOOLEAN"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("with"));
    stTokens.push_back(std::make_shared<Token>("1"));
    stTokens.push_back(std::make_shared<Token>("="));
    stTokens.push_back(std::make_shared<Token>("1"));
    tokenGroups.push_back(std::make_shared<WithTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().empty());
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("With clause extraction 2 constants and not") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("BOOLEAN"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("with"));
    stTokens.push_back(std::make_shared<Token>("not"));
    stTokens.push_back(std::make_shared<Token>("1"));
    stTokens.push_back(std::make_shared<Token>("="));
    stTokens.push_back(std::make_shared<Token>("1"));
    tokenGroups.push_back(std::make_shared<WithTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().empty());
    REQUIRE(q.getClauses()[0]->isNegated);
}

TEST_CASE("With clause extraction 1 constant 1 attribute") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens;
    declarationTokens.push_back(std::make_shared<Token>("read"));
    declarationTokens.push_back(std::make_shared<Token>("r"));
    declarationTokens.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("r"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("with"));
    stTokens.push_back(std::make_shared<Token>("r"));
    stTokens.push_back(std::make_shared<Token>("."));
    stTokens.push_back(std::make_shared<Token>("varName"));
    stTokens.push_back(std::make_shared<Token>("="));
    stTokens.push_back(std::make_shared<Token>("\""));
    stTokens.push_back(std::make_shared<Token>("var"));
    stTokens.push_back(std::make_shared<Token>("\""));
    tokenGroups.push_back(std::make_shared<WithTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"r"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 1);
    REQUIRE(!q.getClauses()[0]->isNegated);
}

TEST_CASE("With clause extraction 2 attributes") {
    std::vector<std::shared_ptr<TokenGroup>> tokenGroups;

    std::vector<std::shared_ptr<Token>> declarationTokens1;
    declarationTokens1.push_back(std::make_shared<Token>("call"));
    declarationTokens1.push_back(std::make_shared<Token>("c"));
    declarationTokens1.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens1));

    std::vector<std::shared_ptr<Token>> declarationTokens2;
    declarationTokens2.push_back(std::make_shared<Token>("procedure"));
    declarationTokens2.push_back(std::make_shared<Token>("p"));
    declarationTokens2.push_back(std::make_shared<Token>(";"));
    tokenGroups.push_back(std::make_shared<DeclarationTokenGroup>(declarationTokens2));

    std::vector<std::shared_ptr<Token>> selectTokens;
    selectTokens.push_back(std::make_shared<Token>("Select"));
    selectTokens.push_back(std::make_shared<Token>("c"));
    tokenGroups.push_back(std::make_shared<SelectTokenGroup>(selectTokens));

    std::vector<std::shared_ptr<Token>> stTokens;
    stTokens.push_back(std::make_shared<Token>("with"));
    stTokens.push_back(std::make_shared<Token>("c"));
    stTokens.push_back(std::make_shared<Token>("."));
    stTokens.push_back(std::make_shared<Token>("procName"));
    stTokens.push_back(std::make_shared<Token>("="));
    stTokens.push_back(std::make_shared<Token>("p"));
    stTokens.push_back(std::make_shared<Token>("."));
    stTokens.push_back(std::make_shared<Token>("procName"));
    tokenGroups.push_back(std::make_shared<WithTokenGroup>(stTokens));

    REQUIRE(QPSValidator{tokenGroups}.validate().empty()); // fills up the required fields in the token group
    QueryExtractor extractor = QueryExtractor(tokenGroups);
    Query q = extractor.extractQuery();
    auto clauses = q.getClauses()[0]->getSynonyms();
    REQUIRE(*clauses == std::set<std::string>{"c", "p"});

    REQUIRE(q.getClauses().size() == 1);
    REQUIRE(q.getSynonyms().size() == 2);
    REQUIRE(!q.getClauses()[0]->isNegated);
}