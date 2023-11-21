#pragma once

#include <memory>
#include "BaseExtractor.h"
#include "source_processor/design_extractor/interface/ASTVisitor.h"
#include "source_processor/storage_writer/writer_interface/IEntityWriter.h"
#include "source_processor/ast/ProgramNode.h"
#include "source_processor/ast/ProcedureNode.h"
#include "source_processor/ast/statements/ConditionNode.h"

class EntityExtractor : public ASTVistor, public BaseExtractor {
private:
    std::shared_ptr<IEntityWriter> entityWriter;
    void extractProcedure(std::shared_ptr<ProcedureNode>& node) override;
    void extractStatement(std::shared_ptr<StatementNode>& node) override;
    void extractConstantsFromConditionNode(const std::shared_ptr<ConditionNode>&);
    void extractVariablesFromConditionNode(const std::shared_ptr<ConditionNode>&);

public:
    explicit EntityExtractor(std::shared_ptr<IEntityWriter> entityWriter);

    void extract(std::shared_ptr<ProgramNode>& node);

    void visit(std::shared_ptr<IfNode>& ifNode) override;
    void visit(std::shared_ptr<WhileNode>& whileNode) override;
    void visit(std::shared_ptr<CallNode>& forNode) override;
    void visit(std::shared_ptr<AssignNode>& assignNode) override;
    void visit(std::shared_ptr<PrintNode>& printNode) override;
    void visit(std::shared_ptr<ReadNode>& readNode) override;
};
