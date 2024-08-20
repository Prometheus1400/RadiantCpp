#include "../include/lexer.h"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("check lexer works", "[lexer]") {
    Lexer lexer("let x = \"test\";");
    bool result = lexer.scanTokens();
    std::vector<TokenType> actual;
    for (Token token : lexer.getTokens()) {
        actual.push_back(token.type);
    }
    std::vector<TokenType> expected = {LET, IDENTIFIER, ASSIGN, STRING_LIT,
                                       SEMI_COLON};
    REQUIRE(result == true);
    REQUIRE(actual == expected);
}

TEST_CASE("check lexer error", "[lexer]") {
    Lexer lexer("let x = \"test;");
    bool result = lexer.scanTokens();
    std::vector<TokenType> actual;
    for (Token token : lexer.getTokens()) {
        actual.push_back(token.type);
    }
    std::vector<TokenType> expected = {LET, IDENTIFIER, ASSIGN, STRING_LIT,
                                       SEMI_COLON};
    REQUIRE(result == false);
    REQUIRE(actual != expected);
}

