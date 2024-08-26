#include "../include/ast_printer.h"
#include "../include/parser.h"
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

using std::string;
using std::vector;

TEST_CASE("check parser works", "[parser]") {
    Parser         parser("5 + 6;");
    ASTPrinter     printer;
    vector<string> expected = {"(5+6)"};

    parser.parse();
    const auto& stmts = parser.getStatements();
    REQUIRE(stmts.size() == 1);
    REQUIRE(printer.print(stmts) == expected);
}

TEST_CASE("check parser works multiple expressions", "[parser]") {
    Parser parser("5 + 6; 2 - 1; 3 * 2;");
    ASTPrinter     printer;
    vector<string> expected = {"(5+6)", "(2-1)", "(3*2)"};

    parser.parse();
    const auto& stmts = parser.getStatements();
    REQUIRE(stmts.size() == 3);
    REQUIRE(printer.print(stmts) == expected);
}

TEST_CASE("check parser works with grouping expression", "[parser]") {
    Parser parser("5 + 6 * 2; (5 + 6) * 2;");
    ASTPrinter     printer;
    vector<string> expected = {"(5+(6*2))", "((5+6)*2)"};

    parser.parse();
    const auto& stmts = parser.getStatements();
    REQUIRE(stmts.size() == 2);
    REQUIRE(printer.print(stmts) == expected);
}
