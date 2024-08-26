#include "include/parser.h"
#include "include/expressions.h"
#include "include/statements.h"
#include "include/token.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

Parser::Parser(string&& source)
    : lexer(std::move(source)), tokens(), start(0), current(0), hadError(false), errors(),
      // clang-format off
    parseTable{
        {LEFT_PAREN,  {&Parser::grouping, nullptr, PREC_CALL}},
        {RIGHT_PAREN, {nullptr, nullptr, PREC_NONE}},
        {LEFT_BRACE,  {nullptr, nullptr, PREC_NONE}},
        {RIGHT_BRACE, {nullptr, nullptr, PREC_NONE}},
        {LEFT_BRACK,  {nullptr, nullptr, PREC_NONE}},
        {RIGHT_BRACK, {nullptr, nullptr, PREC_NONE}},
        {SEMI_COLON,  {nullptr, nullptr, PREC_NONE}},
        {COMMA,       {nullptr, nullptr, PREC_NONE}},
        {PLUS,        {nullptr, &Parser::binary, PREC_TERM}},
        {MINUS,       {&Parser::unary, &Parser::binary, PREC_TERM}},
        {STAR,        {&Parser::unary, &Parser::binary, PREC_FACTOR}},
        {SLASH,       {nullptr, &Parser::binary, PREC_FACTOR}},
        {BANG,        {&Parser::unary, nullptr, PREC_UNARY}},
        {ADDRESS,     {nullptr, nullptr, PREC_UNARY}},
        {ASSIGN,      {nullptr, nullptr, PREC_NONE}},
        {DOT,         {nullptr, nullptr, PREC_NONE}},
        {PLUSPLUS,    {nullptr, nullptr, PREC_NONE}},
        {MINUSMINUS,  {nullptr, nullptr, PREC_NONE}},
        {EQUAL,       {nullptr, &Parser::binary, PREC_EQUALITY}},
        {NOT_EQUAL,   {nullptr, &Parser::binary, PREC_EQUALITY}},
        {LESS,        {nullptr, &Parser::binary, PREC_COMPARISON}},
        {GREATER,     {nullptr, &Parser::binary, PREC_COMPARISON}},
        {LESS_EQ,     {nullptr, &Parser::binary, PREC_COMPARISON}},
        {GREATER_EQ,  {nullptr, &Parser::binary, PREC_COMPARISON}},
        {NUMBER_LIT,  {&Parser::number, nullptr, PREC_PRIMARY}},
        /* {STRING,      {nullptr, nullptr, PREC_PRIMARY}}, */
        {CHAR_LIT,    {&Parser::character, nullptr, PREC_PRIMARY}},
        {BOOL_LIT,    {&Parser::boolean, nullptr, PREC_PRIMARY}},
        {IDENTIFIER,  {&Parser::variable, nullptr, PREC_PRIMARY}},
        {LET,         {nullptr, nullptr, PREC_NONE}},
        {FN,          {nullptr, nullptr, PREC_NONE}},
        {IF,          {nullptr, nullptr, PREC_NONE}},
        {ELSE,        {nullptr, nullptr, PREC_NONE}},
        {RETURN,      {nullptr, nullptr, PREC_NONE}},
        {EOF_TOKEN,   {nullptr, nullptr, PREC_NONE}},
          // clang-format on
      } {
}

Parser::~Parser() {
}

const vector<unique_ptr<Stmt>>& Parser::getStatements() const {
    return this->statements;
}

vector<unique_ptr<Stmt>>& Parser::getStatements() {
    return this->statements;
}

void Parser::parse() {
    // check for error during lexing
    if (!this->lexer.scanTokens()) {
        return;
    }
    this->tokens = this->lexer.getTokens();

    while (!this->isAtEnd()) {
        try {
            unique_ptr<Stmt> stmt = this->declaration();
            this->statements.push_back(std::move(stmt));
        } catch (const std::exception& e) {
            this->synchronize();
        }
    }
}

unique_ptr<Stmt> Parser::declaration() {
    if (this->match(LET)) {
        return this->varDeclaration();
    } else if (this->match(FN)) {
        return this->fnDeclaration();
    } else {
        return this->statement();
    }
}

unique_ptr<Stmt> Parser::statement() {
    if (this->match(IF)) {
        return this->ifStmt();
    } else if (this->match(LEFT_BRACE)) {
        return this->blockStmt();
    } else if (this->match(RETURN)) {
        return this->returnStmt();
    } else {
        return this->expressionStmt();
    }
}

unique_ptr<Stmt> Parser::varDeclaration() {
    // TODO:
    return nullptr;
}

unique_ptr<Stmt> Parser::fnDeclaration() {
    // TODO:
    return nullptr;
}

unique_ptr<Stmt> Parser::returnStmt() {
    // TODO:
    return nullptr;
}

unique_ptr<Stmt> Parser::ifStmt() {
    // TODO:
    return nullptr;
}

unique_ptr<Stmt> Parser::blockStmt() {
    // TODO:
    return nullptr;
}

unique_ptr<Stmt> Parser::expressionStmt() {
    auto expr = this->expression();
    this->consume(SEMI_COLON, "Expected ';'");

    auto stmt        = std::make_unique<ExpressionStmt>();
    stmt->expression = std::move(expr);
    return stmt;
}

unique_ptr<Expr> Parser::expression() {
    return this->prattParse(PREC_ASSIGNMENT);
}

unique_ptr<Expr> Parser::prattParse(Precedence precedence) {
    Token      token    = this->advance();
    PrefixRule prefixFn = this->parseTable.at(token.type).prefixRule;
    if (prefixFn == nullptr) {
        stringstream ss;
        ss << "no prefix parse expression for lexeme '" << token.lexeme << "'";
        this->errorAtCurrent(ss.str());
    }

    unique_ptr<Expr> left = (this->*prefixFn)();
    while (precedence < this->currentTokenPrecedence()) {
        Token     token   = this->advance();
        InfixRule infixFn = this->parseTable.at(token.type).infixRule;
        if (infixFn == nullptr) {
            return left;
        }
        left = (this->*infixFn)(std::move(left));
    }
    return left;
}

unique_ptr<Expr> Parser::variable() {
    Token token = this->prev();
    auto  expr  = std::make_unique<IdentifierExpr>();
    expr->value = token;
    return expr;
}

unique_ptr<Expr> Parser::grouping() {
    auto expr = this->expression();
    this->consume(RIGHT_PAREN, "expected closing parenthesis");
    auto groupingExpr        = std::make_unique<GroupingExpr>();
    groupingExpr->expression = std::move(expr);
    return groupingExpr;
}

unique_ptr<Expr> Parser::boolean() {
}

unique_ptr<Expr> Parser::character() {
}

unique_ptr<Expr> Parser::number() {
    auto expr   = unique_ptr<NumberExpr>(new NumberExpr());
    expr->value = std::stod(this->prev().lexeme);
    return expr;
}

unique_ptr<Expr> Parser::binary(unique_ptr<Expr> left) {
    Token      oper     = this->prev();
    Precedence operPrec = this->tokenPrecedence(oper.type);
    auto       right    = this->prattParse(operPrec);

    auto expr   = std::make_unique<BinaryExpr>();
    expr->left  = std::move(left);
    expr->oper  = oper;
    expr->right = std::move(right);
    return expr;
}

unique_ptr<Expr> Parser::unary() {
    Token      oper     = this->prev();
    Precedence operPrec = this->tokenPrecedence(oper.type);
    auto       right    = this->prattParse(operPrec);

    auto expr   = std::make_unique<UnaryExpr>();
    expr->oper  = oper;
    expr->right = std::move(right);
    return expr;
}

Token Parser::advance() {
    this->current++;
    return this->prev();
}
Token Parser::peek() {
    return this->tokens.at(this->current);
}
Token Parser::prev() {
    return this->tokens.at(this->current - 1);
}
bool Parser::check(TokenType type) {
    return this->peek().type == type;
}
template <typename... Args> bool Parser::match(Args... args) {
    return (... || (this->check(args)));
}
bool Parser::isAtEnd() {
    return this->current >= this->tokens.size() || this->peek().type == EOF_TOKEN;
}
Token Parser::consume(TokenType type, string message) {
    Token cur = this->peek();
    if (cur.type == type) {
        return this->advance();
    }
    stringstream errorMessage;
    errorMessage << message << " - instead consumed '" << type << "'"; // TODO: make this a nice string instead of enum val
    this->errorAtCurrent(errorMessage.str());
    // TODO: figure out how to disable this warn since this throws an exception at this point always
}
Parser::ParseRule Parser::currentTokenRule() {
    return this->parseTable.at(this->peek().type);
}
Parser::Precedence Parser::currentTokenPrecedence() {
    return this->currentTokenRule().precedence;
}
Parser::Precedence Parser::tokenPrecedence(TokenType type) {
    return this->parseTable.at(type).precedence;
}
void Parser::synchronize() {
    this->advance();
    while (!this->isAtEnd()) {
        if (this->prev().type == SEMI_COLON) {
            return;
        }
        switch (this->peek().type) {
        case LET:
        case FN:
        case RETURN:
        case IF:
            return;
        default:
            this->advance();
        }
    }
}
void Parser::errorAtCurrent(string message) {
    stringstream ss;
    ss << "error: near line [" << this->prev().line << "], cause: " << message;
    this->errors.push_back(ss.str());
    this->hadError = true;
    throw std::runtime_error(ss.str());
}
