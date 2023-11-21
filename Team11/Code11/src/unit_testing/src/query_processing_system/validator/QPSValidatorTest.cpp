#include "catch.hpp"
#include "query_processing_system/validator/QPSValidator.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/UsesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ModifiesTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/FollowsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/ParentTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/CallsTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/NextTokenGroup.h"
#include "query_processing_system/tokenizer/token_groups/clause/such_that/AffectsTokenGroup.h"
#include "commons/tokenizer/Tokenizer.h"

// General Tests FOR validate()

TEST_CASE("ERROR: validate() Wrong Design Entity given") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("var"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    QPSValidator validator = QPSValidator({group1, group2});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() First word in Select statement is not 'Select'") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("v2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Hello"));
    tokens2.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    QPSValidator validator = QPSValidator({group1, group2});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Synonym should be in declaration") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("v2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("hello"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    QPSValidator validator = QPSValidator({group1, group2});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Not suchthat-cl or pattern-cl") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("v2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("suchthat"));
    std::shared_ptr<InvalidClauseTokenGroup> group3 = std::make_shared<InvalidClauseTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Empty Token list") {
    QPSValidator validator = QPSValidator({});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() No declarations") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("Select"));
    tokens1.push_back(std::make_shared<Token>("s"));
    std::shared_ptr<SelectTokenGroup> group1 = std::make_shared<SelectTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("such"));
    tokens2.push_back(std::make_shared<Token>("that"));
    tokens2.push_back(std::make_shared<Token>("Follows"));
    tokens2.push_back(std::make_shared<Token>("("));
    tokens2.push_back(std::make_shared<Token>("_"));
    tokens2.push_back(std::make_shared<Token>(","));
    tokens2.push_back(std::make_shared<Token>("_"));
    tokens2.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group2 = std::make_shared<FollowsTokenGroup>(tokens2);

    QPSValidator validator = QPSValidator({group1, group2});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() No Closing Brackets ") {

    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() No Opening Brackets") {

    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Additional Closing Brackets") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() No semi colon") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("assign"));
    tokens3.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<DeclarationTokenGroup> group3 = std::make_shared<DeclarationTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("Select"));
    tokens4.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group4 = std::make_shared<SelectTokenGroup>(tokens4);

    std::vector<std::shared_ptr<Token>> tokens5;
    tokens5.push_back(std::make_shared<Token>("such"));
    tokens5.push_back(std::make_shared<Token>("that"));
    tokens5.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens5.push_back(std::make_shared<Token>("a1"));
    tokens5.push_back(std::make_shared<Token>(","));
    tokens5.push_back(std::make_shared<Token>("v"));
    tokens5.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group5 = std::make_shared<UsesTokenGroup>(tokens5);

    QPSValidator validator = QPSValidator({group1, group3, group4, group5});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Additional semi colon") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("assign"));
    tokens3.push_back(std::make_shared<Token>("a1"));
    tokens3.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group3 = std::make_shared<DeclarationTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("Select"));
    tokens4.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group4 = std::make_shared<SelectTokenGroup>(tokens4);

    std::vector<std::shared_ptr<Token>> tokens5;
    tokens5.push_back(std::make_shared<Token>("such"));
    tokens5.push_back(std::make_shared<Token>("that"));
    tokens5.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens5.push_back(std::make_shared<Token>("a1"));
    tokens5.push_back(std::make_shared<Token>(","));
    tokens5.push_back(std::make_shared<Token>("v"));
    tokens5.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group5 = std::make_shared<UsesTokenGroup>(tokens5);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4, group5});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Invalid Declarations first char in IDENT isdigit") {

    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("1431by"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    QPSValidator validator = QPSValidator({group1, group2});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Invalid Declarations special char in INDENT") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("y12?"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    QPSValidator validator = QPSValidator({group1, group2});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Invalid Declarations repeated synonyms") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("a1a"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a1a"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    QPSValidator validator = QPSValidator({group1, group2});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");

}

TEST_CASE("ERROR: ERROR: validate() 'such that' 'that' not in query") {

    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: ERROR: validate() 'such that' 'that' not correct") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("thatt"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: ERROR: validate() wrong char to separate 2 ref in Uses clause") {

    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>("/"));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() \"Testing\" is not a relRef") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Testing"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Usage of undeclared variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("such"));
    tokens3.push_back(std::make_shared<Token>("that"));
    tokens3.push_back(std::make_shared<Token>("Modifies"));
    tokens3.push_back(std::make_shared<Token>("("));
    tokens3.push_back(std::make_shared<Token>("a1"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("v"));
    tokens3.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group3 = std::make_shared<ModifiesTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}


// Modifies Test

TEST_CASE("ERROR: validate() Modifies, second arg not a variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Modifies"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ModifiesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Modifies, first argument cannot be a wildcard") {

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Modifies"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ModifiesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("SUCCESS: validate() Modifies successful arg1 assign, arg2 variable ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Modifies"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ModifiesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}


// Uses Tests

TEST_CASE("ERROR: validate() Uses, second arg not a variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Uses arg1 variable, arg2 print") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("SUCCESS: validate() Uses successful arg1 assign, arg2 variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Uses successful arg1 print, arg2 variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

// Parents Test

TEST_CASE("SUCCESS: validate() Parents successful arg1 stmt, arg2 stmt ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("stmt"));
    tokens1.push_back(std::make_shared<Token>("s1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("s2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Parent"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ParentTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Parents successful arg1 print, arg2 stmt ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Parent"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ParentTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR: validate() Parents failed arg1 cannot be a variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Parent"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ParentTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Parents failed arg1 and arg2 cannot be a variable ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("v2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Parent"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ParentTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Parents failed arg2 cannot be a constant ") {

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Parent"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("hello"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ParentTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}


TEST_CASE("ERROR: validate() Parents failed arg1 and arg2 cannot be a constant ") {

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Parent"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("this"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("hello"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ParentTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

// Follows Test

TEST_CASE("SUCCESS: validate() Follows successful arg1 stmt, arg2 stmt ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("stmt"));
    tokens1.push_back(std::make_shared<Token>("s1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("s2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Follows"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<FollowsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Follows* successful arg1 stmt, arg2 stmt ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("stmt"));
    tokens1.push_back(std::make_shared<Token>("s1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("s2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Follows*"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<FollowsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}


TEST_CASE("SUCCESS: validate() Follows successful arg1 print, arg2 stmt ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Follows"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<FollowsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR: validate() Follows failed arg1 cannot be a variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Follows"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<FollowsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Follows failed arg1 and arg2 cannot be a variable ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("v2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Follows"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<FollowsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Follows failed arg2 cannot be a constant ") {

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Follows"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("hello"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<FollowsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Follows failed arg1 and arg2 cannot be a constant ") {
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Follows"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("this"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("hello"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<FollowsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

// Pattern

TEST_CASE("SUCCESS: Pattern 1 Simple pattern") {
    // variable v; assign a; Select a pattern a(v, "x + y")
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Pattern 2 Simple pattern with 1 bracket") {
    // variable v; assign a; Select a pattern a(v, "c + ( x + y )")
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("c"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>(")"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Pattern 3 Nested Brackets") {
    // variable v; assign a; Select a pattern a(v, "c + ( x + ( y + z ) )")
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("c"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("z"));
    tokens4.push_back(std::make_shared<Token>(")"));
    tokens4.push_back(std::make_shared<Token>(")"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Pattern 4 Consecutive Brackets") {
    // variable v; assign a; Select a pattern a(v, "( c + x ) - ( y + z ) ")
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("c"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>(")"));
    tokens4.push_back(std::make_shared<Token>("-"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("z"));
    tokens4.push_back(std::make_shared<Token>(")"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Pattern 5 Consecutive Brackets with nested Brackets with constant values") {
    // variable v; assign a; Select a pattern a(v, "( c + ( x * 1 ) ) + ( y % 2 ) ")
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("c"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("*"));
    tokens4.push_back(std::make_shared<Token>("1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    tokens4.push_back(std::make_shared<Token>(")"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>("%"));
    tokens4.push_back(std::make_shared<Token>("2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Pattern 6 Arg1 Wildcard, Arg2 name + const_value") {
    // assign a; Select a pattern a ( _ , "count + 1")
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("count"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("1"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Pattern 7 Arg2 _\"expr\"_  ") {
    // assign a; Select a pattern a ( "normSq" , _"cenX * cenX"_)
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("normSq"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("cenX"));
    tokens4.push_back(std::make_shared<Token>("*"));
    tokens4.push_back(std::make_shared<Token>("cenX"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Pattern 8 arg1 string arg2 wildcard") {
    // assign a; Select a pattern a ("normSq", _)
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("normSq"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR:  Consecutive operators + + ") {
    // assign a; Select a pattern a ("x", "x + + y")
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: Consecutive operators + *") {
    // assign a; Select a pattern a ("x", "x + * y")
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("*"));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: Arg2 \"expr\"_  is not a correct expression-spec") {
    // assign a; Select a pattern a ( "normSq" , "cenX * cenX"_)
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("normSq"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("cenX"));
    tokens4.push_back(std::make_shared<Token>("*"));
    tokens4.push_back(std::make_shared<Token>("cenX"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: Arg2 _expr_  is not a correct expression-spec") {
    // assign a; Select a pattern a ( "normSq" , _cenX * cenX_)
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("normSq"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>("cenX"));
    tokens4.push_back(std::make_shared<Token>("*"));
    tokens4.push_back(std::make_shared<Token>("cenX"));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: Invalid operator") {
    // variable v; assign a; Select a pattern a(v, "x ^ y")
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("^"));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}


// Multi select clause

TEST_CASE("SUCCESS: BOOLEAN Select clause") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("BOOLEAN"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Multi Select clause, 1 element tuple") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("v"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Multi Select clause, 1 element tuple constant.value") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("constant"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("v"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("value"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Multi Select clause, 2 element tuple") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("v"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("a1"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Multi Select clause, 3 element tuple") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("v"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("a1"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("a2"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Multi Select clause, attrName ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("v"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("varName"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Multi Select clause, 2 tuples attrName, read.varname assign.stmt#") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("read"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("varName"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("a1"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("stmt#"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Multi Select clause, 2 tuples attrName, read.varname while.stmt#") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("while"));
    tokens1.push_back(std::make_shared<Token>("while"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("read"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("varName"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("while"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("stmt#"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Multi Select clause, 2 tuples BOOLEAN is the synonym name for assign, read.varname assign.stmt#") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("BOOLEAN"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("read"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("varName"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("BOOLEAN"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("stmt#"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: Multi Select clause, BOOLEAN is the synonym name for assign") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("BOOLEAN"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    // This is no longer a BOOLEAN but rather as assign selection
    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("BOOLEAN"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR: Multi Select clause, 2 tuples attrName, read.procName is not valid ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("BOOLEAN"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("read"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("procName"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("BOOLEAN"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("stmt#"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: Multi Select clause, 2 tuples attrName, ass.procName is not valid ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("ass"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("read"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("varName"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("ass"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("procName"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: Multi Select clause, 2 tuples attrName, invalid attrName") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("ass"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("<"));
    tokens3.push_back(std::make_shared<Token>("read"));
    tokens3.push_back(std::make_shared<Token>("."));
    tokens3.push_back(std::make_shared<Token>("HELLOTHERE"));
    tokens3.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

// Calls Such that Clause

TEST_CASE("SUCCESS: validate() Calls successful arg1 procedure, arg2 procedure ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("procedure"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Calls successful arg1 procedure, arg2 constant ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("Third"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Calls successful arg1 wildcard, arg2 constant") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("First"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Calls* successful arg1 constant, arg2 procedure") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls*"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("Third"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Calls* successful arg1 constant, arg2 wildcard") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls*"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("First"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR: validate() Calls failed arg1 cannot be a variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("procedure"));
    tokens2.push_back(std::make_shared<Token>("p"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Calls failed arg2 cannot be a variable ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Calls failed arg2 cannot be a print stmt") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("print"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Calls* failed arg2 cannot be a read stmt") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("read"));
    tokens1.push_back(std::make_shared<Token>("read"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls*"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("read"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Calls failed arg1 cannot be invalid constant, naming convention is wrong") {

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Calls"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("this"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("2143HELLOOOsjdbf"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("SUCCESS: Single BOOLEAN synonym in tuple, BOOLEAN is the synonym name for assign") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("BOOLEAN"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    // This is no longer a BOOLEAN but rather as assign selection
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("<"));
    tokens2.push_back(std::make_shared<Token>("BOOLEAN"));
    tokens2.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    QPSValidator validator = QPSValidator({group1, group2});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR: Single BOOLEAN synonym in tuple, BOOLEAN is not declared") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    // This is no longer a BOOLEAN but rather as assign selection
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("<"));
    tokens2.push_back(std::make_shared<Token>("BOOLEAN"));
    tokens2.push_back(std::make_shared<Token>(">"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    QPSValidator validator = QPSValidator({group1, group2});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: * and Calls are in different tokens") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("stmt"));
    tokens1.push_back(std::make_shared<Token>("a"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("b"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    // This is no longer a BOOLEAN but rather as assign selection
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("Select"));
    tokens2.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group2 = std::make_shared<SelectTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("such"));
    tokens3.push_back(std::make_shared<Token>("that"));
    tokens3.push_back(std::make_shared<Token>("Calls"));
    tokens3.push_back(std::make_shared<Token>("*"));
    tokens3.push_back(std::make_shared<Token>("("));
    tokens3.push_back(std::make_shared<Token>("a"));
    tokens3.push_back(std::make_shared<Token>(","));
    tokens3.push_back(std::make_shared<Token>("b"));
    tokens3.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group3 = std::make_shared<CallsTokenGroup>(tokens3);

    QPSValidator validator = QPSValidator({group1, group2, group3});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

// Next Such that Clauses

TEST_CASE("SUCCESS: validate() Next successful arg1 stmt, arg2 stmt ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("stmt"));
    tokens1.push_back(std::make_shared<Token>("s1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("s2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Next"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<NextTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Next* successful arg1 stmt, arg2 stmt ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("stmt"));
    tokens1.push_back(std::make_shared<Token>("s1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("s2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Next*"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<NextTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Next BOOLEAN successful arg1 print, arg2 stmt ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>("r"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("BOOLEAN"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Next"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("r"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<NextTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Next successful arg1 print, arg2 stmt ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Next"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<NextTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR: validate() Next failed arg1 cannot be a variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Next"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<NextTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Next failed arg1 and arg2 cannot be a variable ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("v2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Next"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<NextTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Next failed arg2 cannot be a constant ") {

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Next"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("hello"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<NextTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Next failed arg1 and arg2 cannot be a constant ") {
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Next"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("this"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("hello"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<NextTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

// Affects Such that clauses

TEST_CASE("SUCCESS: validate() Affects successful arg1 assignment, arg2 assignment") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("a2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Affects successful arg1 and arg 2 are statement numbers ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("khf"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("khf"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("3"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Affects BOOLEAN successful arg1 assign, arg2 assign") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("BOOLEAN"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("a2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Affects successful arg1 and arg2 are wildcards") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("assign1"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("assignTWO"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("assign1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: validate() Affects successful arg1 is wildcard and arg2 is assign stmt") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("assign1"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("assignTWO"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("assign1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("assignTWO"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR: validate() Affects failed arg1 cannot be a variable") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Affects failed arg1 cannot be stmt and arg2 cannot be a variable ") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v1"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s2"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: validate() Affects failed arg2 cannot be a constant ") {

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("hello"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: validate() Affects failed arg1 and arg2 cannot be a constant ") {
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("this"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("hello"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

// Not operator in Clauses

TEST_CASE("SUCCESS: not operator in Modifies clause") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("a2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("v"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("not"));
    tokens4.push_back(std::make_shared<Token>("Modifies"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ModifiesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(group4->isNegated);
}

TEST_CASE("SUCCESS: not operator in Uses clause") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("not"));
    tokens4.push_back(std::make_shared<Token>("Uses"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<UsesTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(group4->isNegated);
}

TEST_CASE("SUCCESS: not operator in Follow clause") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("stmt"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("not"));
    tokens4.push_back(std::make_shared<Token>("Follows"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<FollowsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(group4->isNegated);
}

TEST_CASE("SUCCESS: not operator in Parent clause") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("stmt"));
    tokens1.push_back(std::make_shared<Token>("s1"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("s2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("not"));
    tokens4.push_back(std::make_shared<Token>("Parent"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<ParentTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(group4->isNegated);
}

TEST_CASE("SUCCESS: not operator in Call clause") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("procedure"));
    tokens2.push_back(std::make_shared<Token>("s1"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("not"));
    tokens4.push_back(std::make_shared<Token>("Calls"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("s1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<CallsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(group4->isNegated);
}

TEST_CASE("SUCCESS: not operator in Next clause") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("stmt"));
    tokens1.push_back(std::make_shared<Token>("s"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("read"));
    tokens2.push_back(std::make_shared<Token>("reading"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("s"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("not"));
    tokens4.push_back(std::make_shared<Token>("Next"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("s"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("reading"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<NextTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(group4->isNegated);
}

TEST_CASE("SUCCESS: not operator in Affects clause") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("assign"));
    tokens1.push_back(std::make_shared<Token>("a1"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("assign2"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a1"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("such"));
    tokens4.push_back(std::make_shared<Token>("that"));
    tokens4.push_back(std::make_shared<Token>("not"));
    tokens4.push_back(std::make_shared<Token>("Affects"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("a1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("assign2"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<SuchThatTokenGroup> group4 = std::make_shared<AffectsTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(group4->isNegated);
}

TEST_CASE("SUCCESS: not operator in pattern clause") {
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("not"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(group4->isNegated);
}

// With Clauses

TEST_CASE("SUCCESS: With Clause procName = varName") {
    // procedure p; variable v;
    //`Select p with p.procName = v.varName
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("procName"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("varName"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: With Clause procName = procName") {
    // procedure p, assign2;
    //`Select p with p.procName = v.varName
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("assign2"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("procName"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("assign2"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("procName"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: With Clause \"string\" = \"string\"") {
    // procedure p; variable v;
    //`Select p with "Hello" = "Hello"
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("nani"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("sheesh"));
    tokens4.push_back(std::make_shared<Token>("\""));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: With Clause procName = \"string\"") {
    // procedure p;
    //`Select p with p.procName = "Hello"
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("procName"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("Hello"));
    tokens4.push_back(std::make_shared<Token>("\""));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: With Clause stmt# = stmt#") {
    // print p; call calls;
    //`Select p with p.stmt# = calls.stmt#
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("call"));
    tokens2.push_back(std::make_shared<Token>("calls"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("stmt#"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("calls"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("stmt#"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: With Clause stmt# = INTEGER") {
    // print p; call calls;
    //`Select p with p.stmt# = 24
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("call"));
    tokens2.push_back(std::make_shared<Token>("calls"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("stmt#"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("24"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: With Clause value = INTEGER") {
    // print p; constant 24;
    //`Select p with p.stmt# = 24
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("constant"));
    tokens2.push_back(std::make_shared<Token>("calls"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("stmt#"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("24"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: With Clause stmt# = constant.value") {
    // print p; call calls;
    //`Select p with p.procName = 24
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("print"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("constant"));
    tokens2.push_back(std::make_shared<Token>("c"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("stmt#"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("c"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("value"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR: With Clause Only Synonym is given without attrName") {
    // procedure p; variable v;
    //`Select p with p = v.varName
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("varName"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: With Clause variable.stmt# is not allowed") {
    // procedure p;
    //`Select p with p.stmt# = variable.stmt#
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("stmt#"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("stmt#"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: With Clause the 2 ref types dont match") {
    // procedure p;
    //`Select p with p.stmt# = variable.varName
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("stmt#"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("varName"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: With Clause the 2 ref types dont match same synonym used") {
    // procedure p;
    //`Select p with p.stmt# = variable.varName
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("procedure"));
    tokens1.push_back(std::make_shared<Token>("p"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("variable"));
    tokens2.push_back(std::make_shared<Token>("v"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("p"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("with"));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("stmt#"));
    tokens4.push_back(std::make_shared<Token>("="));
    tokens4.push_back(std::make_shared<Token>("p"));
    tokens4.push_back(std::make_shared<Token>("."));
    tokens4.push_back(std::make_shared<Token>("procName"));
    std::shared_ptr<WithTokenGroup> group4 = std::make_shared<WithTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

// if pattern

TEST_CASE("SUCCESS: If Pattern 1 has 3 expressions, all wildcard") {
    // variable v; assign a; Select a pattern if(v_, _ ,_)
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("if"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("if"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("SUCCESS: If Pattern 1 has 3 expressions, first is variable, last 2 wildcard") {
    // variable v; assign a; Select a pattern if(v, _, _)
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("if"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

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

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("ERROR: If Pattern 1 has 3 expressions, first is variable, second is variable, third is wildcard") {
    // variable v, v1; assign a; Select a pattern if(v, v1 , _)
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("v1"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("if"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("if"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v1"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: If Pattern 1 has 3 expressions, first is variable, second is expression, third is wildcard") {
    // variable v, v1; assign a; Select a pattern if(v, "v + v1" , _)
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("v1"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("if"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("if"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("v1"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("ERROR: If Pattern 1 has 3 expressions, first is variable, second is expression, third is variable") {
    // variable v, v1; assign a; Select a pattern if(v, "v + v1" , v1)
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(","));
    tokens1.push_back(std::make_shared<Token>("v1"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("if"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("if"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("v1"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("v1"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

TEST_CASE("Error: If Pattern has 2 arguments") {
    // variable v; if if; Select a pattern a (_, _)
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>("if"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("if"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("if"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

// while pattern

TEST_CASE("SUCCESS: While Pattern arg1 string arg2 wildcard") {
    // while a; Select a pattern a ("normSq", _)
    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("while"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("normSq"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "");
}

TEST_CASE("Error: While Pattern arg1 string arg2 string") {
    // variable v; while a; Select a pattern a ("normSq", "v")
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("while"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(","));
    tokens2.push_back(std::make_shared<Token>("while"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("normSq"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("Error: While Pattern arg1 string arg2 _\"string\"_") {
    // variable v; while a; Select a pattern a ("normSq", _"v"_)
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("while"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(","));
    tokens2.push_back(std::make_shared<Token>("while"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("normSq"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("Error: While Pattern has 3 arguments") {
    // variable v; while a; Select a pattern a (_, _, _)
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("while"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(","));
    tokens2.push_back(std::make_shared<Token>("while"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SemanticError");
}

TEST_CASE("ERROR: Assign Pattern 1 has 3 expressions") {
    // variable v; assign a; Select a pattern a(v, "x + y", _)
    std::vector<std::shared_ptr<Token>> tokens1;
    tokens1.push_back(std::make_shared<Token>("variable"));
    tokens1.push_back(std::make_shared<Token>("v"));
    tokens1.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group1 = std::make_shared<DeclarationTokenGroup>(tokens1);

    std::vector<std::shared_ptr<Token>> tokens2;
    tokens2.push_back(std::make_shared<Token>("assign"));
    tokens2.push_back(std::make_shared<Token>("a"));
    tokens2.push_back(std::make_shared<Token>(";"));
    std::shared_ptr<DeclarationTokenGroup> group2 = std::make_shared<DeclarationTokenGroup>(tokens2);

    std::vector<std::shared_ptr<Token>> tokens3;
    tokens3.push_back(std::make_shared<Token>("Select"));
    tokens3.push_back(std::make_shared<Token>("a"));
    std::shared_ptr<SelectTokenGroup> group3 = std::make_shared<SelectTokenGroup>(tokens3);

    std::vector<std::shared_ptr<Token>> tokens4;
    tokens4.push_back(std::make_shared<Token>("pattern"));
    tokens4.push_back(std::make_shared<Token>("a"));
    tokens4.push_back(std::make_shared<Token>("("));
    tokens4.push_back(std::make_shared<Token>("v"));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>("x"));
    tokens4.push_back(std::make_shared<Token>("+"));
    tokens4.push_back(std::make_shared<Token>("y"));
    tokens4.push_back(std::make_shared<Token>("\""));
    tokens4.push_back(std::make_shared<Token>(","));
    tokens4.push_back(std::make_shared<Token>("_"));
    tokens4.push_back(std::make_shared<Token>(")"));
    std::shared_ptr<PatternTokenGroup> group4 = std::make_shared<PatternTokenGroup>(tokens4);

    QPSValidator validator = QPSValidator({group1, group2, group3, group4});
    std::string res = validator.validate();

    REQUIRE(res == "SyntaxError");
}

