#include <execution>
#include <stdexcept>
#include <algorithm>
#include "SyntaxValidator.h"

// NAME: LETTER ( LETTER | DIGIT )*
bool SyntaxValidator::validateName(const std::string& name) {
    if (name.empty()) {
        return false;
    }

    if (isdigit(name[0])) {
        return false;
    }
    return std::all_of(name.begin(), name.end(), isalnum);
}

// INTEGER : 0 | NZDIGIT ( DIGIT )*
bool SyntaxValidator::validateInteger(const std::string& integer) {
    if (integer.empty()) {
        return false;
    }

    size_t i = 0;
    if (integer.length() > 1 && integer[i] == '0') {
        return false;
    }

    for (; i < integer.length(); i++) {
        if (!isdigit(integer[i])) {
            return false;
        }
    }

    return true;
}

/**
 * Anything within a conditional expr that does not follow the 2 parenthesis 1 cond op syntax should be a relational expr.
 * Relational expr cannot be surrounded by brace (Unless in a conditional expr) and they MUST have a relational operator
 * @param t Relation expression in vector of strings form
 */
void SyntaxValidator::validateRelExpr(std::vector<std::string> t) {
    int count = 0;
    for (const auto& i : t) {
        if (isRelationOperator(i)) {
            count++;
        } else if (isConditionOperator(i)) {
            throw std::invalid_argument("Conditional expression does not follow syntax with two parenthesis at " + i);
        }
    }
    if (count > 1) {
        throw std::invalid_argument("More than one relational op in Relational expr");
    } else if (count == 0) {
        throw std::invalid_argument("Relational expression must have relational operator");
    }
    std::vector<std::string> leftArg;
    std::vector<std::string> rightArg;
    auto stringPtr = t.begin();
    while (!isRelationOperator(*stringPtr)) {
        leftArg.push_back(*stringPtr);
        stringPtr++;
    }
    stringPtr++;
    while (stringPtr != t.end()) {
        rightArg.push_back(*stringPtr);
        stringPtr++;
    }
    if (!(validateExpression(leftArg) && validateExpression(rightArg))) {
        throw std::invalid_argument("Error in relational factor");
    }
}

/**
 * Validates a conditional expression
 * @param t vector of tokens in string form, representing the condition with initial parenthesis excluded
 * i.e "if (a < b)" this function should receive "a < b"
 * Control flow is as follows:
 * Check for negate op i.e (!)
 * Check for that it follows the syntax for && and || that has 2 parenthesis i.e "(...)&&(...)"
 * Else, parse it as relational expression
 */
void SyntaxValidator::validateCondExpr(std::vector<std::string> t) {
    auto curIt = t.begin();

    if (t.size() < 4) {
        validateRelExpr(t);
    } else if (t[0] == "!" && t[1] == "(") { /* Negation condition */
        curIt++;
        auto endPtr = getEndPointerOfParen(curIt);
        endPtr++;
        if (endPtr != t.end()) {
            throw std::invalid_argument("Negate condition cannot have any characters after it");
        }
        validateCondExpr(getSubVectorBetweenParen(curIt));
    } else if (t[0] == "(") {
        // Check if condition with brace is actually a condition expr syntax i.e 2 parenthesis 1 cond op
        auto leftArgEnd = getEndPointerOfParen(curIt);
        auto op = leftArgEnd+1;
        // Expression has invalid character after parenthesis, violates syntax for Conditional expr
        if (op == t.end()) {
            throw std::invalid_argument("Brace alone without conditional operator is not valid syntax");
        }
        if (!isConditionOperator(*op)) {
            return validateRelExpr(t);
        }
        auto rightArgStart = op + 1;
        if (*rightArgStart != "(") {
            throw std::invalid_argument(
                      "Conditional expression does not follow syntax with two parenthesis at \""
                      + *op + " " + *rightArgStart +
                      "\"");
        }
        auto rightArgEnd = getEndPointerOfParen(rightArgStart);
        auto end = rightArgEnd + 1;
        if (end != t.end()) {
            throw std::invalid_argument("Conditional expression has character after closed parenthesis");
        }
        auto leftArg = getSubVectorBetweenParen(curIt);
        auto rightArg = getSubVectorBetweenParen(rightArgStart);
        validateCondExpr(leftArg);
        validateCondExpr(rightArg);
    } else {
        // If no starting brace, it must be a relational
        validateRelExpr(t);
    }
}

std::vector<std::string> SyntaxValidator::getSubVectorBetweenParen(SyntaxValidator::tokenPointer t) {
    std::vector<std::string> res;
    auto endPointer = getEndPointerOfParen(t);
    auto start = t + 1;
    res.insert(res.begin(), start, endPointer);
    return res;
}

/**
 * Used to get an iterator pointing to the corresponding closing brace
 * @param t iterator pointing to an open brace
 * @return iterator pointing to a close brace
 */
SyntaxValidator::tokenPointer SyntaxValidator::getEndPointerOfParen(SyntaxValidator::tokenPointer t) {
    std::stack<std::string> s;
    if (*t != "(") {
        throw std::invalid_argument("Function needs to start with pointer to open parenthesis");
    }
    s.emplace("(");
    t++;
    auto cur = *t;
    if (cur == ")") {
        throw std::invalid_argument("Empty parenthesis");
    }
    while (!s.empty()) {
        if (cur == "(") {
            s.push(cur);
        } else if (cur == ")") {
            s.pop();
            if (s.empty()) {
                return t;
            }
        }
        t++;
        cur = *t;
    }
    throw std::logic_error{"[SyntaxValidator::getEndPointerOfParen] returns nothing"};
}

bool SyntaxValidator::validateBrackets(const std::vector<std::string>& bracketArray) {
    // ai-gen start (gpt3, 1)
    std::stack<std::string> stack;

    for (const std::string& bracket : bracketArray) {
        if (bracket == "(" || bracket == "{") {
            stack.push(bracket);
        } else if (bracket == ")" || bracket == "}") {
            if (stack.empty() || (bracket == ")" && stack.top() != "(") || (bracket == "}" && stack.top() != "{")) {
                return false;
            }
            stack.pop();
        }
    }

    return stack.empty();
    // ai-gen end
}

// argument is vector of tokens
bool SyntaxValidator::validateExpression(const std::vector<std::string>& tokenVector) {
    if (!validateBrackets(tokenVector)) {
        return false;
    }
    std::stack<int> outerLevelIndices;
    int currentLevelIndex = 0;
    for (const auto & token : tokenVector) {
        if (token == "(") {
            // cannot open a bracket on a factor
            if (currentLevelIndex % 2 == 1) {
                return false;
            }
            outerLevelIndices.push(currentLevelIndex);
            currentLevelIndex = 0;
        } else if (token == ")") {
            // cannot close a bracket on an operator or close a bracket immediately
            if (currentLevelIndex % 2 == 0) {
                return false;
            }
            // +1 accounts for this set of brackets as a factor
            currentLevelIndex = outerLevelIndices.top() + 1;
            outerLevelIndices.pop();
        } else {
            if (currentLevelIndex % 2 == 0) {
                // within every bracket level, the first/third/... term is a factor
                if (!validateName(token) && !validateInteger(token)) {
                    return false;
                }
            } else {
                // within every bracket level, the second/forth... is operator
                if (validExpressionOperators.find(token) == validExpressionOperators.end()) {
                    return false;
                }
            }
            currentLevelIndex++;
        }
    }
    // cannot end inside brackets, and must end on a factor
    return outerLevelIndices.empty() && currentLevelIndex % 2 == 1;
}
