#include "ParserFactory.h"
#include "ReadParser.h"
#include "PrintParser.h"
#include "ProcedureParser.h"
#include "CallParser.h"
#include "AssignmentParser.h"
#include "WhileParser.h"
#include "IfParser.h"

std::shared_ptr<AbstractParser> ParserFactory::getParser(const std::string& val) {
    ParserType p = parserTypeMap[val];
    switch (p) {
    case ParserType::PROC:
        return std::make_shared<ProcedureParser>();
    case ParserType::READ:
        return std::make_shared<ReadParser>();
    case ParserType::PRINT:
        return std::make_shared<PrintParser>();
    case ParserType::CALL:
        return std::make_shared<CallParser>();
    case ParserType::WHILE:
        return std::make_shared<WhileParser>();
    case ParserType::IF:
        return std::make_shared<IfParser>();
    default:
        return std::make_shared<AssignmentParser>();
    }
}
