#include "../include/scanner.h"
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <utility>

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

TEST_CASE("check scanner can parse basic expression", "[scanner]") {
    std::string source = "3 + 5;";
    Scanner scanner(std::move(source));

    Token t1 = scanner.getNextToken();
    REQUIRE(t1.type == NUMBER_LIT);
    REQUIRE(t1.line == 0);
    REQUIRE(t1.lexeme == "3");

    Token t2 = scanner.getNextToken();
    REQUIRE(t2.type == PLUS);
    REQUIRE(t2.line == 0);
    REQUIRE(t2.lexeme == "+");

    Token t3 = scanner.getNextToken();
    REQUIRE(t3.type == NUMBER_LIT);
    REQUIRE(t3.line == 0);
    REQUIRE(t3.lexeme == "5");

    Token t4 = scanner.getNextToken();
    REQUIRE(t4.type == SEMI_COLON);
    REQUIRE(t4.line == 0);
    REQUIRE(t4.lexeme == ";");


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

TEST_CASE("number literal", "[scanner]") {
    std::string source = "123";
    std::string source2 = "123.456";
    Scanner scanner(std::move(source));
    Scanner scanner2(std::move(source2));

    Token t = scanner.getNextToken();
    REQUIRE(t.type == NUMBER_LIT);
    REQUIRE(t.lexeme == "123");
    REQUIRE(t.line == 0);

    Token t2 = scanner2.getNextToken();
    REQUIRE(t2.type == NUMBER_LIT);
    REQUIRE(t2.lexeme == "123.456");
    REQUIRE(t2.line == 0);

    REQUIRE(scanner.hadError() == false);
}

TEST_CASE("identifiers and keywords", "[scanner]") {
    std::string source = "return\nelse\nfn";
    std::string source2 = "myVar myVar2 myVar_3";
    Scanner scanner(std::move(source));
    Scanner scanner2(std::move(source2));

    std::vector<std::pair<TokenType, std::string>> expected = {{RETURN, "return"}, {ELSE, "else"}, {FN, "fn"}};
    for (auto pair : expected) {
        Token t = scanner.getNextToken();
        REQUIRE(t.type == pair.first);
        REQUIRE(t.lexeme == pair.second);
    }

    std::vector<std::pair<TokenType, std::string>> expected2 = {{IDENTIFIER, "myVar"}, {IDENTIFIER, "myVar2"}, {IDENTIFIER, "myVar_3"}};
    for (auto pair : expected2) {
        Token t = scanner2.getNextToken();
        REQUIRE(t.type == pair.first);
        REQUIRE(t.lexeme == pair.second);
    }

    REQUIRE(scanner.hadError() == false);
}
