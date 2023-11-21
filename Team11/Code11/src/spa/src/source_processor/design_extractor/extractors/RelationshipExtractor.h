#pragma once

#include <memory>
#include <string>
#include "BaseExtractor.h"
#include "source_processor/design_extractor/RelationshipContext.h"
#include "source_processor/design_extractor/interface/ASTVisitor.h"
#include "source_processor/storage_writer/writer_interface/IRelationshipWriter.h"
#include "source_processor/ast/ProgramNode.h"
#include "source_processor/ast/ProcedureNode.h"
#include "source_processor/ast/expressions/VariableNode.h"

class RelationshipExtractor : public ASTVistor, public BaseExtractor {
private:
    RelationshipContext context;
    std::string currentProcedureName;
    std::shared_ptr<IRelationshipWriter> relationshipWriter;

    void extractProcedure(std::shared_ptr<ProcedureNode>& node) override;
    void extractStatementList(std::vector<std::shared_ptr<StatementNode> > stmtList) override;
    void extractStatement(std::shared_ptr<StatementNode>& node) override;
    void interlinkRelationships();
    void interlinkStatementRelationships(std::string& procedureName);
    void interlinkProcedureRelationships(std::string& procedureName);

public:
    explicit RelationshipExtractor(std::shared_ptr<IRelationshipWriter> relationshipWriter);

    void extract(std::shared_ptr<ProgramNode>& node);

    void visit(std::shared_ptr<IfNode>& ifNode) override;
    void visit(std::shared_ptr<WhileNode>& whileNode) override;
    void visit(std::shared_ptr<CallNode>& forNode) override;
    void visit(std::shared_ptr<AssignNode>& assignNode) override;
    void visit(std::shared_ptr<PrintNode>& printNode) override;
    void visit(std::shared_ptr<ReadNode>& readNode) override;
};
