#ifndef LEXER_H
#define LEXER_H
#include "scanner.h"
#include "token.h"
#include <string>
#include <vector>

class Lexer {
  public:
    Lexer(std::string&& source);
    ~Lexer();
    bool scanTokens();
    std::vector<Token> getTokens();

  private:
    Scanner scanner;
    std::vector<Token> tokens;
};

#endif
