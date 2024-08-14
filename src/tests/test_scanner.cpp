#include "../include/scanner.h"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("check scanner can parse basic tokens", "[scanner]") {
    std::string source = "()";
    Scanner scanner(std::move(source));

    source = "ee";

    Token t1 = scanner.getNextToken();
    REQUIRE(t1.type == LEFT_PAREN);
    REQUIRE(t1.line == 0);
    REQUIRE(t1.lexeme == "(");

    Token t2 = scanner.getNextToken();
    REQUIRE(t2.type == RIGHT_PAREN);
    REQUIRE(t2.line == 0);
    REQUIRE(t2.lexeme == ")");

    Token t3 = scanner.getNextToken();
    REQUIRE(t3.type == EOF_TOKEN);
    REQUIRE(t3.line == 0);
    REQUIRE(t3.lexeme == "");

    REQUIRE(scanner.hadError() == false);
}

TEST_CASE("check scanner can parse basic tokens and keeps track of changing "
          "line number",
          "[scanner]") {
    std::string source = "(\n)\n";
    Scanner scanner(std::move(source));

    Token t1 = scanner.getNextToken();
    REQUIRE(t1.type == LEFT_PAREN);
    REQUIRE(t1.line == 0);
    REQUIRE(t1.lexeme == "(");

    Token t2 = scanner.getNextToken();
    REQUIRE(t2.type == RIGHT_PAREN);
    REQUIRE(t2.line == 1);
    REQUIRE(t2.lexeme == ")");

    Token t3 = scanner.getNextToken();
    REQUIRE(t3.type == EOF_TOKEN);
    REQUIRE(t3.line == 2);
    REQUIRE(t3.lexeme == "");

    REQUIRE(scanner.hadError() == false);
}

TEST_CASE("check array_type", "[scanner]") {
    std::string source = "[)][]";
    Scanner scanner(std::move(source));

    std::vector<TokenType> expected = {LEFT_BRACK, RIGHT_PAREN, RIGHT_BRACK,
                                       ARRAY_TYPE};
    for (TokenType type : expected) {
        REQUIRE(scanner.getNextToken().type == type);
    }

    REQUIRE(scanner.hadError() == false);
}

TEST_CASE("string literal", "[scanner]") {
    std::string source = "\"hello world\"}";
    Scanner scanner(std::move(source));

    Token stringLit = scanner.getNextToken();
    REQUIRE(stringLit.type == STRING_LIT);
    REQUIRE(stringLit.lexeme == "hello world");
    REQUIRE(stringLit.line == 0);

    Token brace = scanner.getNextToken();
    REQUIRE(brace.type == RIGHT_BRACE);
    REQUIRE(brace.lexeme == "}");
    REQUIRE(brace.line == 0);

    REQUIRE(scanner.hadError() == false);
}

TEST_CASE("string literal errors", "[scanner]") {
    std::string source = "\"hello world";
    std::string source2 = "\"hello world\n\"";
    Scanner scanner(std::move(source));
    Scanner scanner2(std::move(source2));

    Token t = scanner.getNextToken();
    REQUIRE(scanner.hadError() == true);

    Token t2 = scanner2.getNextToken();
    REQUIRE(scanner2.hadError() == true);
}

TEST_CASE("char literal", "[scanner]") {
    std::string source = "'c'";
    Scanner scanner(std::move(source));

    Token t = scanner.getNextToken();
    REQUIRE(t.type == CHAR_LIT);
    REQUIRE(t.lexeme == "c");
    REQUIRE(t.line == 0);

    REQUIRE(scanner.hadError() == false);
}

TEST_CASE("char literal errors", "[scanner]") {
    std::string source = "'ca'";
    std::string source2 = "'c";
    Scanner scanner(std::move(source));
    Scanner scanner2(std::move(source2));

    Token t = scanner.getNextToken();
    REQUIRE(scanner.hadError() == true);

    Token t2 = scanner2.getNextToken();
    REQUIRE(scanner2.hadError() == true);
}
