#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#include <string>
#include <vector>

class Scanner {
  public:
    Scanner(std::string&& source);
    Scanner(Scanner&& other);
    ~Scanner();
    Token getNextToken();
    void reportErrors();
    bool hadError();

  private:
    std::string source;
    int line;
    int start;
    int current;
    bool error;
    std::vector<std::string> errors;

    // helper methods
    void skipWhitespace();
    void advance();
    bool match(char c);
    char peek();
    char doublePeek();
    char prev();
    bool isAtEnd();
    Token makeToken(TokenType tokenType);
    void errorAtCurrent(std::string message);
    Token stringToken();
    Token charToken();
    Token numberToken();
    Token identifierToken();
};

#endif
