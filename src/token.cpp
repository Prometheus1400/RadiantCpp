#include "include/token.h"

Token::Token(TokenType type, std::string lexeme, int line): type(type), lexeme(lexeme), line(line) {

}

Token::~Token() {

}
