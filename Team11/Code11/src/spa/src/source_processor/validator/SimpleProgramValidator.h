#pragma once

#include <stack>
#include <vector>
#include <string>
#include "../../commons/validator/SyntaxValidator.h"

class SimpleProgramValidator : public SyntaxValidator {
public:
    SimpleProgramValidator();
    bool validateSpecialCharacter(const std::string& s);
};
