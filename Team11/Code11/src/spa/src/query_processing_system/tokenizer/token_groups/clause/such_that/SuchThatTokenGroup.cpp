#include "SuchThatTokenGroup.h"

SuchThatTokenGroup::SuchThatTokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList) : ClauseTokenGroup(
        tokenList) {
}


void SuchThatTokenGroup::setAttributes() {
    arg1IsWildcard = arg1 == "_";
    arg1IsString = arg1[0] == '"';
    arg1IsNumber = isdigit(arg1[0]);
    arg1IsSynonym = !arg1IsWildcard && !arg1IsNumber && !arg1IsString;

    arg2IsWildcard = arg2 == "_";
    arg2IsString = arg2[0] == '"';
    arg2IsNumber = isdigit(arg2[0]);
    arg2IsSynonym = !arg2IsWildcard && !arg2IsNumber && !arg2IsString;
}
