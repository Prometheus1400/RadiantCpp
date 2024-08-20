#include "include/parser.h"
#include "include/statements.h"
#include "include/token.h"
#include <sstream>
#include <string>

Parser::Parser(std::string&& source)
    : lexer(std::move(source)), tokens(), start(0), current(0), hadError(false),
      errors(),
      // clang-format off
    parseTable{
        {LEFT_PAREN,  {nullptr, nullptr, PREC_CALL}},
        {RIGHT_PAREN, {nullptr, nullptr, PREC_NONE}},
        {LEFT_BRACE,  {nullptr, nullptr, PREC_NONE}},
        {RIGHT_BRACE, {nullptr, nullptr, PREC_NONE}},
        {LEFT_BRACK,  {nullptr, nullptr, PREC_NONE}},
        {RIGHT_BRACK, {nullptr, nullptr, PREC_NONE}},
        {SEMI_COLON,  {nullptr, nullptr, PREC_NONE}},
        {COMMA,       {nullptr, nullptr, PREC_NONE}},
        {PLUS,        {nullptr, nullptr, PREC_TERM}},
        {MINUS,       {nullptr, nullptr, PREC_TERM}},
        {STAR,        {nullptr, nullptr, PREC_FACTOR}},
        {SLASH,       {nullptr, nullptr, PREC_FACTOR}},
        {BANG,        {nullptr, nullptr, PREC_UNARY}},
        {ADDRESS,     {nullptr, nullptr, PREC_UNARY}},
        {ASSIGN,      {nullptr, nullptr, PREC_NONE}},
        {DOT,         {nullptr, nullptr, PREC_NONE}},
        {PLUSPLUS,    {nullptr, nullptr, PREC_NONE}},
        {MINUSMINUS,  {nullptr, nullptr, PREC_NONE}},
        {EQUAL,       {nullptr, nullptr, PREC_EQUALITY}},
        {NOT_EQUAL,   {nullptr, nullptr, PREC_EQUALITY}},
        {LESS,        {nullptr, nullptr, PREC_COMPARISON}},
        {GREATER,     {nullptr, nullptr, PREC_COMPARISON}},
        {LESS_EQ,     {nullptr, nullptr, PREC_COMPARISON}},
        {GREATER_EQ,  {nullptr, nullptr, PREC_COMPARISON}},
        {NUMBER_LIT,  {nullptr, nullptr, PREC_PRIMARY}},
        /* {STRING,      {nullptr, nullptr, PREC_PRIMARY}}, */
        {CHAR_LIT,    {nullptr, nullptr, PREC_PRIMARY}},
        {BOOL_LIT,    {nullptr, nullptr, PREC_PRIMARY}},
        {BOOL_LIT,    {nullptr, nullptr, PREC_PRIMARY}},
        {IDENTIFIER,  {nullptr, nullptr, PREC_PRIMARY}},
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

const std::vector<std::unique_ptr<Stmt>>& Parser::getStatements() const {
    return this->statements;
}

std::vector<std::unique_ptr<Stmt>>& Parser::getStatements() {
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
            std::unique_ptr<Stmt> stmt = this->declaration();
            this->statements.push_back(stmt);
        } catch (const std::exception& e) {
            this->synchronize();
        }
    }
}

Token Parser::advance() {
    this->current++;
    return this->peek();
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
Token Parser::consume(TokenType type, std::string message) {
    Token cur = this->peek();
    if (cur.type == type) {
        return this->advance();
    }
    std::stringstream errorMessage;
    errorMessage << message << " - instead consumed '" << type << "'"; //TODO: make this a nice string instead of enum val
    this->errorAtCurrent(errorMessage.str());
}
Parser::ParseRule Parser::currentTokenRule() {
}
Parser::Precedence Parser::currentTokenPrecedence() {
}
Parser::Precedence Parser::tokenPrecedence(TokenType type) {
}
void Parser::synchronize() {
}
void Parser::errorAtCurrent(std::string message) {
}
