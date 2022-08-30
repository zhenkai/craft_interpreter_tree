#pragma once

#include <string>
#include <any>

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, 
  
  EOF_
};

class Token {
public:
  Token(TokenType type, const std::string& lexeme, const std::any& literal, int line): type(type), lexeme(lexeme), literal(literal), line(line) {}
  std::string str() const;
  std::string errorStr() const;
  const TokenType type;
  const std::string lexeme;
  const std::any literal;
  const int line;
};
