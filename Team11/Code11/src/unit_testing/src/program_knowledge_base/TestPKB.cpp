#include <unordered_set>
#include "catch.hpp"
#include "program_knowledge_base/data_structures/DoubleMap.h"
#include "program_knowledge_base/data_structures/Set.h"
#include "commons/ExpressionParser.h"
#include "program_knowledge_base/PKB.h"

/**   program_knowledge_base IMPLEMENTATION TESTS
      program_knowledge_base/DoubleMap tests        **/
TEST_CASE("program_knowledge_base DoubleMap insertForward test") {
    DoubleMap<int, std::string> doubleMap;
    doubleMap.insertForward(1, "read");
    REQUIRE(doubleMap.lookupForward(1) == std::unordered_set<std::string>{"read"});
    REQUIRE(doubleMap.lookupBackward("read").empty());
}

TEST_CASE("program_knowledge_base DoubleMap insertForward multiple test") {
    DoubleMap<int, std::string> doubleMap;
    doubleMap.insertForward(1, "read");
    doubleMap.insertForward(1, "print");
    REQUIRE(doubleMap.lookupForward(1) == std::unordered_set<std::string>{ "read", "print" });
    REQUIRE(doubleMap.lookupBackward("read").empty());
    REQUIRE(doubleMap.lookupBackward("print").empty());
}

TEST_CASE("program_knowledge_base DoubleMap insertForwardHeadAndBody test") {
    DoubleMap<int, std::string, HeadContainer> doubleMap;
    doubleMap.insertForwardHeadAndBody(1, "hello");
    REQUIRE(doubleMap.lookupForward(1).getHead() ==  std::unordered_set<std::string>{"hello"});
    REQUIRE(doubleMap.lookupForward(1).getBody() ==  std::unordered_set<std::string>{"hello"});
    REQUIRE(doubleMap.lookupBackward("hello").empty());
}

TEST_CASE("program_knowledge_base DoubleMap insertForwardHeadAndBody multiple test") {
    DoubleMap<int, std::string, HeadContainer> doubleMap;
    doubleMap.insertForwardHeadAndBody(1, "hello");
    doubleMap.insertForwardHeadAndBody(1, "world");
    doubleMap.insertForwardHeadAndBody(1, "hello");
    doubleMap.insertForwardHeadAndBody(2, "hello");
    REQUIRE(doubleMap.lookupForward(1).getHead() ==  std::unordered_set<std::string>{"hello", "world"});
    REQUIRE(doubleMap.lookupForward(1).getBody() ==  std::unordered_set<std::string>{"hello", "world"});
    REQUIRE(doubleMap.lookupForward(2).getHead() ==  std::unordered_set<std::string>{"hello"});
    REQUIRE(doubleMap.lookupForward(2).getBody() ==  std::unordered_set<std::string>{"hello"});
}

TEST_CASE("program_knowledge_base DoubleMap insertBackward test") {
    DoubleMap<int, std::string> doubleMap;
    doubleMap.insertBackward("read", 1);
    REQUIRE(doubleMap.lookupBackward("read") == std::unordered_set<int>{ 1 });
}

TEST_CASE("program_knowledge_base DoubleMap insertBackward multiple test") {
    DoubleMap<int, std::string> doubleMap;
    doubleMap.insertBackward("read", 1);
    doubleMap.insertBackward("read", 2);
    REQUIRE(doubleMap.lookupBackward("read") == std::unordered_set<int>{ 1, 2 });
}

TEST_CASE("program_knowledge_base DoubleMap insertBackwardHeadAndBody test") {
    DoubleMap<int, std::string, HeadContainer> doubleMap;
    doubleMap.insertBackwardHeadAndBody("hello", 1);
    REQUIRE(doubleMap.lookupBackward("hello").getHead() ==  std::unordered_set<int>{1});
    REQUIRE(doubleMap.lookupBackward("hello").getBody() ==  std::unordered_set<int>{1});
}

TEST_CASE("program_knowledge_base DoubleMap insertBackwardHeadAndBody multiple test") {
    DoubleMap<int, std::string, HeadContainer> doubleMap;
    doubleMap.insertBackwardHeadAndBody("hello", 1);
    doubleMap.insertBackwardHeadAndBody("world", 1);
    doubleMap.insertBackwardHeadAndBody("hello", 1);
    doubleMap.insertBackwardHeadAndBody("hello", 2);
    REQUIRE(doubleMap.lookupBackward("hello").getHead() == std::unordered_set<int>{1, 2});
    REQUIRE(doubleMap.lookupBackward("hello").getBody() == std::unordered_set<int>{1, 2});
    REQUIRE(doubleMap.lookupBackward("world").getHead() == std::unordered_set<int>{1});
    REQUIRE(doubleMap.lookupBackward("world").getBody() == std::unordered_set<int>{1});
}
TEST_CASE("program_knowledge_base DoubleMap insertBoth test") {
    DoubleMap<int, std::string> doubleMap;
    doubleMap.insertBoth(1, "read");
    REQUIRE(doubleMap.lookupForward(1) == std::unordered_set<std::string>{ "read" });
    REQUIRE(doubleMap.lookupBackward("read") == std::unordered_set<int>{ 1 });
}

TEST_CASE("program_knowledge_base DoubleMap insertBoth multiple test") {
    DoubleMap<int, std::string> doubleMap;
    doubleMap.insertBoth(1, "read");
    doubleMap.insertBoth(2, "read");
    doubleMap.insertBoth(3, "print");
    REQUIRE(doubleMap.lookupForward(1) == std::unordered_set<std::string>{ "read" });
    REQUIRE(doubleMap.lookupBackward("read") == std::unordered_set<int>{1, 2});
    REQUIRE(doubleMap.lookupForward(3) == std::unordered_set<std::string>{ "print" });
    REQUIRE(doubleMap.lookupBackward("print") == std::unordered_set<int>{ 3 });
}

TEST_CASE("program_knowledge_base DoubleMap insertEmpty test") {
    DoubleMap<int, int> doubleMap;
    doubleMap.insertEmpty(1);
    REQUIRE(doubleMap.lookupForward(1).empty());
    REQUIRE(doubleMap.lookupBackward(2).empty());
    REQUIRE(doubleMap.getAllForwardKeys() == std::unordered_set<int>{1});
    REQUIRE(doubleMap.getAllBackwardKeys().empty());
}

TEST_CASE("program_knowledge_base DoubleMap insertBothHeadAndBody test") {
    DoubleMap<int, int, HeadContainer> doubleMap;
    doubleMap.insertBothHeadAndBody(1, 2);
    REQUIRE(doubleMap.lookupForward(1).getHead() == std::unordered_set<int>{2});
    REQUIRE(doubleMap.lookupForward(1).getHead() == std::unordered_set<int>{2});
    REQUIRE(doubleMap.lookupBackward(1).getBody().empty());
    REQUIRE(doubleMap.lookupBackward(1).getBody().empty());
    REQUIRE(doubleMap.lookupForward(2).getHead().empty());
    REQUIRE(doubleMap.lookupForward(2).getHead().empty());
    REQUIRE(doubleMap.lookupBackward(2).getBody() == std::unordered_set<int>{1});
    REQUIRE(doubleMap.lookupBackward(2).getBody() == std::unordered_set<int>{1});
}

TEST_CASE("program_knowledge_base DoubleMap lookupForward multiple test") {
    DoubleMap<int, int> doubleMap;
    doubleMap.insertBoth(1, 2);
    doubleMap.insertBoth(3, 4);
    doubleMap.insertBoth(5, 6);

    REQUIRE(doubleMap.lookupForward(1) == std::unordered_set<int>{2});
    REQUIRE(doubleMap.lookupForward(2).empty());
    REQUIRE(doubleMap.lookupForward(3) == std::unordered_set<int>{4});
    REQUIRE(doubleMap.lookupForward(4).empty());
    REQUIRE(doubleMap.lookupForward(5) == std::unordered_set<int>{6});
    REQUIRE(doubleMap.lookupForward(6).empty());
}

TEST_CASE("program_knowledge_base DoubleMap lookupBackward multiple test") {
    DoubleMap<int, int> doubleMap;
    doubleMap.insertBoth(1, 2);
    doubleMap.insertBoth(3, 4);
    doubleMap.insertBoth(5, 6);

    REQUIRE(doubleMap.lookupBackward(1).empty());
    REQUIRE(doubleMap.lookupBackward(2) == std::unordered_set<int>{1});
    REQUIRE(doubleMap.lookupBackward(3).empty());
    REQUIRE(doubleMap.lookupBackward(4) == std::unordered_set<int>{3});
    REQUIRE(doubleMap.lookupBackward(5).empty());
    REQUIRE(doubleMap.lookupBackward(6) == std::unordered_set<int>{5});
}

TEST_CASE("program_knowledge_base DoubleMap getAllForwardKeys multiple test") {
    DoubleMap<int, int> doubleMap;
    doubleMap.insertBoth(1, 2);
    doubleMap.insertBoth(2, 3);
    doubleMap.insertBoth(3, 4);

    REQUIRE(doubleMap.getAllForwardKeys() == std::unordered_set<int>{1, 2, 3});
}

TEST_CASE("program_knowledge_base DoubleMap getAllBackwardKeys multiple test") {
    DoubleMap<int, int> doubleMap;
    doubleMap.insertBoth(1, 2);
    doubleMap.insertBoth(2, 3);
    doubleMap.insertBoth(3, 4);

    REQUIRE(doubleMap.getAllBackwardKeys() == std::unordered_set<int>{2, 3, 4});
}

TEST_CASE("program_knowledge_base empty test") {
    DoubleMap<int, int> doubleMap;
    REQUIRE(doubleMap.empty());
}

/**   program_knowledge_base IMPLEMENTATION TESTS
      program_knowledge_base/Set tests              **/

TEST_CASE("program_knowledge_base Set insertElement test") {
    Set<int> set;
    set.insertElement(1);
    REQUIRE(set.findElement(1) == true);
}

TEST_CASE("program_knowledge_base Set findElement test") {
    Set<int> set;
    REQUIRE(set.findElement(1) == false);
    set.insertElement(1);
    REQUIRE(set.findElement(1) == true);
}

TEST_CASE("program_knowledge_base Set removeElement test") {
    Set<int> set;
    set.insertElement(1);
    REQUIRE(set.findElement(1) == true);
    set.removeElement(1);
    REQUIRE(set.findElement(1) == false);
}

TEST_CASE("program_knowledge_base Set replaceElement test") {
    Set<int> set;
    set.insertElement(1);
    REQUIRE(set.findElement(1) == true);
    set.replaceElement(1, 2);
    REQUIRE(set.findElement(1) == false);
    REQUIRE(set.findElement(2) == true);
}

TEST_CASE("program_knowledge_base PKB isSubarray test") {
    PKB pkb;
    std::vector<std::string> array{ "x", "b", "+", "c", "+" };
    std::vector<std::string> subArray1{ "b", "+" };
    std::vector<std::string> subArray2{ "x", "b" };
    std::vector<std::string> subArray3{ "b", "+", "c", "+" };
    std::vector<std::string> subArray4{ "x", "b", "+", "c", "+" };
    std::vector<std::string> subArray5{ };
    std::vector<std::string> subArray6{ "x", "b", "+", "c", "+", "x" };
    std::vector<std::string> subArray7{ "b", "x" };
    std::vector<std::string> subArray8{ "x", "b", "c", "+", "x", "+" };

    REQUIRE(pkb.isSubarray(array, subArray1, false));
    REQUIRE(pkb.isSubarray(array, subArray2, false));
    REQUIRE(pkb.isSubarray(array, subArray3, false));
    REQUIRE(pkb.isSubarray(array, subArray4, false));
    REQUIRE(pkb.isSubarray(array, subArray5, false));
    REQUIRE(!pkb.isSubarray(array, subArray6, false));
    REQUIRE(!pkb.isSubarray(array, subArray7, false));
    REQUIRE(!pkb.isSubarray(array, subArray8, false));
    REQUIRE(!pkb.isSubarray(array, subArray1, true));
    REQUIRE(!pkb.isSubarray(array, subArray2, true));
    REQUIRE(!pkb.isSubarray(array, subArray3, true));
    REQUIRE(pkb.isSubarray(array, subArray4, true));
    REQUIRE(!pkb.isSubarray(array, subArray5, true));
    REQUIRE(!pkb.isSubarray(array, subArray6, true));
    REQUIRE(!pkb.isSubarray(array, subArray7, true));
    REQUIRE(!pkb.isSubarray(array, subArray8, true));
}

/**   program_knowledge_base IMPLEMENTATION TESTS
      program_knowledge_base/HeadContainer tests              **/

TEST_CASE("program_knowledge_base HeadContainer getHead test") {
    HeadContainer<int> container;
    container.insertHeadAndBody(1);
    REQUIRE(container.getHead() == std::unordered_set<int>{1});
}

TEST_CASE("program_knowledge_base HeadContainer getBody test") {
    HeadContainer<int> container;
    container.insertHeadAndBody(1);
    container.insert(2);
    REQUIRE(container.getHead() == std::unordered_set<int>{1});
    REQUIRE(container.getBody() == std::unordered_set<int>{1, 2});
}

TEST_CASE("program_knowledge_base HeadContainer empty test") {
    HeadContainer<int> container;
    REQUIRE(container.empty());
}