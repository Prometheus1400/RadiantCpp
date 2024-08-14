#include "include/lexer.h"
#include "include/token.h"

Lexer::Lexer(std::string&& source) : scanner(std::move(source)) {}

Lexer::~Lexer() {}

std::vector<Token>& Lexer::getTokens() {
    return this->tokens;
}
