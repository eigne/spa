#include "TokenSplitter.h"

#include <utility>

std::vector<TokenList> TokenSplitter::splitIntoDeclarationsAndSelect(TokenList queryTokens) {
    auto it = queryTokens.begin();
    auto lastSemicolon = it;
    while (it != queryTokens.end()) {
        if ((*it)->getStringValue() == ";") {
            lastSemicolon = it;
        }
        it++;
    }

    TokenList declarations;
    TokenList select;
    if (lastSemicolon != queryTokens.end() && (*lastSemicolon)->getStringValue() != ";") {
        select = queryTokens;
    } else {
        declarations = {queryTokens.begin(), lastSemicolon + 1};
        select = {lastSemicolon + 1, queryTokens.end()};
    }

    return {declarations, select};
}

std::vector<DeclarationTokenGroup> TokenSplitter::splitDeclarations(const TokenList& declarationTokens) {
    std::vector<DeclarationTokenGroup> res;
    TokenList currentClause;
    for (const auto& token : declarationTokens) {
        if (token->getStringValue() == ";") {
            currentClause.push_back(token);
            res.emplace_back(currentClause);
            currentClause.clear();
        } else {
            currentClause.push_back(token);
        }
    }
    return res;
}

SelectTokenGroup TokenSplitter::getSelectTokens(TokenList selectAndClauseTokens) {
    if (selectAndClauseTokens.size() <= 1) {
        SelectTokenGroup selectTokenGroup(selectAndClauseTokens);
        return selectTokenGroup;
    }
    // Select clause is a tuple
    if (selectAndClauseTokens[1]->getStringValue() == "<") {
        auto it = selectAndClauseTokens.begin();
        auto triangleClosingBracket = it;
        while (it != selectAndClauseTokens.end()) {
            if ((*it)->getStringValue() == ">") {
                triangleClosingBracket = it;
                break;
            }
            it++;
        }
        TokenList select(selectAndClauseTokens.begin(), triangleClosingBracket + 1);
        SelectTokenGroup selectTokenGroup(select);
        return selectTokenGroup;
    } else if (selectAndClauseTokens.size() >= 4 && selectAndClauseTokens[2]->getStringValue() == ".") { //Select clause has attribute
        auto end = selectAndClauseTokens.begin() + 4;
        TokenList select(selectAndClauseTokens.begin(), end);
        SelectTokenGroup selectTokenGroup(select);
        return selectTokenGroup;
    } else { // select clause has no attribute
        TokenList select(selectAndClauseTokens.begin(), selectAndClauseTokens.begin() + 2);
        SelectTokenGroup selectTokenGroup(select);
        return selectTokenGroup;
    }
}

std::vector<std::shared_ptr<ClauseTokenGroup> > TokenSplitter::getClauseTokens(TokenList selectAndClauseTokens,
                                                                               int numberOfSelectTokens) {
    TokenList clauseTokens(selectAndClauseTokens.begin() + numberOfSelectTokens, selectAndClauseTokens.end());
    std::vector<std::shared_ptr<ClauseTokenGroup> > res;
    TokenList currentClause;

    bool inQuote = false;
    TokenGroupType currTokenGroupType = None;

    for (size_t i = 0; i < clauseTokens.size(); i++) {
        std::string currString = clauseTokens[i]->getStringValue();
        if (currString == "\"") { // handle quotation marks
            inQuote = !inQuote;
        }

        if (currentClause.empty()) { // handle start of clause
            if (currString == "such") {
                currTokenGroupType = SuchThat;
                currentClause.push_back(clauseTokens[i]);
            } else if (currString == "pattern") {
                currTokenGroupType = Pattern;
                currentClause.push_back(clauseTokens[i]);
            } else if (currString == "with") {
                currTokenGroupType = With;
                currentClause.push_back(clauseTokens[i]);
            } else if (currString == "and") {
                if (currTokenGroupType == None) {
                    return {std::make_shared<InvalidClauseTokenGroup>(currentClause)};
                } else if (currTokenGroupType == SuchThat) {
                    currTokenGroupType = SuchThat;
                    currentClause.push_back(suchToken);
                    currentClause.push_back(thatToken);
                } else if (currTokenGroupType == Pattern) {
                    currTokenGroupType = Pattern;
                    currentClause.push_back(patternToken);
                } else if (currTokenGroupType == With) {
                    currTokenGroupType = With;
                    currentClause.push_back(withToken);
                }
            } else {
                return {std::make_shared<InvalidClauseTokenGroup>(currentClause)};
            }
        } else { // the rest of the clause
            currentClause.push_back(clauseTokens[i]);
            // end of clause checks
            if (!inQuote && currString == ")" && (currTokenGroupType == SuchThat || currTokenGroupType == Pattern)) {
                if (currTokenGroupType == SuchThat) {
                    int relRefIdx = 2;
                    if (currentClause[2]->getStringValue() == "not") {
                        relRefIdx++;
                    }

                    if (currentClause[relRefIdx]->getStringValue() == "Calls" ||
                        currentClause[relRefIdx]->getStringValue() == "Calls*")
                        res.push_back(std::make_shared<CallsTokenGroup>(currentClause));
                    else if (currentClause[relRefIdx]->getStringValue() == "Follows" ||
                             currentClause[relRefIdx]->getStringValue() == "Follows*")
                        res.push_back(std::make_shared<FollowsTokenGroup>(currentClause));
                    else if (currentClause[relRefIdx]->getStringValue() == "Modifies")
                        res.push_back(std::make_shared<ModifiesTokenGroup>(currentClause));
                    else if (currentClause[relRefIdx]->getStringValue() == "Next" ||
                             currentClause[relRefIdx]->getStringValue() == "Next*")
                        res.push_back(std::make_shared<NextTokenGroup>(currentClause));
                    else if (currentClause[relRefIdx]->getStringValue() == "Parent" ||
                             currentClause[relRefIdx]->getStringValue() == "Parent*")
                        res.push_back(std::make_shared<ParentTokenGroup>(currentClause));
                    else if (currentClause[relRefIdx]->getStringValue() == "Uses")
                        res.push_back(std::make_shared<UsesTokenGroup>(currentClause));
                    else if (currentClause[relRefIdx]->getStringValue() == "Affects")
                        res.push_back(std::make_shared<AffectsTokenGroup>(currentClause));
                    else
                        return {std::make_shared<InvalidClauseTokenGroup>(currentClause)};
                } else if (currTokenGroupType == Pattern) {
                    res.push_back(std::make_shared<PatternTokenGroup>(currentClause));
                }
                currentClause.clear();
            } else if (!inQuote && currTokenGroupType == With) {
                if (clauseTokens[i]->getStringValue() != "." &&
                    (i + 2 >= clauseTokens.size() || clauseTokens[i + 2]->getStringValue() != ".")) {
                    if (i + 1 >= clauseTokens.size()) {
                        res.push_back(std::make_shared<WithTokenGroup>(currentClause));
                        currentClause.clear();
                    } else {
                        bool nextTokenIsAlphanumeric = true;
                        for (char c : clauseTokens[i + 1]->getStringValue()) {
                            if (isalpha(c) == 0) {
                                nextTokenIsAlphanumeric = false;
                            }
                        }
                        if (nextTokenIsAlphanumeric) {
                            // if next word is alphabets only, not surrounded by dots, or in quotes
                            res.push_back(std::make_shared<WithTokenGroup>(currentClause));
                            currentClause.clear();
                        }
                    }
                }
            }
        }
    }

    if (!currentClause.empty()) {
        return { std::make_shared<InvalidClauseTokenGroup>(currentClause) };
    }

    return res;
}

std::vector<std::shared_ptr<TokenGroup> > TokenSplitter::splitIntoTokenGroups(TokenList queryTokens) {
    auto declAndSelect = splitIntoDeclarationsAndSelect(std::move(queryTokens));
    auto declarationList = splitDeclarations(declAndSelect[0]);

    std::vector<std::shared_ptr<TokenGroup> > res;
    for (const auto& declaration : declarationList) {
        res.push_back(std::make_shared<DeclarationTokenGroup>(declaration));
    }

    auto selectAndClause = declAndSelect[1];

    if (selectAndClause.size() >= 2) {
        SelectTokenGroup selectStatement = getSelectTokens(selectAndClause);
        auto clauseList = getClauseTokens(selectAndClause, selectStatement.getSize());
        res.push_back(std::make_shared<SelectTokenGroup>(selectStatement));
        for (const auto& clause : clauseList) {
            res.push_back(clause);
        }
    } else {
        res.push_back(std::make_shared<SelectTokenGroup>(selectAndClause));
    }

    return res;
}
