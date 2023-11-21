#include "catch.hpp"
#include "query_processing_system/query_evaluator/design_entity_visitor/SynonymVisitor.h"

class SynonymReaderStub : public ISynonymReader {
    std::unordered_set<int> getAllStatements() {
        return {1, 2, 3, 4, 5, 6};
    }
    std::unordered_set<int> getAllReads() {
        return {1};
    }
    std::unordered_set<int> getAllPrints() {
        return {2};
    }
    std::unordered_set<int> getAllCalls() {
        return {3};
    }
    std::unordered_set<int> getAllWhiles() {
        return {4};
    }
    std::unordered_set<int> getAllIfs() {
        return {5};
    }
    std::unordered_set<int> getAllAssigns() {
        return {6};
    }

    std::unordered_set<std::string> getAllVariables() {
        return {"a", "b", "c"};
    }
    std::unordered_set<std::string> getAllConstants() {
        return {"1", "2", "3"};
    }
    std::unordered_set<std::string> getAllProcedures() {
        return {"main", "p1"};
    }

    std::string getProcedureCalledByCall(int statementNumber) {
        return "p1";
    }

    std::unordered_set<std::string> getVariablesModifiedInStatement(int statementNumber) {
        return {"a"};
    }

    std::unordered_set<std::string> getVariablesUsedInStatement(int statementNumber) {
        return {"b"};
    }

    // unused
    std::unordered_set<int> getStatementsThatModifyAnyVariable() { return {}; }
    std::unordered_set<int> getStatementsThatModifyVariable(std::string variableName) { return {}; }
    std::unordered_set<int> getStatementsThatUseAnyVariable() { return {}; }
    std::unordered_set<int> getStatementsThatUseVariable(std::string variableName) { return {}; }
};

TEST_CASE("SynonymVisitor Test") {
    std::shared_ptr<ISynonymReader> reader = std::make_shared<SynonymReaderStub>();
    SynonymVisitor visitor(reader);

    std::shared_ptr<StatementSynonym> s = std::make_shared<StatementSynonym>("s");
    std::shared_ptr<ReadSynonym> r = std::make_shared<ReadSynonym>("r");
    std::shared_ptr<PrintSynonym> p = std::make_shared<PrintSynonym>("p");
    std::shared_ptr<CallSynonym> c = std::make_shared<CallSynonym>("c");
    std::shared_ptr<WhileSynonym> w = std::make_shared<WhileSynonym>("w");
    std::shared_ptr<IfSynonym> i = std::make_shared<IfSynonym>("i");
    std::shared_ptr<AssignSynonym> a = std::make_shared<AssignSynonym>("a");

    std::shared_ptr<VariableSynonym> var = std::make_shared<VariableSynonym>("var");
    std::shared_ptr<ConstantSynonym> con = std::make_shared<ConstantSynonym>("con");
    std::shared_ptr<ProcedureSynonym> proc = std::make_shared<ProcedureSynonym>("proc");

    REQUIRE(s->getStatementNumbers().empty());
    REQUIRE(r->getStatementNumbers().empty());
    REQUIRE(p->getStatementNumbers().empty());
    REQUIRE(c->getStatementNumbers().empty());
    REQUIRE(w->getStatementNumbers().empty());
    REQUIRE(i->getStatementNumbers().empty());
    REQUIRE(a->getStatementNumbers().empty());

    REQUIRE(var->getNames().empty());
    REQUIRE(con->getNames().empty());
    REQUIRE(proc->getNames().empty());

    visitor.visit(s);
    visitor.visit(r);
    visitor.visit(p);
    visitor.visit(c);
    visitor.visit(w);
    visitor.visit(i);
    visitor.visit(a);

    visitor.visit(var);
    visitor.visit(con);
    visitor.visit(proc);

    REQUIRE(s->getStatementNumbers() == std::unordered_set<int>({1, 2, 3, 4 , 5, 6}));

    REQUIRE(r->getStatementNumbers() == std::unordered_set<int>({1}));
    REQUIRE(r->getVariableNameAttributes() == std::unordered_set<std::string>({"a"}));

    REQUIRE(p->getStatementNumbers() == std::unordered_set<int>({2}));
    REQUIRE(p->getVariableNameAttributes() == std::unordered_set<std::string>({"b"}));

    REQUIRE(c->getStatementNumbers() == std::unordered_set<int>({3}));
    REQUIRE(c->getProcedureNameAttributes() == std::unordered_set<std::string>({"p1"}));

    REQUIRE(w->getStatementNumbers() == std::unordered_set<int>({4}));

    REQUIRE(i->getStatementNumbers() == std::unordered_set<int>({5}));

    REQUIRE(a->getStatementNumbers() == std::unordered_set<int>({6}));

    REQUIRE(var->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    REQUIRE(var->getVariableNameAttributes() == std::unordered_set<std::string>({"a", "b", "c"}));

    REQUIRE(con->getNames() == std::unordered_set<std::string>({"1", "2", "3"}));
    REQUIRE(con->getValueAttributes() == std::unordered_set<std::string>({"1", "2", "3"}));

    REQUIRE(proc->getNames() == std::unordered_set<std::string>({"main", "p1"}));
    REQUIRE(proc->getProcedureNameAttributes() == std::unordered_set<std::string>({"main", "p1"}));
}