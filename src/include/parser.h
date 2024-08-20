#ifndef PARSER_H
#define PARSER_H
#include "expressions.h"
#include "lexer.h"
#include "statements.h"
#include "token.h"
#include <map>
#include <memory>

class Parser {
    using PrefixRule = Expr* (*)(/* parameters */);
    using InfixRule  = Expr* (*)(/* parameters */);

  public:
    Parser(std::string&& source);
    ~Parser();
    void                                      parse();
    std::vector<std::unique_ptr<Stmt>>&       getStatements();
    const std::vector<std::unique_ptr<Stmt>>& getStatements() const;

  private:
    Lexer                              lexer;
    std::vector<Token>                 tokens;
    int                                start;
    int                                current;
    bool                               hadError;
    std::vector<std::string>           errors;
    std::vector<std::unique_ptr<Stmt>> statements;
    enum Precedence {
        PREC_NONE,
        PREC_ASSIGNMENT, // =
        PREC_OR,         // or
        PREC_AND,        // and
        PREC_EQUALITY,   // ==
        PREC_COMPARISON, // < > <= >=
        PREC_TERM,       // + -
        PREC_FACTOR,     // * /
        PREC_UNARY,      // ! -
        PREC_CALL,       // . ()
        PREC_PRIMARY,
    };
    struct ParseRule {
        PrefixRule prefixRule;
        InfixRule  infixRule;
        Precedence precedence;
    } parseRule;
    std::map<TokenType, ParseRule>   parseTable;
    Token                            advance();
    Token                            peek();
    Token                            prev();
    bool                             check(TokenType type);
    template <typename... Args> bool match(Args... args);
    bool                             isAtEnd();
    Token                            consume(TokenType type, std::string message);
    ParseRule                        currentTokenRule();
    Precedence                       currentTokenPrecedence();
    Precedence                       tokenPrecedence(TokenType type);
    void                             synchronize();
    void                             errorAtCurrent(std::string message);
};

#endif
