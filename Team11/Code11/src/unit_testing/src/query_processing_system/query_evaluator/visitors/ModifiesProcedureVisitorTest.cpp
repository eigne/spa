#include "catch.hpp"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query_evaluator/storage_reader/IModifiesProcedureReader.h"
#include "query_processing_system/query/design_abstractions/such_that/modifies/ModifiesProcedure.h"

// a -> ab, b -> bc, c-> ac
class ModifiesProcedureReaderStub : public IModifiesProcedureReader {
    std::unordered_set<std::string> getVariablesModifiedInProcedure(std::string procedureName) override {
        if (procedureName == "a") {
            return {"a", "b"};
        } else if (procedureName == "b") {
            return {"b", "c"};
        } else {
            return {"a", "c"};
        }
    }

    std::unordered_set<std::string> getProceduresThatModifyVariable(std::string variableName) override {
        if (variableName == "a") {
            return {"a", "c"};
        } else if (variableName == "b") {
            return {"a", "b"};
        } else {
            return {"b", "c"};
        }
    }

    std::unordered_set<std::string> getProceduresThatModifyAnyVariable() override {
        return {"a", "b", "c"};
    }
};

TEST_CASE("Visit ModifiesProcedure Two Synonyms") {
    std::shared_ptr<IModifiesProcedureReader> modifiesReader = std::make_shared<ModifiesProcedureReaderStub>();

    std::shared_ptr<ProcedureSynonym> p = std::make_shared<ProcedureSynonym>("a");
    std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
    QuerySelection selection({p, v});
    std::list<std::string> ans;

    p->setNames({"a", "b", "c"});
    v->setNames({"a", "b", "c"});
    std::shared_ptr<ModifiesProcedure> modifies1 = std::make_shared<ModifiesProcedure>(p, v, false);
    REQUIRE(modifies1->evaluateModifiesProcedure(modifiesReader));
    REQUIRE(p->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a a", "a b", "b b", "b c", "c a", "c c"}));

    p->setNames({"a", "b", "c"});
    v->setNames({"a", "b", "c"});
    std::shared_ptr<ModifiesProcedure> modifies2 = std::make_shared<ModifiesProcedure>(p, v, true);
    REQUIRE(modifies2->evaluateModifiesProcedure(modifiesReader));
    REQUIRE(p->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a c", "b a", "c b"}));
}

TEST_CASE("Visit ModifiesProcedure One Synonym") {
    VariableWildcard::allPossibleVariables = {"a", "b", "c"};
    std::shared_ptr<IModifiesProcedureReader> modifiesReader = std::make_shared<ModifiesProcedureReaderStub>();

    std::shared_ptr<ProcedureSynonym> p = std::make_shared<ProcedureSynonym>("a");
    std::shared_ptr<VariableSynonym> v = std::make_shared<VariableSynonym>("v");
    std::shared_ptr<VariableName> v_a = std::make_shared<VariableName>("a");
    std::shared_ptr<VariableWildcard> v_w = std::make_shared<VariableWildcard>();
    std::shared_ptr<ProcedureName> p_a = std::make_shared<ProcedureName>("a");

    SECTION("Variable Name") {
        p->setNames({"a", "b", "c"});
        std::shared_ptr<ModifiesProcedure> modifies1 = std::make_shared<ModifiesProcedure>(p, v_a, false);
        REQUIRE(modifies1->evaluateModifiesProcedure(modifiesReader));
        REQUIRE(p->getNames() == std::unordered_set<std::string>({"a", "c"}));

        p->setNames({"a", "b", "c"});
        std::shared_ptr<ModifiesProcedure> modifies2 = std::make_shared<ModifiesProcedure>(p, v_a, true);
        REQUIRE(modifies2->evaluateModifiesProcedure(modifiesReader));
        REQUIRE(p->getNames() == std::unordered_set<std::string>({"b"}));
    }

    SECTION("Variable Wildcard") {
        p->setNames({"a", "b", "c"});
        std::shared_ptr<ModifiesProcedure> modifies1 = std::make_shared<ModifiesProcedure>(p, v_w, false);
        REQUIRE(modifies1->evaluateModifiesProcedure(modifiesReader));
        REQUIRE(p->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));

        p->setNames({"a", "b", "c"});
        std::shared_ptr<ModifiesProcedure> modifies2 = std::make_shared<ModifiesProcedure>(p, v_w, true);
        REQUIRE_FALSE(modifies2->evaluateModifiesProcedure(modifiesReader));
        REQUIRE(p->getNames().empty());

        p->setNames({"a", "b", "c", "d"});
        std::shared_ptr<ModifiesProcedure> modifies3 = std::make_shared<ModifiesProcedure>(p, v_w, true);
        REQUIRE(modifies3->evaluateModifiesProcedure(modifiesReader));
        REQUIRE(p->getNames() == std::unordered_set<std::string>({"d"}));
    }

    SECTION("Procedure Name, Variable Synonym") {
        v->setNames({"a", "b", "c"});
        std::shared_ptr<ModifiesProcedure> modifies1 = std::make_shared<ModifiesProcedure>(p_a, v, false);
        REQUIRE(modifies1->evaluateModifiesProcedure(modifiesReader));
        REQUIRE(v->getNames() == std::unordered_set<std::string>({"a", "b"}));

        v->setNames({"a", "b", "c"});
        std::shared_ptr<ModifiesProcedure> modifies2 = std::make_shared<ModifiesProcedure>(p_a, v, true);
        REQUIRE(modifies2->evaluateModifiesProcedure(modifiesReader));
        REQUIRE(v->getNames() == std::unordered_set<std::string>({"c"}));
    }
}

TEST_CASE("Visit ModifiesProcedure No Synonym") {
    VariableWildcard::allPossibleVariables = {"a", "b", "c"};
    std::shared_ptr<IModifiesProcedureReader> modifiesReader = std::make_shared<ModifiesProcedureReaderStub>();

    std::shared_ptr<VariableName> v_a = std::make_shared<VariableName>("a");
    std::shared_ptr<VariableWildcard> v_w = std::make_shared<VariableWildcard>();
    std::shared_ptr<ProcedureName> p_a = std::make_shared<ProcedureName>("a");

    SECTION("Variable Name") {
        std::shared_ptr<ModifiesProcedure> modifies1 = std::make_shared<ModifiesProcedure>(p_a, v_a, false);
        REQUIRE(modifies1->evaluateModifiesProcedure(modifiesReader));

        std::shared_ptr<ModifiesProcedure> modifies2 = std::make_shared<ModifiesProcedure>(p_a, v_a, true);
        REQUIRE_FALSE(modifies2->evaluateModifiesProcedure(modifiesReader));
    }

    SECTION("Variable Wildcard") {
        std::shared_ptr<ModifiesProcedure> modifies1 = std::make_shared<ModifiesProcedure>(p_a, v_w, false);
        REQUIRE(modifies1->evaluateModifiesProcedure(modifiesReader));

        std::shared_ptr<ModifiesProcedure> modifies2 = std::make_shared<ModifiesProcedure>(p_a, v_w, true);
        REQUIRE_FALSE(modifies2->evaluateModifiesProcedure(modifiesReader));
    }
}
