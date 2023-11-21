#include "SimpleProgramValidator.h"
#include <unordered_set>

SimpleProgramValidator::SimpleProgramValidator() = default;

std::unordered_set<std::string> validSpecialCharacters = { "{", "}", "(", ")", ";", "=", ">", "<",
                                                           "+", "-", "*", "/", "%", "!", "&", "|" };
std::unordered_set<std::string> validLogicalOperators = { "&&", "||", ">=", "<=", "==", "!=" };

bool SimpleProgramValidator::validateSpecialCharacter(const std::string& s) {
    if (s.length() > 2) {
        // Throw error: Invalid Character
        return false;
    }

    if (s.length() == 2) {
        return validLogicalOperators.find(s) != validLogicalOperators.end();
    }

    return validSpecialCharacters.find(s) != validSpecialCharacters.end();
}
