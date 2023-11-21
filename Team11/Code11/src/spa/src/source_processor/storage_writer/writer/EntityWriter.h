#pragma once

#include <memory>
#include "source_processor/storage_writer/writer_interface/IEntityWriter.h"
#include "program_knowledge_base/facades/PKBSP.h"

class EntityWriter : public IEntityWriter {
private:
    std::shared_ptr<PKBSP> kbWriter;
public:
    explicit EntityWriter(std::shared_ptr<PKBSP>& writer);

    void insertProcedure(std::string procedureName) override;
    void insertVariable(std::string varName) override;
    void insertConstant(std::string value) override;

    void insertRead(std::shared_ptr<ReadNode> node) override;
    void insertPrint(std::shared_ptr<PrintNode> node) override;
    void insertAssign(std::shared_ptr<AssignNode> node) override;
    void insertCall(std::shared_ptr<CallNode> node) override;
    void insertIf(std::shared_ptr<IfNode> node) override;
    void insertWhile(std::shared_ptr<WhileNode> node) override;
    void insertPattern(std::shared_ptr<AssignNode> node) override;

    bool containsProcedure(std::string procedureName) override;
};
