#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include "source_processor/ast/ProgramNode.h"
#include "source_processor/ast/ProcedureNode.h"
#include "source_processor/ast/statements/StatementNode.h"
#include "source_processor/exceptions/DesignExtractorException.h"
#include "source_processor/exceptions/ExceptionMessage.h"

class BaseExtractor {
protected:
    virtual void extractProcedure(std::shared_ptr<ProcedureNode>& node);
    virtual void extractStatementList(std::vector<std::shared_ptr<StatementNode> >   stmtList);
    virtual void extractStatement(std::shared_ptr<StatementNode>& node) = 0;

public:
    void extract(std::shared_ptr<ProgramNode>& root);
};
