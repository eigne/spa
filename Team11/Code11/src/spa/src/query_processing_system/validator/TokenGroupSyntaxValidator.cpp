#include "TokenGroupSyntaxValidator.h"

// tries to advance the iterator, and returns whether the iterator can be safely dereferenced
bool TokenGroupSyntaxValidator::safelyAdvanceIterator(std::vector<std::string>::iterator &it,
                                                      std::vector<std::string>::iterator &end) {
    if (it == end) {
        return false;
    }
    it++;
    return it != end;
}

bool TokenGroupSyntaxValidator::readArgument(std::vector<std::string>::iterator& it,
                                             std::vector<std::string>::iterator &end, std::string& res) {
    res = "";
    res += *it;
    if (!safelyAdvanceIterator(it, end)) {
        return false;
    }

    if (res == "\"") {
        res += *it;
        if (!safelyAdvanceIterator(it, end)) {
            return false;
        }
        res += *it;
        if (!safelyAdvanceIterator(it, end)) {
            return false;
        }
    }
    return true;
}

bool TokenGroupSyntaxValidator::validateQuotationIdent(const std::string& ref) {
    if (ref[0] != '"' || ref[ref.size() - 1] != '"') {
        return false;
    }
    return validateName(ref.substr(1, ref.size() - 2));
}

// any statement
bool TokenGroupSyntaxValidator::validateStatementRefs(const std::string &ref) {
    return ref == "_" || validateInteger(ref) || validateName(ref);
}

bool TokenGroupSyntaxValidator::validateEntityRefs(const std::string& ref) {
    return ref == "_" || validateQuotationIdent(ref) || validateName(ref);
}

bool TokenGroupSyntaxValidator::validateUsesModifiesRef1(const std::string &refOne) {
    return validateStatementRefs(refOne) || validateEntityRefs(refOne);
}

bool TokenGroupSyntaxValidator::validateUsesAndModifies(const std::string& refOne, const std::string& refTwo) {
    if (!validateUsesModifiesRef1(refOne)) {
        return false;
    }
    if (!validateEntityRefs(refTwo)) {
        return false;
    }
    return true;
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<DeclarationTokenGroup>& declarationTokenGroup) {
    auto tokens = declarationTokenGroup->getStringifiedTokens();
    // type name; minimally
    if (tokens.size() < 3) {
        return false;
    }
    std::string declarationType = tokens[0];
    if (designEntityList.find(declarationType) == designEntityList.end()) {
        return false;
    }

    std::vector<std::string> synonymNames;
    for (size_t i = 1; i < tokens.size() - 1; i++) {
        if (i % 2 == 0) {
            if (tokens[i] != ",") {
                return false;
            }
        } else {
            if (!validateName(tokens[i])) {
                return false;
            }
            if (tokens[i] == "BOOLEAN") {
                hasBOOLEAN = true;
            }
            synonymNames.push_back(tokens[i]);
        }
    }

    declarationTokenGroup->type = declarationType;
    declarationTokenGroup->synonyms = synonymNames;

    return tokens[tokens.size() - 1] == ";";
}

bool TokenGroupSyntaxValidator::validateSynonymAttribute(
    std::vector<std::string>::iterator &it,
    std::vector<std::string>::iterator &end,
    std::string& attrName
    ) {
    if (*it != "." || !safelyAdvanceIterator(it, end)) {
        return false;
    }

    if (*it == "varName" || *it == "procName" || *it == "value" || *it == "stmt#") {
        attrName = *it;
    } else {
        return false;
    }

   safelyAdvanceIterator(it, end);

    return true;
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<SelectTokenGroup>& selectTokenGroup) {
    auto tokens = selectTokenGroup->getStringifiedTokens();
    std::vector<std::string> synonymsInSelectClause;
    std::vector<std::pair<std::string, std::string>> synonymAttrNameVector;
    // Select ele -> minimally
    if (tokens.size() < 2) {
        return false;
    }

    if (tokens[0] != "Select") {
        return false;
    }

    auto it = tokens.begin() + 1;
    auto end = tokens.end();
    std::string attrName;

    std::string secondString = *it;
    if (secondString == "<") {
        // <a, b.procName, c.stmt#, d.value>
        if (!safelyAdvanceIterator(it, end)) {
            return false;
        }

        while (it != end) {
            std::string currSynonym = *it; // synonym
            if (!validateName(currSynonym) || !safelyAdvanceIterator(it, end)) {
                return false;
            }

            // Just a Synonym
            if (*it == "," || *it == ">") {
                synonymsInSelectClause.push_back(currSynonym);
                synonymAttrNameVector.emplace_back(currSynonym, "");
                it++;
                continue;
            }

            if (!validateSynonymAttribute(it, end, attrName)) {
                return false;
            }

            if (*it == "," || *it == ">") {
                synonymsInSelectClause.push_back(currSynonym);
                synonymAttrNameVector.emplace_back(currSynonym, attrName);
                it++;
            }
        }
    } else { // Just Synonym
        std::string currSynonym = *it;

        if (!validateName(currSynonym)) {
            return false;
        }

        it++;
        if (it == end) {
            if (currSynonym != "BOOLEAN" || hasBOOLEAN) {
                synonymsInSelectClause.push_back(currSynonym);
                synonymAttrNameVector.emplace_back(currSynonym, "");
            } else {
                selectTokenGroup->isBOOLEAN = true;
            }
        } else if (*it == ".") {
            if (!validateSynonymAttribute(it, end, attrName)) {
                return false;
            }
            synonymsInSelectClause.push_back(currSynonym);
            synonymAttrNameVector.emplace_back(currSynonym, attrName);
        } else {
            return false;
        }
    }

    selectTokenGroup->synonyms = synonymsInSelectClause;
    selectTokenGroup->synonymsAndAttributes = synonymAttrNameVector;
    return it == end;
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<PatternTokenGroup>& patternTokenGroup) {
    auto tokens = patternTokenGroup->getStringifiedTokens();
    // smallest possible group: pattern a(_, _) = 7 tokens
    if (tokens.size() < 7) {
        return false;
    }
    if (tokens[0] != "pattern") {
        return false;
    }

    auto it = tokens.begin();
    if (tokens[1] == "not") {
        patternTokenGroup->isNegated = true;
        it = it + 2;
    } else {
        patternTokenGroup->isNegated = false;
        it = it + 1;
    }


    auto end = tokens.end();

    std::string synAssign = *it;
    if (!validateName(synAssign)) {
        return false;
    }

    if (!safelyAdvanceIterator(it, end)) {
        return false;
    }

    if (*it != "(" || !safelyAdvanceIterator(it, end)) {
        return false;
    }

    std::string refOne;
    if (!readArgument(it, end, refOne)) {
        return false;
    }
    if (!validateEntityRefs(refOne)) {
        return false;
    }

    if (*it != "," || !safelyAdvanceIterator(it, end)) {
        return false;
    }

    std::string patternChar1 = *it;
    if (patternChar1 != "_" && patternChar1 != "\"") {
        return false;
    }
    bool hasWildcard = patternChar1 == "_";
    bool hasExpression = patternChar1 == "\"";
    bool hasThirdExpression = false;
    bool secondArgumentIsWildcard = false;
    bool thirdArgumentIsWildcard = false;

    if (!safelyAdvanceIterator(it, end)) {
        return false;
    }
    if (patternChar1 == "_") {
        std::string patternChar2 = *it;
        if (patternChar2 == "\"") {
            hasExpression = true;
            if (!safelyAdvanceIterator(it, end)) {
                return false;
            }
        } else if (patternChar2 == ")") {
            secondArgumentIsWildcard = true;
            hasExpression = false;
        } else if (patternChar2 == ",") {
            secondArgumentIsWildcard = true;
            hasThirdExpression = true;
        } else {
            return false;
        }
    }


    std::vector<std::string> expressionTokens;
    if (hasExpression) { // for assignment pattern only
        while (*it != "\"") {
            expressionTokens.push_back(*it);
            if (!safelyAdvanceIterator(it, end)) {
                return false;
            }
        }
        if (!validateExpression(expressionTokens)) {
            return false;
        }

        if (!safelyAdvanceIterator(it, end)) {
            return false;
        }
        if (hasWildcard) {
            if (*it != "_" || !safelyAdvanceIterator(it, end)) {
                return false;
            }
        }
    } else if (hasThirdExpression) {
        if (!safelyAdvanceIterator(it, end)) {
            return false;
        }

        if (*it != "_" || !safelyAdvanceIterator(it, end)) {
            return false;
        }

        thirdArgumentIsWildcard = true;
    }

    if (*(it++) != ")") { // definitely safe to dereference
        return false;
    }

    std::string expression;
    for (const auto& expressionToken : expressionTokens) {
        expression += expressionToken;
    }
    patternTokenGroup->statement = synAssign;
    patternTokenGroup->entity = refOne;
    patternTokenGroup->expression = expression;
    patternTokenGroup->hasWildcard = hasWildcard;
    patternTokenGroup->secondArgumentIsWildcard = secondArgumentIsWildcard;
    patternTokenGroup->thirdArgumentIsWildcard = thirdArgumentIsWildcard;
    patternTokenGroup->hasThirdExpression = hasThirdExpression;
    patternTokenGroup->isNegated = tokens[1] == "not";

    return it == end;
}

bool TokenGroupSyntaxValidator::validateSuchThatRelationship(
    std::vector<std::string>::iterator& it,
    std::vector<std::string>::iterator& end,
    std::string& relRef,
    std::string& refOne,
    std::string& refTwo
    ) {
    relRef = *it;
    if (!safelyAdvanceIterator(it, end)) {
        return false;
    }

    if (relRefList.find(relRef) == relRefList.end()) {
        return false;
    }

    if (*it != "(" || !safelyAdvanceIterator(it, end)) {
        return false;
    }

    if (!readArgument(it, end, refOne)) {
        return false;
    }

    if (*it != "," || !safelyAdvanceIterator(it, end)) {
        return false;
    }

    if (!readArgument(it, end, refTwo)) {
        return false;
    }

    if (*(it++) != ")") {
        return false;
    }

    return true;
}

bool TokenGroupSyntaxValidator::validateRef(std::vector<std::string>::iterator& it,
                                            std::vector<std::string>::iterator& end,
                                            std::string& ref, std::string& refAttrName, bool& isRefInteger) {
    std::string refOneStart = *it;
    // ref: "IDENT" || synonym.attrName || INTEGER
    if (!(refOneStart == "\"" || validateName(refOneStart) || validateInteger(refOneStart))) {
        return false;
    }

    ref += refOneStart;
    if (refOneStart == "\"") { // this is a string "IDENT"
        if (!safelyAdvanceIterator(it, end) || !validateName(*it)) {
            return false;
        }

        ref += *it;

        if (!safelyAdvanceIterator(it, end) || *it != "\"") {
            return false;
        }

        ref += *it;

    } else if (validateName(refOneStart)) { // ref.attrName
        if (!safelyAdvanceIterator(it, end) || *it != ".") {
            return false;
        }

        if (!safelyAdvanceIterator(it, end)) {
            return false;
        }

        if (*it == "procName" || *it == "varName" || *it == "value" || *it == "stmt#") {
            refAttrName += *it;
        } else {
            return false;
        }
    } else {
        isRefInteger = true;
    }

    return true;
}

bool TokenGroupSyntaxValidator::visitUsesAndModifies(const std::shared_ptr<SuchThatTokenGroup>& suchThatTokenGroup) {
    auto tokens = suchThatTokenGroup->getStringifiedTokens();
    // smallest possible group: such that follows(_, _) = 8 tokens
    if (tokens.size() < 8) {
        return false;
    }
    if (tokens[0] != "such" || tokens[1] != "that") {
        return false;
    }

    suchThatTokenGroup->isNegated = tokens[2] == "not";

    auto it = tokens.begin();
    if (suchThatTokenGroup->isNegated) {
        it = it + 3;
    } else {
        it = it + 2;
    }

    auto end = tokens.end();

    std::string relRef, refOne, refTwo;

    if (!validateSuchThatRelationship(it, end, relRef, refOne, refTwo)) {
        return false;
    }

    if (!(validateUsesAndModifies(refOne, refTwo))) {
        return false;
    }

    suchThatTokenGroup->type = relRef;
    suchThatTokenGroup->arg1 = refOne;
    suchThatTokenGroup->arg2 = refTwo;
    suchThatTokenGroup->setAttributes();

    return it == end;
}

bool TokenGroupSyntaxValidator::visitParentFollowsNextAffects(const std::shared_ptr<SuchThatTokenGroup>& suchThatTokenGroup) {
    auto tokens = suchThatTokenGroup->getStringifiedTokens();
    // smallest possible group: such that follows(_, _) = 8 tokens
    if (tokens.size() < 8) {
        return false;
    }
    if (tokens[0] != "such" || tokens[1] != "that") {
        return false;
    }
    suchThatTokenGroup->isNegated = tokens[2] == "not";

    auto it = tokens.begin();
    if (suchThatTokenGroup->isNegated) {
        it = it + 3;
    } else {
        it = it + 2;
    }

    auto end = tokens.end();

    std::string relRef, refOne, refTwo;

    if (!validateSuchThatRelationship(it, end, relRef, refOne, refTwo)) {
        return false;
    }

    if (!validateStatementRefs(refOne) || !validateStatementRefs(refTwo)) {
        return false;
    }

    suchThatTokenGroup->type = relRef;
    suchThatTokenGroup->arg1 = refOne;
    suchThatTokenGroup->arg2 = refTwo;
    suchThatTokenGroup->setAttributes();

    return it == end;
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<ModifiesTokenGroup>& suchThatTokenGroup) {
    return visitUsesAndModifies(suchThatTokenGroup);
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<UsesTokenGroup>& suchThatTokenGroup) {
    return visitUsesAndModifies(suchThatTokenGroup);
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<ParentTokenGroup>& suchThatTokenGroup) {
    return visitParentFollowsNextAffects(suchThatTokenGroup);
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<FollowsTokenGroup>& suchThatTokenGroup) {
    return visitParentFollowsNextAffects(suchThatTokenGroup);
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<NextTokenGroup> &suchThatTokenGroup) {
    return visitParentFollowsNextAffects(suchThatTokenGroup);
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<AffectsTokenGroup> &suchThatTokenGroup) {
    return visitParentFollowsNextAffects(suchThatTokenGroup);
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<CallsTokenGroup> &suchThatTokenGroup) {
    auto tokens = suchThatTokenGroup->getStringifiedTokens();
    // smallest possible group: such that follows(_, _) = 8 tokens
    if (tokens.size() < 8) {
        return false;
    }
    if (tokens[0] != "such" || tokens[1] != "that") {
        return false;
    }

    suchThatTokenGroup->isNegated = tokens[2] == "not";

    auto it = tokens.begin();
    if (suchThatTokenGroup->isNegated) {
        it = it + 3;
    } else {
        it = it + 2;
    }

    auto end = tokens.end();

    std::string relRef, refOne, refTwo;

    if (!validateSuchThatRelationship(it, end, relRef, refOne, refTwo)) {
        return false;
    }

    if (!validateEntityRefs(refOne) || !validateEntityRefs(refTwo)) {
        return false;
    }

    suchThatTokenGroup->type = relRef;
    suchThatTokenGroup->arg1 = refOne;
    suchThatTokenGroup->arg2 = refTwo;
    suchThatTokenGroup->setAttributes();

    return it == end;
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<WithTokenGroup> &withTokenGroup) {
    auto tokens = withTokenGroup->getStringifiedTokens();
    // smallest possible group: with 4 = 4 = 4 tokens
    if (tokens.size() < 4) {
        return false;
    }
    if (tokens[0] != "with") {
        return false;
    }

    auto it = tokens.begin();
    if (tokens[1] == "not") {
        withTokenGroup->isNegated = true;
        it = it + 2;
    } else {
        withTokenGroup->isNegated = false;
        it = it + 1;
    }

    auto end = tokens.end();

    std::string firstRef, firstRefAttrName;
    bool isFirstRefInteger = false;
    // Check for the first Ref
    if (!validateRef(it, end, firstRef, firstRefAttrName, isFirstRefInteger)) {
        return false;
    }

    if (!safelyAdvanceIterator(it, end)) {
        return false;
    }
    if (*it != "=") {
        return false;
    }
    if (!safelyAdvanceIterator(it, end)) {
        return false;
    }

    std::string secondRef, secondRefAttrName;
    bool isSecondRefInteger = false;
    // Check for the Second Ref
    if (!validateRef(it, end, secondRef, secondRefAttrName, isSecondRefInteger)) {
        return false;
    }

    withTokenGroup->firstRef = firstRef;
    withTokenGroup->firstRefAttrName = firstRefAttrName;
    withTokenGroup->isFirstRefInteger = isFirstRefInteger;
    withTokenGroup->secondRef = secondRef;
    withTokenGroup->secondRefAttrName = secondRefAttrName;
    withTokenGroup->isSecondRefInteger = isSecondRefInteger;

    it++;
    return it == end;
}

bool TokenGroupSyntaxValidator::visit(const std::shared_ptr<InvalidClauseTokenGroup>& invalidTokenGroup) {
    return false;
}
