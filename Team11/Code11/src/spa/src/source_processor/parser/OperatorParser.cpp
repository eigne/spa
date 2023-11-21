#include "OperatorParser.h"

std::shared_ptr<ExpressionNode> OperatorParser::parse(std::vector<std::shared_ptr<Token> >::iterator &curToken,
                                                      int index) {
    std::string fOp;
    std::vector<std::string> var;
    std::vector<std::string> c;
    std::string cur = (**curToken).getStringValue();
    SyntaxValidator s;
    while (cur != ";") {
        fOp += cur;
        if (isdigit(cur[0])) {
            if (!s.validateInteger(cur)) {
                throw std::invalid_argument("Operand is not a proper int in assignment line " + std::to_string(index));
            }
            c.push_back(cur);
        } else if (isalpha(cur[0])) {
            if (!s.validateName(cur)) {
                throw std::invalid_argument("Operand is not a proper variable name in assignment line " + std::to_string(index));
            }
            var.push_back(cur);
        }
        curToken++;
        cur = (**curToken).getStringValue();
    }
    return std::make_shared<ExpressionNode>(fOp, var, c, index);
}
