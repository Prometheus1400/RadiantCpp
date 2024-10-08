#ifndef TOKEN_H
#define TOKEN_H
#include <map>
#include <string>

enum TokenType {
    literal_beg,
    NUMBER_LIT,
    STRING_LIT,
    CHAR_LIT,
    BOOL_LIT,
    NIL_LIT,
    IDENTIFIER, // special kind of 'literal' that represents a stored value
    literal_end,

    operator_beg,
    SEMI_COLON,  // ; simplify the parsing by mandating semicolons
    LEFT_PAREN,  // (
    RIGHT_PAREN, // )
    LEFT_BRACE,  // {
    RIGHT_BRACE, // }
    LEFT_BRACK,  // [
    RIGHT_BRACK, // ]
    COMMA,       // ,
    PLUS,        // +
    MINUS,       // -
    STAR,        // *
    SLASH,       // /
    BANG,        // !
    ADDRESS,     // &
    ASSIGN,      // =
    DOT,         // .
    PLUSPLUS,    // ++
    MINUSMINUS,  // --
    EQUAL,       // ==
    NOT_EQUAL,   // !=
    LESS,        // <
    GREATER,     // >
    LESS_EQ,     // <=
    GREATER_EQ,  // >=
    operator_end,

    keyword_beg,
    LET,
    CONST,
    FN,
    RETURN,
    IF,
    ELIF,
    ELSE,
    FOR,
    DEFER,
    STRUCT,
    keyword_end,

    types_beg,
    NUMBER_TYPE,
    BOOL_TYPE,
    CHAR_TYPE,
    ARRAY_TYPE, // '[]' is a modifier after type like 'int[]'
    types_end,

    special_beg,
    COMMENT,
    EOF_TOKEN,
    ERROR,
    special_end,
};

const std::map<std::string, TokenType> KEYWORDS = {
    {"let", TokenType::LET},
    {"const", TokenType::CONST},
    {"fn", TokenType::FN},
    {"return", TokenType::RETURN},
    {"if", TokenType::IF},
    {"elif", TokenType::ELIF},
    {"else", TokenType::ELSE},
    {"for", TokenType::FOR},
    {"defer", TokenType::DEFER},
    {"struct", TokenType::STRUCT},
    {"number_type", TokenType::NUMBER_TYPE},
    {"bool_type", TokenType::BOOL_TYPE},
    {"char_type", TokenType::CHAR_TYPE},
};

struct Token {
    Token();
    Token(TokenType type, std::string lexeme, int line);
    ~Token();

    TokenType type;
    std::string lexeme;
    int line;
};

#endif
