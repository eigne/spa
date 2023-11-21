#pragma once

#include <memory>
#include <string>
#include "source_processor/ast/ProcedureNode.h"
#include "source_processor/ast/statements/AssignNode.h"
#include "source_processor/ast/statements/CallNode.h"
#include "source_processor/ast/statements/IfNode.h"
#include "source_processor/ast/statements/PrintNode.h"
#include "source_processor/ast/statements/ReadNode.h"
#include "source_processor/ast/statements/WhileNode.h"

class IEntityWriter {
public:
    virtual void insertProcedure(std::string procedureName) = 0;
    virtual void insertVariable(std::string varName) = 0;
    virtual void insertConstant(std::string value) = 0;

    virtual void insertRead(std::shared_ptr<ReadNode> node) = 0;
    virtual void insertPrint(std::shared_ptr<PrintNode> node) = 0;
    virtual void insertAssign(std::shared_ptr<AssignNode> node) = 0;
    virtual void insertCall(std::shared_ptr<CallNode> node) = 0;
    virtual void insertIf(std::shared_ptr<IfNode> node) = 0;
    virtual void insertWhile(std::shared_ptr<WhileNode> node) = 0;
    virtual void insertPattern(std::shared_ptr<AssignNode> node) = 0;

    virtual bool containsProcedure(std::string procedureName) = 0;
};
