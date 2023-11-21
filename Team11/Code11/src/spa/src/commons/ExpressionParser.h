#pragma once

#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

class ExpressionParser {
public:
    static std::vector<std::string> convertToPostfix(std::string infix);
    static bool isSubarray(std::vector<std::string> array, std::vector<std::string> subarray);

// For debugging purposes
    static void printPostfixNotation(const std::vector<std::string>& postFixVector);

private:
    static inline std::vector<std::string> postfixNotation;
    static inline std::stack<char> operatorStack;

    static void removeSpaces(std::string& str);
    static bool isOperator(char c);
    static void handleCloseParen();
    static void handleOperator(char c);
    static void handleOperand(std::string& infix, int& i);
    static void popRemainingElements();
};
