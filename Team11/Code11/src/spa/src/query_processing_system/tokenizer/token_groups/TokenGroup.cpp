#include "TokenGroup.h"

TokenGroup::TokenGroup(const std::vector<std::shared_ptr<Token> >& tokenList) {
    this->tokenList = tokenList;
}

int TokenGroup::getSize() {
    return this->tokenList.size();
}

std::vector<std::string> TokenGroup::getStringifiedTokens() {
    std::vector<std::string> res;
    for (const auto& token : tokenList) {
        res.push_back(token->getStringValue());
    }
    return res;
}
