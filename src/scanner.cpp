#include "include/scanner.h"
#include "include/token.h"
#include <cctype>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

Scanner::Scanner(std::string&& source)
    : source(source), line(0), start(0), current(0), error(false), errors() {
}

Scanner::~Scanner() {
}

bool Scanner::hadError() {
    return this->error;
}

void Scanner::reportErrors() {
    for (std::string error : this->errors) {
        std::cerr << error << std::endl;
    }
}

Token Scanner::getNextToken() {
    this->skipWhitespace();
    if (this->isAtEnd()) {
        return Token(EOF_TOKEN, "", this->line);
    }

    this->start = this->current;
    this->advance();
    char c = this->prev();

    switch (c) {
    case '(':
        return this->makeToken(LEFT_PAREN);
    case ')':
        return this->makeToken(RIGHT_PAREN);
    case '{':
        return this->makeToken(LEFT_BRACE);
    case '}':
        return this->makeToken(RIGHT_BRACE);
    case '[':
        if (this->peek() == ']') {
            this->advance();
            return this->makeToken(ARRAY_TYPE);
        }
        return this->makeToken(LEFT_BRACK);
    case ']':
        return this->makeToken(RIGHT_BRACK);
    case ',':
        return this->makeToken(COMMA);
    case '*':
        return this->makeToken(STAR);
    case ';':
        return this->makeToken(SEMI_COLON);
    case '&':
        return this->makeToken(ADDRESS);
    case '.':
        return this->makeToken(DOT);
    case '<':
        if (this->peek() == '=') {
            this->advance();
            return this->makeToken(LESS_EQ);
        }
        return this->makeToken(LESS);
    case '>':
        if (this->peek() == '=') {
            this->advance();
            return this->makeToken(GREATER_EQ);
        }
        return this->makeToken(GREATER);
    case '!':
        if (this->peek() == '=') {
            this->advance();
            return this->makeToken(NOT_EQUAL);
        }
        return this->makeToken(BANG);
    case '=':
        if (this->peek() == '=') {
            this->advance();
            return this->makeToken(EQUAL);
        }
        return this->makeToken(ASSIGN);
    case '+':
        if (this->peek() == '+') {
            this->advance();
            return this->makeToken(PLUSPLUS);
        }
        return this->makeToken(PLUS);
    case '-':
        if (this->peek() == '-') {
            this->advance();
            return this->makeToken(MINUSMINUS);
        }
        return this->makeToken(MINUS);
    case '/':
        if (this->match('/')) {
            while (!this->isAtEnd() && this->peek() != '\n') {
                this->advance();
            }
            return this->makeToken(COMMENT);
        }
        return this->makeToken(SLASH);
    case '"':
        return this->stringToken();
    case '\'':
        return this->charToken();
    }
}

Token Scanner::stringToken() {
    while (!this->isAtEnd() && this->peek() != '"') {
        if (this->peek() == '\n') {
            this->errorAtCurrent("multiline strings are not supported");
            this->advance();
            break;
        }
        this->advance();
    }
    if (this->isAtEnd()) {
        this->errorAtCurrent("unterminated string literal");
    }
    this->advance();
    return Token(
        STRING_LIT,
        this->source.substr(this->start + 1, this->current - this->start - 2),
        this->line);
}

Token Scanner::charToken() {
    while (!this->isAtEnd() && this->peek() != '\'') {
        this->advance();
    }
    if (this->isAtEnd()) {
        this->errorAtCurrent("unterminated character literal");
    }
    this->advance();
    if (this->current - this->start > 3) {
        this->errorAtCurrent("char must be of length 1");
    }

    return Token(
        CHAR_LIT,
        this->source.substr(this->start + 1, this->current - this->start - 2),
        this->line);
}

void Scanner::advance() {
    this->current++;
}

void Scanner::skipWhitespace() {
    while (!this->isAtEnd() && std::isspace(this->source.at(this->current))) {
        if (this->source.at(this->current) == '\n') {
            this->line++;
        }
        this->advance();
    }
}

bool Scanner::match(char c) {
    if (this->source.at(this->current) == c) {
        this->advance();
        return true;
    }
    return false;
}

char Scanner::peek() {
    if (this->isAtEnd()) {
        return '\0';
    }
    return this->source.at(this->current);
}

char Scanner::doublePeek() {
    if (this->current + 1 >= this->source.size()) {
        return '\0';
    }
    return this->source.at(this->current + 1);
}

char Scanner::prev() {
    return this->source.at(this->current - 1);
}

bool Scanner::isAtEnd() {
    return this->current >= this->source.size();
}

Token Scanner::makeToken(TokenType tokenType) {
    return Token(tokenType,
                 this->source.substr(this->start, this->current - this->start),
                 this->line);
}

void Scanner::errorAtCurrent(std::string message) {
    std::stringstream errorString;
    errorString << "error: near line [" << this->line
                << "]. cause: " << message;
    this->errors.push_back(errorString.str());
    this->error = true;
}
