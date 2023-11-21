#pragma once

#include <vector>
#include <string>
#include "query_processing_system/query_evaluator/storage_reader/IModifiesStatementReader.h"

class IAssignPatternReader : virtual public IModifiesStatementReader {
public:
    virtual bool hasMatchingPattern(int statementNumber, std::vector<std::string> postfixExpression,
                                    bool exactMatch) = 0;
};
