#include "include/lexer.h"
#include "include/token.h"

Lexer::Lexer(std::string&& source) : scanner(std::move(source)), tokens() {}

Lexer::~Lexer() {}

bool Lexer::scanTokens() {
    Token token = this->scanner.getNextToken();
    while (token.type != EOF_TOKEN && token.type != ERROR) {
        this->tokens.push_back(token);
        token = this->scanner.getNextToken();
    }

    if (this->scanner.hadError()) {
        this->scanner.reportErrors();
        return false;
    }
    return true;
}

std::vector<Token> Lexer::getTokens() {
    return std::move(this->tokens);
}
