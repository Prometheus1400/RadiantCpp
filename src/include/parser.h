#ifndef PARSER_H
#define PARSER_H
#include "expressions.h"
#include "lexer.h"
#include "statements.h"
#include "token.h"
#include <map>
#include <memory>

class Parser {
    using PrefixRule = std::unique_ptr<Expr>(Parser::*)();
    using InfixRule  = std::unique_ptr<Expr>(Parser::*)(std::unique_ptr<Expr> left);

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
    // actual parse methods
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> fnDeclaration();
    std::unique_ptr<Stmt> ifStmt();
    std::unique_ptr<Stmt> blockStmt();
    std::unique_ptr<Stmt> returnStmt();
    std::unique_ptr<Stmt> expressionStmt();
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> prattParse(Precedence precedence);
    std::unique_ptr<Expr> grouping();
    std::unique_ptr<Expr> number();
    std::unique_ptr<Expr> character();
    std::unique_ptr<Expr> boolean();
    std::unique_ptr<Expr> variable();
    std::unique_ptr<Expr> binary(std::unique_ptr<Expr> left);
    std::unique_ptr<Expr> unary();
    // parse helpers
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
