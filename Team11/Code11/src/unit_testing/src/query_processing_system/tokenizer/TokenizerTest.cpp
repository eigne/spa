#include "catch.hpp"
#include <sstream>
#include "query_processing_system/tokenizer/QueryTokenizer.h"
#include "query_processing_system/tokenizer/TokenSplitter.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/FollowsTokenGroup.h"

bool tokenGroupsAreEqual(const std::shared_ptr<TokenGroup>& list1, const std::shared_ptr<TokenGroup>& list2) {
    return list1->getStringifiedTokens() == list2->getStringifiedTokens();
}

TEST_CASE("No semicolon") {
    std::istringstream input("assign a Select");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedSelectStatement;
    expectedSelectStatement.push_back(std::make_shared<Token>("assign"));
    expectedSelectStatement.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelectStatement);

    TokenList expectedInvalidClause;
    std::shared_ptr<InvalidClauseTokenGroup> invalid = std::make_shared<InvalidClauseTokenGroup>(expectedInvalidClause);

    REQUIRE(tokenGroups.size() == 2);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], select));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], invalid));
}

TEST_CASE("Incomplete select clause") {
    std::istringstream input("assign a; Select");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration;
    expectedDeclaration.push_back(std::make_shared<Token>("assign"));
    expectedDeclaration.push_back(std::make_shared<Token>("a"));
    expectedDeclaration.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration = std::make_shared<DeclarationTokenGroup>(expectedDeclaration);

    TokenList expectedSelectStatement;
    expectedSelectStatement.push_back(std::make_shared<Token>("Select"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelectStatement);

    REQUIRE(tokenGroups.size() == 2);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], select));
}

TEST_CASE("No clause") {
    std::istringstream input("assign a; Select a");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration;
    expectedDeclaration.push_back(std::make_shared<Token>("assign"));
    expectedDeclaration.push_back(std::make_shared<Token>("a"));
    expectedDeclaration.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration = std::make_shared<DeclarationTokenGroup>(expectedDeclaration);

    TokenList expectedSelectStatement;
    expectedSelectStatement.push_back(std::make_shared<Token>("Select"));
    expectedSelectStatement.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelectStatement);

    REQUIRE(tokenGroups.size() == 2);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], select));
}

TEST_CASE("One clause") {
    std::istringstream input("assign a1, a2; Select a1 such that Follows*(a1, a2)");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration;
    expectedDeclaration.push_back(std::make_shared<Token>("assign"));
    expectedDeclaration.push_back(std::make_shared<Token>("a1"));
    expectedDeclaration.push_back(std::make_shared<Token>(","));
    expectedDeclaration.push_back(std::make_shared<Token>("a2"));
    expectedDeclaration.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration = std::make_shared<DeclarationTokenGroup>(expectedDeclaration);


    TokenList expectedSelectStatement;
    expectedSelectStatement.push_back(std::make_shared<Token>("Select"));
    expectedSelectStatement.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelectStatement);


    TokenList expectedClauseList;
    expectedClauseList.push_back(std::make_shared<Token>("such"));
    expectedClauseList.push_back(std::make_shared<Token>("that"));
    expectedClauseList.push_back(std::make_shared<Token>("Follows*"));
    expectedClauseList.push_back(std::make_shared<Token>("("));
    expectedClauseList.push_back(std::make_shared<Token>("a1"));
    expectedClauseList.push_back(std::make_shared<Token>(","));
    expectedClauseList.push_back(std::make_shared<Token>("a2"));
    expectedClauseList.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> st = std::make_shared<FollowsTokenGroup>(expectedClauseList);

    REQUIRE(tokenGroups.size() == 3);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], select));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[2], st));
}

TEST_CASE("Space between follows and star") {
    std::istringstream input("assign a1, a2; Select a1 such that Follows *(a1, a2)");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration;
    expectedDeclaration.push_back(std::make_shared<Token>("assign"));
    expectedDeclaration.push_back(std::make_shared<Token>("a1"));
    expectedDeclaration.push_back(std::make_shared<Token>(","));
    expectedDeclaration.push_back(std::make_shared<Token>("a2"));
    expectedDeclaration.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration = std::make_shared<DeclarationTokenGroup>(expectedDeclaration);


    TokenList expectedSelectStatement;
    expectedSelectStatement.push_back(std::make_shared<Token>("Select"));
    expectedSelectStatement.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelectStatement);


    TokenList expectedClauseList;
    expectedClauseList.push_back(std::make_shared<Token>("such"));
    expectedClauseList.push_back(std::make_shared<Token>("that"));
    expectedClauseList.push_back(std::make_shared<Token>("Follows"));
    expectedClauseList.push_back(std::make_shared<Token>("*"));
    expectedClauseList.push_back(std::make_shared<Token>("("));
    expectedClauseList.push_back(std::make_shared<Token>("a1"));
    expectedClauseList.push_back(std::make_shared<Token>(","));
    expectedClauseList.push_back(std::make_shared<Token>("a2"));
    expectedClauseList.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> st = std::make_shared<FollowsTokenGroup>(expectedClauseList);


    REQUIRE(tokenGroups.size() == 3);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], select));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[2], st));
}

TEST_CASE("Two clause") {
    std::istringstream input("assign a1, a2; variable v; Select a1 such that Follows(a1, a2) pattern a1(v, _\"(a + b) * c\"_)");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration1;
    expectedDeclaration1.push_back(std::make_shared<Token>("assign"));
    expectedDeclaration1.push_back(std::make_shared<Token>("a1"));
    expectedDeclaration1.push_back(std::make_shared<Token>(","));
    expectedDeclaration1.push_back(std::make_shared<Token>("a2"));
    expectedDeclaration1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration1 = std::make_shared<DeclarationTokenGroup>(expectedDeclaration1);

    TokenList expectedDeclaration2;
    expectedDeclaration2.push_back(std::make_shared<Token>("variable"));
    expectedDeclaration2.push_back(std::make_shared<Token>("v"));
    expectedDeclaration2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration2 = std::make_shared<DeclarationTokenGroup>(expectedDeclaration2);


    TokenList expectedSelectStatement;
    expectedSelectStatement.push_back(std::make_shared<Token>("Select"));
    expectedSelectStatement.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelectStatement);


    TokenList expectedClauseList1;
    expectedClauseList1.push_back(std::make_shared<Token>("such"));
    expectedClauseList1.push_back(std::make_shared<Token>("that"));
    expectedClauseList1.push_back(std::make_shared<Token>("Follows"));
    expectedClauseList1.push_back(std::make_shared<Token>("("));
    expectedClauseList1.push_back(std::make_shared<Token>("a1"));
    expectedClauseList1.push_back(std::make_shared<Token>(","));
    expectedClauseList1.push_back(std::make_shared<Token>("a2"));
    expectedClauseList1.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> st = std::make_shared<FollowsTokenGroup>(expectedClauseList1);


    TokenList expectedClauseList2;
    expectedClauseList2.push_back(std::make_shared<Token>("pattern"));
    expectedClauseList2.push_back(std::make_shared<Token>("a1"));
    expectedClauseList2.push_back(std::make_shared<Token>("("));
    expectedClauseList2.push_back(std::make_shared<Token>("v"));
    expectedClauseList2.push_back(std::make_shared<Token>(","));
    expectedClauseList2.push_back(std::make_shared<Token>("_"));
    expectedClauseList2.push_back(std::make_shared<Token>("\""));
    expectedClauseList2.push_back(std::make_shared<Token>("("));
    expectedClauseList2.push_back(std::make_shared<Token>("a"));
    expectedClauseList2.push_back(std::make_shared<Token>("+"));
    expectedClauseList2.push_back(std::make_shared<Token>("b"));
    expectedClauseList2.push_back(std::make_shared<Token>(")"));
    expectedClauseList2.push_back(std::make_shared<Token>("*"));
    expectedClauseList2.push_back(std::make_shared<Token>("c"));
    expectedClauseList2.push_back(std::make_shared<Token>("\""));
    expectedClauseList2.push_back(std::make_shared<Token>("_"));
    expectedClauseList2.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> pat = std::make_shared<PatternTokenGroup>(expectedClauseList2);


    REQUIRE(tokenGroups.size() == 5);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration1));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], declaration2));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[2], select));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[3], st));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[4], pat));
}

TEST_CASE("Valid select with brackets") {
    std::istringstream input("if ifs; while w; Select <w, ifs> such that Parent(w, ifs)");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration1;
    expectedDeclaration1.push_back(std::make_shared<Token>("if"));
    expectedDeclaration1.push_back(std::make_shared<Token>("ifs"));
    expectedDeclaration1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration1 = std::make_shared<DeclarationTokenGroup>(expectedDeclaration1);

    TokenList expectedDeclaration2;
    expectedDeclaration2.push_back(std::make_shared<Token>("while"));
    expectedDeclaration2.push_back(std::make_shared<Token>("w"));
    expectedDeclaration2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration2 = std::make_shared<DeclarationTokenGroup>(expectedDeclaration2);

    TokenList expectedSelect;
    expectedSelect.push_back(std::make_shared<Token>("Select"));
    expectedSelect.push_back(std::make_shared<Token>("<"));
    expectedSelect.push_back(std::make_shared<Token>("w"));
    expectedSelect.push_back(std::make_shared<Token>(","));
    expectedSelect.push_back(std::make_shared<Token>("ifs"));
    expectedSelect.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelect);

    TokenList expectedClauseList;
    expectedClauseList.push_back(std::make_shared<Token>("such"));
    expectedClauseList.push_back(std::make_shared<Token>("that"));
    expectedClauseList.push_back(std::make_shared<Token>("Parent"));
    expectedClauseList.push_back(std::make_shared<Token>("("));
    expectedClauseList.push_back(std::make_shared<Token>("w"));
    expectedClauseList.push_back(std::make_shared<Token>(","));
    expectedClauseList.push_back(std::make_shared<Token>("ifs"));
    expectedClauseList.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> st = std::make_shared<ParentTokenGroup>(expectedClauseList);

    REQUIRE(tokenGroups.size() == 4);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration1));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], declaration2));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[2], select));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[3], st));
}

TEST_CASE("Select brackets with no synonyms") {
    std::istringstream input("while w; Select <>");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration;
    expectedDeclaration.push_back(std::make_shared<Token>("while"));
    expectedDeclaration.push_back(std::make_shared<Token>("w"));
    expectedDeclaration.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration = std::make_shared<DeclarationTokenGroup>(expectedDeclaration);

    TokenList expectedSelect;
    expectedSelect.push_back(std::make_shared<Token>("Select"));
    expectedSelect.push_back(std::make_shared<Token>("<"));
    expectedSelect.push_back(std::make_shared<Token>(">"));    
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelect);

    REQUIRE(tokenGroups.size() == 2);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], select));
}

TEST_CASE("Select brackets with one synonym") {
    std::istringstream input("while w; Select <w>");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration;
    expectedDeclaration.push_back(std::make_shared<Token>("while"));
    expectedDeclaration.push_back(std::make_shared<Token>("w"));
    expectedDeclaration.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration = std::make_shared<DeclarationTokenGroup>(expectedDeclaration);

    TokenList expectedSelect;
    expectedSelect.push_back(std::make_shared<Token>("Select"));
    expectedSelect.push_back(std::make_shared<Token>("<"));
    expectedSelect.push_back(std::make_shared<Token>("w"));
    expectedSelect.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelect);

    REQUIRE(tokenGroups.size() == 2);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], select));
}

TEST_CASE("Select brackets with multiple synonym") {
    std::istringstream input("while w1, w2; Select <w1, w2>");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration;
    expectedDeclaration.push_back(std::make_shared<Token>("while"));
    expectedDeclaration.push_back(std::make_shared<Token>("w1"));
    expectedDeclaration.push_back(std::make_shared<Token>(","));
    expectedDeclaration.push_back(std::make_shared<Token>("w2"));
    expectedDeclaration.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration = std::make_shared<DeclarationTokenGroup>(expectedDeclaration);

    TokenList expectedSelect;
    expectedSelect.push_back(std::make_shared<Token>("Select"));
    expectedSelect.push_back(std::make_shared<Token>("<"));
    expectedSelect.push_back(std::make_shared<Token>("w1"));
    expectedSelect.push_back(std::make_shared<Token>(","));
    expectedSelect.push_back(std::make_shared<Token>("w2"));
    expectedSelect.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelect);

    REQUIRE(tokenGroups.size() == 2);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], select));
}

TEST_CASE("Select brackets with only closing brace") {
    std::istringstream input("print p; Select >");
    QueryTokenizer tokenizer(&input);
    TokenList tokenList = tokenizer.tokenize();
    TokenSplitter tokenSplitter;

    auto tokenGroups = tokenSplitter.splitIntoTokenGroups(tokenList);

    TokenList expectedDeclaration;
    expectedDeclaration.push_back(std::make_shared<Token>("print"));
    expectedDeclaration.push_back(std::make_shared<Token>("p"));
    expectedDeclaration.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> declaration = std::make_shared<DeclarationTokenGroup>(expectedDeclaration);

    TokenList expectedSelect;
    expectedSelect.push_back(std::make_shared<Token>("Select"));
    expectedSelect.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> select = std::make_shared<SelectTokenGroup>(expectedSelect);

    REQUIRE(tokenGroups.size() == 2);
    REQUIRE(tokenGroupsAreEqual(tokenGroups[0], declaration));
    REQUIRE(tokenGroupsAreEqual(tokenGroups[1], select));
}