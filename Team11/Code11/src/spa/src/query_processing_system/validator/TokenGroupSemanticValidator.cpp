#include <algorithm>
#include <execution>
#include "TokenGroupSemanticValidator.h"

TokenGroupSemanticValidator::TokenGroupSemanticValidator() {
    this->synonymMap = {};
}

// stmtRef : synonym | '_' | INTEGER
bool TokenGroupSemanticValidator::validateSynonymType(const std::string &ref, SynonymType requiredType) {
    if (synonymMap.find(ref) == synonymMap.end()) {
        return false;
    }

    auto types = synonymMap.at(ref);
    return std::any_of(types.begin(), types.end(), [&](const auto& type) {
        return type == requiredType;
    });
}

bool TokenGroupSemanticValidator::validateSynonymAttribute(const std::string &synonymType,
                                                           const std::string &attributeType) {
    AttributeType attr = attributeType == "procName" ? AttributeType::PROCEDURE_NAME
            : attributeType == "varName" ? AttributeType::VARIABLE_NAME
            : attributeType == "stmt#" ? AttributeType::STATEMENT_NUMBER : AttributeType::VALUE;

    auto allowedAttributes = attributeMap.at(synonymType);
    return std::any_of(allowedAttributes.begin(), allowedAttributes.end(), [&](const auto& attribute) {
        return attribute == attr;
    });
}

bool TokenGroupSemanticValidator::validateStatementStatement(
    const std::shared_ptr<SuchThatTokenGroup> &suchThatTokenGroup) {
    if (validateName(suchThatTokenGroup->arg1)) { // arg is a synonym
        if (!validateSynonymType(suchThatTokenGroup->arg1, SynonymType::STATEMENT)) {
            return false;
        }
    }
    if (validateName(suchThatTokenGroup->arg2)) {
        if (!validateSynonymType(suchThatTokenGroup->arg2, SynonymType::STATEMENT)) {
            return false;
        }
    }
    return true;
}

bool TokenGroupSemanticValidator::validateModifiesAndUses(
            const std::shared_ptr<SuchThatTokenGroup> &suchThatTokenGroup) {
    if (suchThatTokenGroup->arg1 == "_") {
        return false;
    }

    if (validateName(suchThatTokenGroup->arg1)) {
        if (!validateSynonymType(suchThatTokenGroup->arg1, SynonymType::STATEMENT) &&
            !validateSynonymType(suchThatTokenGroup->arg1, SynonymType::PROCEDURE)) {
            return false;
        }
    }
    if (validateName(suchThatTokenGroup->arg2)) {
        if (!validateSynonymType(suchThatTokenGroup->arg2, SynonymType::VARIABLE)) {
            return false;
        }
    }
    return true;
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<DeclarationTokenGroup> &declarationTokenGroup) {
    auto types = designEntityList.at(declarationTokenGroup->type);
    for (const auto& synonym : declarationTokenGroup->synonyms) {
        if (synonymMap.find(synonym) != synonymMap.end()) {
            return false;
        }
        synonymMap.insert({synonym, types});
        synonymMapWithType.insert({synonym, declarationTokenGroup->type});
    }
    return true;
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<SelectTokenGroup> &selectTokenGroup) {
    auto selected = selectTokenGroup->synonymsAndAttributes;
    for (const auto& synonymAndAttr : selected) {
        if (synonymMap.find(synonymAndAttr.first) == synonymMap.end()) {
            return false;
        }
        auto currSynonymAttrName = synonymAndAttr.second;
        auto currSynonymType = synonymMapWithType.at(synonymAndAttr.first);
        if (!currSynonymAttrName.empty()) {
            if (!validateSynonymAttribute(currSynonymType, currSynonymAttrName)) {
                return false;
            }
        }
    }
    return true;
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<FollowsTokenGroup>& followsTokenGroup) {
    return validateStatementStatement(followsTokenGroup);
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<ParentTokenGroup>& parentTokenGroup) {
    return validateStatementStatement(parentTokenGroup);
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<ModifiesTokenGroup>& modifiesTokenGroup) {
    return validateModifiesAndUses(modifiesTokenGroup);
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<UsesTokenGroup>& usesTokenGroup) {
    return validateModifiesAndUses(usesTokenGroup);
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<CallsTokenGroup> &callsTokenGroup) {
    if (validateName(callsTokenGroup->arg1)) { // arg is a synonym
        if (!validateSynonymType(callsTokenGroup->arg1, SynonymType::PROCEDURE)) {
            return false;
        }
    }
    if (validateName(callsTokenGroup->arg2)) {
        if (!validateSynonymType(callsTokenGroup->arg2, SynonymType::PROCEDURE)) {
            return false;
        }
    }
    return true;
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<AffectsTokenGroup> &suchThatTokenGroup) {
    return validateStatementStatement(suchThatTokenGroup);
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<NextTokenGroup> &nextTokenGroup) {
    return validateStatementStatement(nextTokenGroup);
}

bool TokenGroupSemanticValidator::validateAssignPattern(const std::shared_ptr<PatternTokenGroup> &patternTokenGroup) {
    if (!validateSynonymType(patternTokenGroup->statement, SynonymType::ASSIGNMENT)) {
        return false;
    }

    if (validateName(patternTokenGroup->entity)) {
        return validateSynonymType(patternTokenGroup->entity, SynonymType::VARIABLE);
    } else {
        return true;
    }
}

bool TokenGroupSemanticValidator::validateWhilePattern(const std::shared_ptr<PatternTokenGroup> &patternTokenGroup) {
    if (!validateSynonymType(patternTokenGroup->statement, SynonymType::WHILE)) {
        return false;
    }

    if (validateName(patternTokenGroup->entity)) {
        return validateSynonymType(patternTokenGroup->entity, SynonymType::VARIABLE);
    } else {
        return patternTokenGroup->secondArgumentIsWildcard && !patternTokenGroup->hasThirdExpression;
    }
}

bool TokenGroupSemanticValidator::validateIfPattern(const std::shared_ptr<PatternTokenGroup> &patternTokenGroup) {
    if (!validateSynonymType(patternTokenGroup->statement, SynonymType::IF)) {
        return false;
    }

    if (validateName(patternTokenGroup->entity)) {
        return validateSynonymType(patternTokenGroup->entity, SynonymType::VARIABLE);
    } else {
        return patternTokenGroup->secondArgumentIsWildcard
                && patternTokenGroup->hasThirdExpression && patternTokenGroup->thirdArgumentIsWildcard;
    }
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<PatternTokenGroup> &patternTokenGroup) {
    if (synonymMap.find(patternTokenGroup->statement) == synonymMap.end()) {
        return false;
    }
    auto type = synonymMapWithType.at(patternTokenGroup->statement);
    if (type == "assign") {
        patternTokenGroup->type = "assign";
        return validateAssignPattern(patternTokenGroup);
    } else if (type == "if") {
        patternTokenGroup->type = "if";
        return validateIfPattern(patternTokenGroup);
    } else if (type == "while") {
        patternTokenGroup->type = "while";
        return validateWhilePattern(patternTokenGroup);
    } else {
        return false;
    }
}

bool TokenGroupSemanticValidator::findType(AttributeTypeGeneric& refOneRes,
                                           std::string& ref, std::string& refAttrName, bool& isRefInteger) {
    if (!refAttrName.empty()) { // ref is synonym
        if (synonymMap.find(ref) == synonymMap.end()) {
            return false;
        }

        auto currSynonymType = synonymMapWithType.at(ref);
        if (refAttrName == "varName") {
            if (!(currSynonymType == "variable" || currSynonymType == "read" || currSynonymType == "print")) {
                return false;
            }
            refOneRes = AttributeTypeGeneric::NAME;
        } else if (refAttrName == "procName") {
            if (!(currSynonymType == "procedure" || currSynonymType == "call")) {
                return false;
            }
            refOneRes = AttributeTypeGeneric::NAME;
        } else if (refAttrName == "value") {
            if (!(currSynonymType == "constant")) {
                return false;
            }
            refOneRes = AttributeTypeGeneric::INTEGER;
        } else { // "stmt#"
            if (!(currSynonymType == "stmt" || currSynonymType == "read" || currSynonymType == "print" || currSynonymType == "call"
                || currSynonymType == "while" || currSynonymType == "if" || currSynonymType == "assign")) {
                return false;
            }
            refOneRes = AttributeTypeGeneric::INTEGER;
        }
    } else if (isRefInteger) {
        refOneRes = AttributeTypeGeneric::INTEGER;
    } else {
        refOneRes = AttributeTypeGeneric::NAME;
    }
    return true;
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<WithTokenGroup> &withTokenGroup) {
    // check if ref is synonym by checking if there is any refAttr
    // check if the synonym has been declared
    // if synonym is declared then find out what is the attrname and associate it was the relevant type
    AttributeTypeGeneric refOneRes;
    if (!findType(refOneRes, withTokenGroup->firstRef, withTokenGroup->firstRefAttrName, withTokenGroup->isFirstRefInteger)) {
        return false;
    }

    AttributeTypeGeneric refTwoRes;
    if (!findType(refTwoRes, withTokenGroup->secondRef, withTokenGroup->secondRefAttrName, withTokenGroup->isSecondRefInteger)) {
        return false;
    }

    if (refOneRes != refTwoRes) {
        return false;
    }

    return true;
}

bool TokenGroupSemanticValidator::visit(const std::shared_ptr<InvalidClauseTokenGroup> &invalidClauseTokenGroup) {
    return false;
}
