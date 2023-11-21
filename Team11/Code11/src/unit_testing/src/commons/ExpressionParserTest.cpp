#include <string>
#include <vector>
#include <algorithm>
#include "commons/ExpressionParser.h"
#include "catch.hpp"

TEST_CASE("Empty expression") {
    auto res = ExpressionParser::convertToPostfix("");

    REQUIRE(res.empty());
}

TEST_CASE("Single term") {
    auto res = ExpressionParser::convertToPostfix("a");
    std::vector<std::string> expectedResults{ "a" };

    REQUIRE(res == expectedResults);
}

TEST_CASE("Simple math expression") {
	std::string mathExpression = "1+2";
	auto res = ExpressionParser::convertToPostfix(mathExpression);
	std::vector<std::string> expectedResults{ "1", "2", "+" };

	REQUIRE(res == expectedResults);
}

TEST_CASE("Simple math expression with spaces") {
	std::string mathExpression = "1 + 2";
	auto res = ExpressionParser::convertToPostfix(mathExpression);
	std::vector<std::string> expectedResults{"1", "2", "+"};

	REQUIRE(res == expectedResults);
}

TEST_CASE("Simple math expression with spaces and brackets") {
	std::string mathExpression = "(1 + 2)";
	auto res = ExpressionParser::convertToPostfix(mathExpression);
	std::vector<std::string> expectedResults{"1", "2", "+"};

	REQUIRE(res == expectedResults);
}

TEST_CASE("Math expression with spaces and brackets and multiplication") {
	std::string mathExpression = "(1 + 2) * 3";
	auto res = ExpressionParser::convertToPostfix(mathExpression);
	std::vector<std::string> expectedResults{"1", "2", "+", "3", "*"};

	REQUIRE(res == expectedResults);
}

TEST_CASE("Math expression with spaces and brackets and multiplication and division") {
	std::string mathExpression = "(1 + 2) * 3 / 4";
	auto res = ExpressionParser::convertToPostfix(mathExpression);
	std::vector<std::string> expectedResults{"1", "2", "+", "3", "*", "4", "/"};

	REQUIRE(res == expectedResults);
}

TEST_CASE("Math expression with spaces and brackets and multiplication and division and modulo") {
	std::string mathExpression = "(1 + 2) * 3 / 4 % 5";
	auto res = ExpressionParser::convertToPostfix(mathExpression);
	std::vector<std::string> expectedResults{"1", "2", "+", "3", "*", "4", "/", "5", "%"};

	REQUIRE(res == expectedResults);
}

TEST_CASE("Complex math expressions") {
	std::string mathExpression1 = "(8 + 3 * 2 - 1) / 2 % 3";
	std::string mathExpression2 = "((15 / 3 * 2) - 2 + 1) % 5";
	std::string mathExpression3 = "(10 * 3 + 2 / 4 - 5) % 7";

	auto res1 = ExpressionParser::convertToPostfix(mathExpression1);
	auto res2 = ExpressionParser::convertToPostfix(mathExpression2);
	auto res3 = ExpressionParser::convertToPostfix(mathExpression3);

	std::vector<std::string> expectedResults1{"8", "3", "2", "*", "+", "1", "-", "2", "/", "3", "%"};
	std::vector<std::string> expectedResults2{"15", "3", "/", "2", "*", "2", "-", "1", "+", "5", "%"};
	std::vector<std::string> expectedResults3{"10", "3", "*", "2", "4", "/", "+", "5", "-", "7", "%"};

	REQUIRE(res1 == expectedResults1);
	REQUIRE(res2 == expectedResults2);
	REQUIRE(res3 == expectedResults3);
}

TEST_CASE("Complex math expression with mix of variables and constants") {
	std::string mathExpression1 = "(abcd + 5052) * (cat - 39182) % e / f";
	std::string mathExpression2 = "k - 15 * m + n / (o * 13)";
	std::string mathExpression3 = "(a + 9) * c - (d - e) / f";
	std::string mathExpression4 = "(go * 820) + (i / 45) - (kek % 9)";
	std::string mathExpression5 = "mad - 1262 * oof + p / q % rat";
	std::string mathExpression6 = "asd * (b + csd * 11) - (e - 10) / g";
	std::string mathExpression7 = "288 * (bao + 7) - dog / (e * 6726)";
	std::string mathExpression8 = "(q * 17) - (s / t) + (u % v)";
	std::string mathExpression9 = "w + x * 19 - z / a % b";
	std::string mathExpression10 = "(c * 21) + (e / f) - (g % 23)";

	auto res1 = ExpressionParser::convertToPostfix(mathExpression1);
	auto res2 = ExpressionParser::convertToPostfix(mathExpression2);
	auto res3 = ExpressionParser::convertToPostfix(mathExpression3);
	auto res4 = ExpressionParser::convertToPostfix(mathExpression4);
	auto res5 = ExpressionParser::convertToPostfix(mathExpression5);
	auto res6 = ExpressionParser::convertToPostfix(mathExpression6);
	auto res7 = ExpressionParser::convertToPostfix(mathExpression7);
	auto res8 = ExpressionParser::convertToPostfix(mathExpression8);
	auto res9 = ExpressionParser::convertToPostfix(mathExpression9);
	auto res10 = ExpressionParser::convertToPostfix(mathExpression10);

	std::vector<std::string> expectedResults1{ "abcd", "5052", "+", "cat", "39182", "-", "*", "e", "%", "f", "/" };
	std::vector<std::string> expectedResults2{ "k", "15", "m", "*", "-", "n", "o", "13", "*", "/", "+"};
	std::vector<std::string> expectedResults3{ "a", "9", "+", "c", "*", "d", "e", "-", "f", "/", "-" };
	std::vector<std::string> expectedResults4{ "go", "820", "*", "i", "45", "/", "+", "kek", "9", "%", "-" };
	std::vector<std::string> expectedResults5{ "mad", "1262", "oof", "*", "-", "p", "q", "/", "rat", "%", "+" };
	std::vector<std::string> expectedResults6{ "asd", "b", "csd", "11", "*", "+", "*", "e", "10", "-", "g", "/", "-" };
	std::vector<std::string> expectedResults7{ "288", "bao", "7", "+", "*", "dog", "e", "6726", "*", "/", "-" };
	std::vector<std::string> expectedResults8{ "q", "17", "*", "s", "t", "/", "-", "u", "v", "%", "+" };
	std::vector<std::string> expectedResults9{ "w", "x", "19", "*", "+", "z", "a", "/", "b", "%", "-" };
	std::vector<std::string> expectedResults10{ "c", "21", "*", "e", "f", "/", "+", "g", "23", "%", "-" };

	REQUIRE(res1 == expectedResults1);
	REQUIRE(res2 == expectedResults2);
	REQUIRE(res3 == expectedResults3);
	REQUIRE(res4 == expectedResults4);
	REQUIRE(res5 == expectedResults5);
	REQUIRE(res6 == expectedResults6);
	REQUIRE(res7 == expectedResults7);
	REQUIRE(res8 == expectedResults8);
	REQUIRE(res9 == expectedResults9);
	REQUIRE(res10 == expectedResults10);
}

TEST_CASE("Expressions with lots of tabs and spaces") {
	std::string mathExpression1 = "   (   a						+ 9			) * c    - (d		 - e				  )		/                          f";
	std::string mathExpression2 = "x		*  I		/ hh		g + p	3   N    \n /				f2b					+ poi0		 - n  ";

	auto res1 = ExpressionParser::convertToPostfix(mathExpression1);
	auto res2 = ExpressionParser::convertToPostfix(mathExpression2);

	std::vector<std::string> expectedResults1{ "a", "9", "+", "c", "*", "d", "e", "-", "f", "/", "-" };
	std::vector<std::string> expectedResults2{ "x", "I", "*", "hhg", "/", "p3N", "f2b", "/", "+", "poi0", "+", "n", "-" };

	REQUIRE(res1 == expectedResults1);
	REQUIRE(res2 == expectedResults2);
}

TEST_CASE("Invalid expressions") {
	std::string mathExpression1 = "1 + 2 +";
	std::string mathExpression2 = "a + b *";
	std::string mathExpression3 = "+ a b";
	std::string mathExpression4 = "a + b * (c - d))";
	std::string mathExpression5 = "(c - d /";
	std::string mathExpression6 = "e * f) + g -";
	std::string mathExpression7 = "k - l * m) / n";
	std::string mathExpression8 = "a + b * (c - d";
	std::string mathExpression9 = "a + b ^ c - d)";
	std::string mathExpression10 = "a$^_!@#";
	std::string mathExpression11 = "a + + b";
	std::string mathExpression12 = "a *+ B";
	std::string mathExpression13 = "(+)";
	std::string mathExpression14 = "()";

	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression1));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression2));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression3));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression4));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression5));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression6));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression7));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression8));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression9));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression10));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression11));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression12));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression13));
	REQUIRE_THROWS(ExpressionParser::convertToPostfix(mathExpression14));
}

TEST_CASE("Similar expressions should produce same tree") {
	std::string mathExpression1 = "a+b";
	std::string mathExpression2 = "(a+b)";
	std::string mathExpression3 = "a + b";

	REQUIRE(ExpressionParser::convertToPostfix(mathExpression1) == ExpressionParser::convertToPostfix(mathExpression2));
	REQUIRE(ExpressionParser::convertToPostfix(mathExpression1) == ExpressionParser::convertToPostfix(mathExpression3));
	REQUIRE(ExpressionParser::convertToPostfix(mathExpression2) == ExpressionParser::convertToPostfix(mathExpression3));

	std::string mathExpression4 = "a+b*c";
	std::string mathExpression5 = "a + b * c";
	std::string mathExpression6 = "a + (b * c)";

	REQUIRE(ExpressionParser::convertToPostfix(mathExpression4) == ExpressionParser::convertToPostfix(mathExpression5));
	REQUIRE(ExpressionParser::convertToPostfix(mathExpression4) == ExpressionParser::convertToPostfix(mathExpression6));
	REQUIRE(ExpressionParser::convertToPostfix(mathExpression5) == ExpressionParser::convertToPostfix(mathExpression6));
}

TEST_CASE("Test subarray function") {
	std::string mathExpression = "v + x * y + z * t";

	auto res = ExpressionParser::convertToPostfix(mathExpression);

    REQUIRE(ExpressionParser::isSubarray(res, ExpressionParser::convertToPostfix("")));
	REQUIRE(ExpressionParser::isSubarray(res, ExpressionParser::convertToPostfix("x")));
	REQUIRE(ExpressionParser::isSubarray(res, ExpressionParser::convertToPostfix("v")));
	REQUIRE(ExpressionParser::isSubarray(res, ExpressionParser::convertToPostfix("x*y")));
	REQUIRE(ExpressionParser::isSubarray(res, ExpressionParser::convertToPostfix("v+x*y")));
	REQUIRE(ExpressionParser::isSubarray(res, ExpressionParser::convertToPostfix("v+x*y+z*t")));

	REQUIRE_FALSE(ExpressionParser::isSubarray(res, ExpressionParser::convertToPostfix("v+x")));
	REQUIRE_FALSE(ExpressionParser::isSubarray(res, ExpressionParser::convertToPostfix("y+z*t")));
	REQUIRE_FALSE(ExpressionParser::isSubarray(res, ExpressionParser::convertToPostfix("x * y + z * t")));
}
