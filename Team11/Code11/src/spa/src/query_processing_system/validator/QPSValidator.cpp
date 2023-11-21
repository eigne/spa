#include "QPSValidator.h"

QPSValidator::QPSValidator(const std::vector<std::shared_ptr<TokenGroup> > &tokenGroups) {
    this->tokenGroups = tokenGroups;
}

std::string QPSValidator::validate() {
    if (tokenGroups.empty()) {
        return "SyntaxError";
    }

    for (const auto& tokenGroup : tokenGroups) {
        if (!tokenGroup->validateSyntax(syntaxValidator)) {
            return "SyntaxError";
        }
    }

    for (const auto& tokenGroup : tokenGroups) {
        if (!tokenGroup->validateSemantics(semanticValidator)) {
            return "SemanticError";
        }
    }

    return "";
}
