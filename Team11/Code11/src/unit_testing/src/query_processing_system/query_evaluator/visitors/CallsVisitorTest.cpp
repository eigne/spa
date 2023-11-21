#include "catch.hpp"
#include "query_processing_system/query/query_selection/QuerySelection.h"
#include "query_processing_system/query_evaluator/storage_reader/ICallsReader.h"
#include "query_processing_system/query/design_abstractions/such_that/calls/Calls.h"

// a->b, b->c,d, c->d
class CallsReaderStub : public ICallsReader {
    std::unordered_set<std::string> getProceduresCalledBy(std::string procedureName, bool onlyImmediate) override {
        if (procedureName == "c") {
            return {"d"};
        } else if (procedureName == "b") {
            return {"c", "d"};
        } else if (procedureName == "a") {
            if (onlyImmediate) {
                return {"b"};
            } else {
                return {"b", "c", "d"};
            }
        } else {
            return {};
        }
    }

    std::unordered_set<std::string> getProceduresCalling(std::string procedureName, bool onlyImmediate) override {
        if (procedureName == "b") {
            return {"a"};
        } else if (procedureName == "c") {
            if (onlyImmediate) {
                return {"b"};
            } else {
                return {"a", "b"};
            }
        } else if (procedureName == "d") {
            if (onlyImmediate) {
                return {"b", "c"};
            } else {
                return {"a", "b", "c"};
            }
        } else {
            return {};
        }
    }

    std::unordered_set<std::string> getAllCalledProcedures() override {
        return {"b", "c", "d"};
    }

    std::unordered_set<std::string> getProceduresThatCall() override {
        return {"a", "b", "c"};
    }
};

class NoCallsStub : public ICallsReader {
    std::unordered_set<std::string> getProceduresCalledBy(std::string procedureName, bool onlyImmediate) override {
        return {};
    }

    std::unordered_set<std::string> getProceduresCalling(std::string procedureName, bool onlyImmediate) override {
        return {};
    }

    std::unordered_set<std::string> getAllCalledProcedures() override {
        return {};
    }

    std::unordered_set<std::string> getProceduresThatCall() override {
        return {};
    }
};

TEST_CASE("Visit Calls Same Synonym") {
    std::shared_ptr<ICallsReader> callsReader = std::make_shared<CallsReaderStub>();

    std::shared_ptr<ProcedureSynonym> p1 = std::make_shared<ProcedureSynonym>("p1");
    p1->setNames({"a", "b", "c", "d"});

    std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p1, false, false);
    REQUIRE_FALSE(calls1->evaluateCall(callsReader));
    REQUIRE(p1->getNames().empty());

    p1->setNames({"a", "b", "c", "d"});
    std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p1, false, true);
    REQUIRE(calls2->evaluateCall(callsReader));
    REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c", "d"}));
}

TEST_CASE("Visit Calls Two Synonyms") {
    std::shared_ptr<ICallsReader> callsReader = std::make_shared<CallsReaderStub>();

    std::shared_ptr<ProcedureSynonym> p1 = std::make_shared<ProcedureSynonym>("p1");
    std::shared_ptr<ProcedureSynonym> p2 = std::make_shared<ProcedureSynonym>("p2");

    QuerySelection selection({p1, p2});
    std::list<std::string> ans;

    p1->setNames({"a", "b", "c", "d"});
    p2->setNames({"a", "b", "c", "d"});
    std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p2, false, false);
    REQUIRE(calls1->evaluateCall(callsReader));
    REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    REQUIRE(p2->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a b", "b c", "b d", "c d"}));

    p1->setNames({"a", "b", "c", "d"});
    p2->setNames({"a", "b", "c", "d"});
    std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p2, false, true);
    REQUIRE(calls2->evaluateCall(callsReader));
    REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c", "d"}));
    REQUIRE(p2->getNames() == std::unordered_set<std::string>({"a", "b", "c", "d"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a a", "a c", "a d",
                                           "b a", "b b",
                                           "c a", "c b", "c c",
                                           "d a", "d b", "d c", "d d"}));
}

TEST_CASE("Visit CallsStar Two Synonyms") {
    std::shared_ptr<ICallsReader> callsReader = std::make_shared<CallsReaderStub>();

    std::shared_ptr<ProcedureSynonym> p1 = std::make_shared<ProcedureSynonym>("p1");
    std::shared_ptr<ProcedureSynonym> p2 = std::make_shared<ProcedureSynonym>("p2");

    QuerySelection selection({p1, p2});
    std::list<std::string> ans;

    p1->setNames({"a", "b", "c", "d"});
    p2->setNames({"a", "b", "c", "d"});
    std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p2, true, false);
    REQUIRE(calls1->evaluateCall(callsReader));
    REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    REQUIRE(p2->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a b", "a c", "a d", "b c", "b d", "c d"}));

    p1->setNames({"a", "b", "c", "d"});
    p2->setNames({"a", "b", "c", "d"});
    std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p2, true, true);
    REQUIRE(calls2->evaluateCall(callsReader));
    REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c", "d"}));
    REQUIRE(p2->getNames() == std::unordered_set<std::string>({"a", "b", "c", "d"}));
    ans = selection.getAnswer(true);
    ans.sort();
    REQUIRE(ans == std::list<std::string>({"a a", "b a", "b b", "c a", "c b", "c c", "d a", "d b", "d c", "d d"}));
}

TEST_CASE("Visit Calls One Synonym") {
    std::shared_ptr<ICallsReader> callsReader = std::make_shared<CallsReaderStub>();
    ProcedureWildcard::allPossibleProcedures = {"a", "b", "c", "d"};

    std::shared_ptr<ProcedureSynonym> p1 = std::make_shared<ProcedureSynonym>("p1");
    std::shared_ptr<ProcedureName> p_a = std::make_shared<ProcedureName>("a");
    std::shared_ptr<ProcedureName> p_b = std::make_shared<ProcedureName>("b");
    std::shared_ptr<ProcedureName> p_c = std::make_shared<ProcedureName>("c");
    std::shared_ptr<ProcedureName> p_d = std::make_shared<ProcedureName>("d");
    std::shared_ptr<ProcedureWildcard> p = std::make_shared<ProcedureWildcard>();

    SECTION("Procedure a") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p_a, false, false);
        REQUIRE_FALSE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames().empty());

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p_a, false, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p_a, p1, false, false);
        REQUIRE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"b"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p_a, p1, false, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "c", "d"}));
    }

    SECTION("Procedure b") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p_b, false, false);
        REQUIRE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p_b, false, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p_b, p1, false, false);
        REQUIRE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p_b, p1, false, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b"}));
    }

    SECTION("Procedure c") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p_c, false, false);
        REQUIRE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"b"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p_c, false, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p_c, p1, false, false);
        REQUIRE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p_c, p1, false, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    }

    SECTION("Procedure d") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p_d, false, false);
        REQUIRE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"b", "c"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p_d, false, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p_d, p1, false, false);
        REQUIRE_FALSE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames().empty());

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p_d, p1, false, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c", "d"}));
    }

    SECTION("Wildcard") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p, false, false);
        REQUIRE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p, false, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p, p1, false, false);
        REQUIRE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p, p1, false, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a"}));
    }
}

TEST_CASE("Visit CallsStar One Synonym") {
    std::shared_ptr<ICallsReader> callsReader = std::make_shared<CallsReaderStub>();
    ProcedureWildcard::allPossibleProcedures = {"a", "b", "c", "d"};

    std::shared_ptr<ProcedureSynonym> p1 = std::make_shared<ProcedureSynonym>("p1");
    std::shared_ptr<ProcedureName> p_a = std::make_shared<ProcedureName>("a");
    std::shared_ptr<ProcedureName> p_b = std::make_shared<ProcedureName>("b");
    std::shared_ptr<ProcedureName> p_c = std::make_shared<ProcedureName>("c");
    std::shared_ptr<ProcedureName> p_d = std::make_shared<ProcedureName>("d");
    std::shared_ptr<ProcedureWildcard> p = std::make_shared<ProcedureWildcard>();

    SECTION("Procedure a") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p_a, true, false);
        REQUIRE_FALSE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames().empty());

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p_a, true, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p_a, p1, true, false);
        REQUIRE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p_a, p1, true, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a"}));
    }

    SECTION("Procedure b") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p_b, true, false);
        REQUIRE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p_b, true, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p_b, p1, true, false);
        REQUIRE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p_b, p1, true, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b"}));
    }

    SECTION("Procedure c") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p_c, true, false);
        REQUIRE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p_c, true, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p_c, p1, true, false);
        REQUIRE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p_c, p1, true, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));
    }

    SECTION("Procedure d") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p_d, true, false);
        REQUIRE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p_d, true, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p_d, p1, true, false);
        REQUIRE_FALSE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames().empty());

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p_d, p1, true, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c", "d"}));
    }

    SECTION("Wildcard") {
        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p, true, false);
        REQUIRE(calls1->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a", "b", "c"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p, true, true);
        REQUIRE(calls2->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p, p1, true, false);
        REQUIRE(calls3->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"b", "c", "d"}));

        p1->setNames({"a", "b", "c", "d"});
        std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p, p1, true, true);
        REQUIRE(calls4->evaluateCall(callsReader));
        REQUIRE(p1->getNames() == std::unordered_set<std::string>({"a"}));
    }
}

TEST_CASE("Visit Calls No Synonym") {
    std::shared_ptr<ICallsReader> callsReader = std::make_shared<CallsReaderStub>();
    ProcedureWildcard::allPossibleProcedures = {"a", "b", "c", "d"};

    std::shared_ptr<ProcedureWildcard> p_w1 = std::make_shared<ProcedureWildcard>();
    std::shared_ptr<ProcedureWildcard> p_w2 = std::make_shared<ProcedureWildcard>();
    std::shared_ptr<ProcedureName> p_a = std::make_shared<ProcedureName>("a");
    std::shared_ptr<ProcedureName> p_b = std::make_shared<ProcedureName>("b");
    std::shared_ptr<ProcedureName> p_c = std::make_shared<ProcedureName>("c");
    std::shared_ptr<ProcedureName> p_d = std::make_shared<ProcedureName>("d");

    SECTION("Two Wildcards") {
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p_w1, p_w2, false, false);
        REQUIRE(calls1->evaluateCall(callsReader));

        // unintuitive, but Calls("a", "a") fulfils this
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p_w1, p_w2, false, true);
        REQUIRE_FALSE(calls2->evaluateCall(callsReader));
    }

    SECTION("One Wildcard") {
        std::shared_ptr<Calls> calls_wa = std::make_shared<Calls>(p_w1, p_a, false, false);
        REQUIRE_FALSE(calls_wa->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_wan = std::make_shared<Calls>(p_w1, p_a, false, true);
        REQUIRE(calls_wan->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_aw = std::make_shared<Calls>(p_a, p_w1, false, false);
        REQUIRE(calls_aw->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_awn = std::make_shared<Calls>(p_a, p_w1, false, true);
        REQUIRE_FALSE(calls_awn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_wb = std::make_shared<Calls>(p_w1, p_b, false, false);
        REQUIRE(calls_wb->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_wbn = std::make_shared<Calls>(p_w1, p_b, false, true);
        REQUIRE_FALSE(calls_wbn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_bw = std::make_shared<Calls>(p_b, p_w1, false, false);
        REQUIRE(calls_bw->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_bwn = std::make_shared<Calls>(p_b, p_w1, false, true);
        REQUIRE_FALSE(calls_bwn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_wc = std::make_shared<Calls>(p_w1, p_c, false, false);
        REQUIRE(calls_wc->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_wcn = std::make_shared<Calls>(p_w1, p_c, false, true);
        REQUIRE_FALSE(calls_wcn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_cw = std::make_shared<Calls>(p_c, p_w1, false, false);
        REQUIRE(calls_cw->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_cwn = std::make_shared<Calls>(p_c, p_w1, false, true);
        REQUIRE_FALSE(calls_cwn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_wd = std::make_shared<Calls>(p_w1, p_d, false, false);
        REQUIRE(calls_wd->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_wdn = std::make_shared<Calls>(p_w1, p_d, false, true);
        REQUIRE_FALSE(calls_wdn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_dw = std::make_shared<Calls>(p_d, p_w1, false, false);
        REQUIRE_FALSE(calls_dw->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_dwn = std::make_shared<Calls>(p_d, p_w1, false, true);
        REQUIRE(calls_dwn->evaluateCall(callsReader));
    }

    SECTION("No Wildcard") {
        std::shared_ptr<Calls> calls_aa = std::make_shared<Calls>(p_a, p_a, false, false);
        REQUIRE_FALSE(calls_aa->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_aan = std::make_shared<Calls>(p_a, p_a, false, true);
        REQUIRE(calls_aan->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ab = std::make_shared<Calls>(p_a, p_b, false, false);
        REQUIRE(calls_ab->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_abn = std::make_shared<Calls>(p_a, p_b, false, true);
        REQUIRE_FALSE(calls_abn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ac = std::make_shared<Calls>(p_a, p_c, false, false);
        REQUIRE_FALSE(calls_ac->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_acn = std::make_shared<Calls>(p_a, p_c, false, true);
        REQUIRE(calls_acn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ad = std::make_shared<Calls>(p_a, p_d, false, false);
        REQUIRE_FALSE(calls_ad->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_adn = std::make_shared<Calls>(p_a, p_d, false, true);
        REQUIRE(calls_adn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ba = std::make_shared<Calls>(p_b, p_a, false, false);
        REQUIRE_FALSE(calls_ba->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_ban = std::make_shared<Calls>(p_b, p_a, false, true);
        REQUIRE(calls_ban->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_bb = std::make_shared<Calls>(p_b, p_b, false, false);
        REQUIRE_FALSE(calls_bb->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_bbn = std::make_shared<Calls>(p_b, p_b, false, true);
        REQUIRE(calls_bbn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_bc = std::make_shared<Calls>(p_b, p_c, false, false);
        REQUIRE(calls_bc->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_bcn = std::make_shared<Calls>(p_b, p_c, false, true);
        REQUIRE_FALSE(calls_bcn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_bd = std::make_shared<Calls>(p_b, p_d, false, false);
        REQUIRE(calls_bd->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_bdn = std::make_shared<Calls>(p_b, p_d, false, true);
        REQUIRE_FALSE(calls_bdn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ca = std::make_shared<Calls>(p_c, p_a, false, false);
        REQUIRE_FALSE(calls_ca->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_can = std::make_shared<Calls>(p_c, p_a, false, true);
        REQUIRE(calls_can->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_cb = std::make_shared<Calls>(p_c, p_b, false, false);
        REQUIRE_FALSE(calls_cb->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_cbn = std::make_shared<Calls>(p_c, p_b, false, true);
        REQUIRE(calls_cbn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_cc = std::make_shared<Calls>(p_c, p_c, false, false);
        REQUIRE_FALSE(calls_cc->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_ccn = std::make_shared<Calls>(p_c, p_c, false, true);
        REQUIRE(calls_ccn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_cd = std::make_shared<Calls>(p_c, p_d, false, false);
        REQUIRE(calls_cd->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_cdn = std::make_shared<Calls>(p_c, p_d, false, true);
        REQUIRE_FALSE(calls_cdn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_da = std::make_shared<Calls>(p_d, p_a, false, false);
        REQUIRE_FALSE(calls_da->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_dan = std::make_shared<Calls>(p_d, p_a, false, true);
        REQUIRE(calls_dan->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_db = std::make_shared<Calls>(p_d, p_b, false, false);
        REQUIRE_FALSE(calls_db->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_dbn = std::make_shared<Calls>(p_d, p_b, false, true);
        REQUIRE(calls_dbn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_dc = std::make_shared<Calls>(p_d, p_c, false, false);
        REQUIRE_FALSE(calls_dc->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_dcn = std::make_shared<Calls>(p_d, p_c, false, true);
        REQUIRE(calls_dcn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_dd = std::make_shared<Calls>(p_d, p_d, false, false);
        REQUIRE_FALSE(calls_dd->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_ddn = std::make_shared<Calls>(p_d, p_d, false, true);
        REQUIRE(calls_ddn->evaluateCall(callsReader));
    }
}

TEST_CASE("Visit CallsStar No Synonym") {
    std::shared_ptr<ICallsReader> callsReader = std::make_shared<CallsReaderStub>();
    ProcedureWildcard::allPossibleProcedures = {"a", "b", "c", "d"};

    std::shared_ptr<ProcedureWildcard> p_w1 = std::make_shared<ProcedureWildcard>();
    std::shared_ptr<ProcedureWildcard> p_w2 = std::make_shared<ProcedureWildcard>();
    std::shared_ptr<ProcedureName> p_a = std::make_shared<ProcedureName>("a");
    std::shared_ptr<ProcedureName> p_b = std::make_shared<ProcedureName>("b");
    std::shared_ptr<ProcedureName> p_c = std::make_shared<ProcedureName>("c");
    std::shared_ptr<ProcedureName> p_d = std::make_shared<ProcedureName>("d");

    SECTION("Two Wildcards") {
        std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p_w1, p_w2, true, false);
        REQUIRE(calls1->evaluateCall(callsReader));

        // unintuitive, but Calls("a", "a") fulfils this
        std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p_w1, p_w2, true, true);
        REQUIRE_FALSE(calls2->evaluateCall(callsReader));
    }

    SECTION("One Wildcard") {
        std::shared_ptr<Calls> calls_wa = std::make_shared<Calls>(p_w1, p_a, true, false);
        REQUIRE_FALSE(calls_wa->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_wan = std::make_shared<Calls>(p_w1, p_a, true, true);
        REQUIRE(calls_wan->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_aw = std::make_shared<Calls>(p_a, p_w1, true, false);
        REQUIRE(calls_aw->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_awn = std::make_shared<Calls>(p_a, p_w1, true, true);
        REQUIRE_FALSE(calls_awn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_wb = std::make_shared<Calls>(p_w1, p_b, true, false);
        REQUIRE(calls_wb->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_wbn = std::make_shared<Calls>(p_w1, p_b, true, true);
        REQUIRE_FALSE(calls_wbn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_bw = std::make_shared<Calls>(p_b, p_w1, true, false);
        REQUIRE(calls_bw->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_bwn = std::make_shared<Calls>(p_b, p_w1, true, true);
        REQUIRE_FALSE(calls_bwn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_wc = std::make_shared<Calls>(p_w1, p_c, true, false);
        REQUIRE(calls_wc->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_wcn = std::make_shared<Calls>(p_w1, p_c, true, true);
        REQUIRE_FALSE(calls_wcn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_cw = std::make_shared<Calls>(p_c, p_w1, true, false);
        REQUIRE(calls_cw->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_cwn = std::make_shared<Calls>(p_c, p_w1, true, true);
        REQUIRE_FALSE(calls_cwn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_wd = std::make_shared<Calls>(p_w1, p_d, true, false);
        REQUIRE(calls_wd->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_wdn = std::make_shared<Calls>(p_w1, p_d, true, true);
        REQUIRE_FALSE(calls_wdn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_dw = std::make_shared<Calls>(p_d, p_w1, true, false);
        REQUIRE_FALSE(calls_dw->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_dwn = std::make_shared<Calls>(p_d, p_w1, true, true);
        REQUIRE(calls_dwn->evaluateCall(callsReader));
    }

    SECTION("No Wildcard") {
        std::shared_ptr<Calls> calls_aa = std::make_shared<Calls>(p_a, p_a, true, false);
        REQUIRE_FALSE(calls_aa->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_aan = std::make_shared<Calls>(p_a, p_a, true, true);
        REQUIRE(calls_aan->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ab = std::make_shared<Calls>(p_a, p_b, true, false);
        REQUIRE(calls_ab->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_abn = std::make_shared<Calls>(p_a, p_b, true, true);
        REQUIRE_FALSE(calls_abn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ac = std::make_shared<Calls>(p_a, p_c, true, false);
        REQUIRE(calls_ac->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_acn = std::make_shared<Calls>(p_a, p_c, true, true);
        REQUIRE_FALSE(calls_acn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ad = std::make_shared<Calls>(p_a, p_d, true, false);
        REQUIRE(calls_ad->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_adn = std::make_shared<Calls>(p_a, p_d, true, true);
        REQUIRE_FALSE(calls_adn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ba = std::make_shared<Calls>(p_b, p_a, true, false);
        REQUIRE_FALSE(calls_ba->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_ban = std::make_shared<Calls>(p_b, p_a, true, true);
        REQUIRE(calls_ban->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_bb = std::make_shared<Calls>(p_b, p_b, true, false);
        REQUIRE_FALSE(calls_bb->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_bbn = std::make_shared<Calls>(p_b, p_b, true, true);
        REQUIRE(calls_bbn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_bc = std::make_shared<Calls>(p_b, p_c, true, false);
        REQUIRE(calls_bc->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_bcn = std::make_shared<Calls>(p_b, p_c, true, true);
        REQUIRE_FALSE(calls_bcn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_bd = std::make_shared<Calls>(p_b, p_d, true, false);
        REQUIRE(calls_bd->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_bdn = std::make_shared<Calls>(p_b, p_d, true, true);
        REQUIRE_FALSE(calls_bdn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_ca = std::make_shared<Calls>(p_c, p_a, true, false);
        REQUIRE_FALSE(calls_ca->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_can = std::make_shared<Calls>(p_c, p_a, true, true);
        REQUIRE(calls_can->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_cb = std::make_shared<Calls>(p_c, p_b, true, false);
        REQUIRE_FALSE(calls_cb->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_cbn = std::make_shared<Calls>(p_c, p_b, true, true);
        REQUIRE(calls_cbn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_cc = std::make_shared<Calls>(p_c, p_c, true, false);
        REQUIRE_FALSE(calls_cc->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_ccn = std::make_shared<Calls>(p_c, p_c, true, true);
        REQUIRE(calls_ccn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_cd = std::make_shared<Calls>(p_c, p_d, true, false);
        REQUIRE(calls_cd->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_cdn = std::make_shared<Calls>(p_c, p_d, true, true);
        REQUIRE_FALSE(calls_cdn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_da = std::make_shared<Calls>(p_d, p_a, true, false);
        REQUIRE_FALSE(calls_da->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_dan = std::make_shared<Calls>(p_d, p_a, true, true);
        REQUIRE(calls_dan->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_db = std::make_shared<Calls>(p_d, p_b, true, false);
        REQUIRE_FALSE(calls_db->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_dbn = std::make_shared<Calls>(p_d, p_b, true, true);
        REQUIRE(calls_dbn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_dc = std::make_shared<Calls>(p_d, p_c, true, false);
        REQUIRE_FALSE(calls_dc->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_dcn = std::make_shared<Calls>(p_d, p_c, true, true);
        REQUIRE(calls_dcn->evaluateCall(callsReader));

        std::shared_ptr<Calls> calls_dd = std::make_shared<Calls>(p_d, p_d, true, false);
        REQUIRE_FALSE(calls_dd->evaluateCall(callsReader));
        std::shared_ptr<Calls> calls_ddn = std::make_shared<Calls>(p_d, p_d, true, true);
        REQUIRE(calls_ddn->evaluateCall(callsReader));
    }
}

TEST_CASE("No Calls") {
    std::shared_ptr<ICallsReader> callsReader = std::make_shared<NoCallsStub>();
    ProcedureWildcard::allPossibleProcedures = {"main"};

    std::shared_ptr<ProcedureWildcard> p1 = std::make_shared<ProcedureWildcard>();
    std::shared_ptr<ProcedureWildcard> p2 = std::make_shared<ProcedureWildcard>();

    std::shared_ptr<Calls> calls1 = std::make_shared<Calls>(p1, p2, false, false);
    REQUIRE_FALSE(calls1->evaluateCall(callsReader));

    std::shared_ptr<Calls> calls2 = std::make_shared<Calls>(p1, p2, true, false);
    REQUIRE_FALSE(calls2->evaluateCall(callsReader));

    std::shared_ptr<Calls> calls3 = std::make_shared<Calls>(p1, p2, false, true);
    REQUIRE(calls3->evaluateCall(callsReader));

    std::shared_ptr<Calls> calls4 = std::make_shared<Calls>(p1, p2, true, true);
    REQUIRE(calls4->evaluateCall(callsReader));
}
