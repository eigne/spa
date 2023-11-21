#include <stdexcept>
#include <iostream>
#include "ExpressionParser.h"

std::vector<std::string> ExpressionParser::convertToPostfix(std::string infix) {
    ExpressionParser::operatorStack = std::stack<char>();
    ExpressionParser::postfixNotation = std::vector<std::string>();

    // remove spaces before parsing the infix expression to make parsing easier
    removeSpaces(infix);

    if (infix.empty()) return postfixNotation;

    for (int i = 0; i < infix.length(); i++) {
        char curr = infix[i];

        if (std::isalnum(curr)) /* varible */ {
            handleOperand(infix, i);
        } else if (curr == '(') /* open paren */ {
            ExpressionParser::operatorStack.push(curr);
        } else if (curr == ')') /* close paren */ {
            handleCloseParen();
        } else if (isOperator(curr)) /* operator */ {
            if (i == 0 || i == infix.length() - 1 || isOperator(infix[i-1])) {
                throw std::invalid_argument("Invalid expression");
            }
            handleOperator(curr);
        } else /* invalid character */ {
            throw std::invalid_argument("Invalid character in expression");
        }
    }
    popRemainingElements();

    if (ExpressionParser::postfixNotation.empty()) {
        throw std::invalid_argument("Invalid expression");
    }

    return postfixNotation;
}

void ExpressionParser::handleCloseParen() {
    // ai-gen start (copilot, 1)
    while (!ExpressionParser::operatorStack.empty() && ExpressionParser::operatorStack.top() != '(') {
        if (ExpressionParser::postfixNotation.size() < 2) {
            throw std::invalid_argument("Invalid expression");
        }

        auto top = std::string(1, ExpressionParser::operatorStack.top());
        ExpressionParser::postfixNotation.push_back(top);
        ExpressionParser::operatorStack.pop();
    }
    if (ExpressionParser::operatorStack.empty()) {
        throw std::invalid_argument("Mismatched parentheses");
    }
    ExpressionParser::operatorStack.pop();
    // ai-gen end
}

void ExpressionParser::handleOperator(char c) {
    static std::unordered_map<char, int> precedence = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'%', 2} };

    // ai-gen start (copilot, 1)
    while (!ExpressionParser::operatorStack.empty() &&
           precedence[c] <= precedence[ExpressionParser::operatorStack.top()]) {
        if (ExpressionParser::postfixNotation.size() < 2) {
            throw std::invalid_argument("Invalid expression");
        }

        auto top = std::string(1, operatorStack.top());
        ExpressionParser::postfixNotation.push_back(top);
        ExpressionParser::operatorStack.pop();
    }
    ExpressionParser::operatorStack.push(c);
    // ai-gen end
}

void ExpressionParser::handleOperand(std::string& infix, int& i) {
    // ai-gen start (copilot, 1)
    int j = i;
    while (j < infix.length() && std::isalnum(infix[j])) {
        j++;
    }
    ExpressionParser::postfixNotation.push_back(infix.substr(i, j - i));
    i = j - 1;
    // ai-gen end
}

bool ExpressionParser::isOperator(char c) {
    static const std::unordered_set<char> operators = { '+', '-', '*', '/', '%' };
    return operators.find(c) != operators.end();
}

void ExpressionParser::removeSpaces(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

void ExpressionParser::popRemainingElements() {
    while (!ExpressionParser::operatorStack.empty()) {
        if (ExpressionParser::operatorStack.top() == '(') {
            throw std::invalid_argument("Mismatched parentheses");
        }
        auto top = std::string(1, ExpressionParser::operatorStack.top());
        ExpressionParser::postfixNotation.push_back(top);
        ExpressionParser::operatorStack.pop();
    }
}

bool ExpressionParser::isSubarray(std::vector<std::string> array, std::vector<std::string> subarray) {
    int i = 0, j = 0, n = array.size(), m = subarray.size();
    if (m == 0) return true;

    while (i < n && j < m) {
        if (array[i] == subarray[j]) {
            i++;
            j++;

            if (j == m) return true;
        } else {
            i = i - j + 1;
            j = 0;
        }
    }

    return false;
}

// For debugging purposes
void ExpressionParser::printPostfixNotation(const std::vector<std::string>& postFixVector) {
    for (auto& str : postFixVector) {
        std::cout << str << " ";
    }
    std::cout << std::endl;
}
