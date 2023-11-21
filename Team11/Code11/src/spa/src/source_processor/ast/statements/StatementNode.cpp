#include "StatementNode.h"

void StatementNode::setLineNum(int index) {
    lineNum = index;
}

int StatementNode::getLineNum() const {
    return lineNum;
}
