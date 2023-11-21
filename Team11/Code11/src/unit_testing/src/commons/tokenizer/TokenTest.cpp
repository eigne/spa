#include "catch.hpp"
#include "commons/tokenizer/tokens/Token.h"

TEST_CASE("Tokens created correctly") {
    Token letterToken("a");
    Token numberToken("69");
    Token wordToken("kukubird");
    Token wordAndNumberAndSpecialCharacterToken("ilove3203!");

    REQUIRE(letterToken.getStringValue() == "a");
    REQUIRE(numberToken.getStringValue() == "69");
    REQUIRE(wordToken.getStringValue() == "kukubird");
    REQUIRE(wordAndNumberAndSpecialCharacterToken.getStringValue() == "ilove3203!");
}
