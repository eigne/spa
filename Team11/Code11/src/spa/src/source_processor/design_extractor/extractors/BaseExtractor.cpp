#include "BaseExtractor.h"

void BaseExtractor::extract(std::shared_ptr<ProgramNode>& root) {
    if (root->getProcLst()->empty()) {
        throw DesignExtractorException(EMPTY_PROCEDURE_LIST_ERROR);
    }

    for (auto& procedure : *root->getProcLst()) {
        extractProcedure(procedure);
    }
}

void BaseExtractor::extractProcedure(std::shared_ptr<ProcedureNode>& node) {
    extractStatementList(*node->getStmtLst());
}

void BaseExtractor::extractStatementList(std::vector<std::shared_ptr<StatementNode> > stmtList) {
    for (auto& stmt : stmtList) {
        extractStatement(stmt);
    }
}
