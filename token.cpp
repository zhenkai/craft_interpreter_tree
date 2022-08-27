#include "token.h"
#include <unordered_map>

std::string Token::str() const {
  static const std::unordered_map<TokenType, std::string> tokenNames {
    {TokenType::LEFT_PAREN, "("},
    {TokenType::RIGHT_PAREN, ")"},
    {TokenType::LEFT_BRACE, "{TokenType::"},
    {TokenType::RIGHT_BRACE, "}"},
    {TokenType::COMMA, ","},
    {TokenType::DOT, "."},
    {TokenType::MINUS, "-"},
    {TokenType::PLUS, "+"},
    {TokenType::SEMICOLON, ";"},
    {TokenType::SLASH, "/"},
    {TokenType::STAR, "*"},
    {TokenType::BANG, "!"},
    {TokenType::BANG_EQUAL, "!="},
    {TokenType::EQUAL, "="},
    {TokenType::EQUAL_EQUAL, "=="},
    {TokenType::LESS, "<"},
    {TokenType::LESS_EQUAL, "<="},
    {TokenType::IDENTIFIER, "Identifier"},
    {TokenType::STRING, "String"},
    {TokenType::NUMBER, "Number"},
    {TokenType::AND, "and"},
    {TokenType::CLASS, "class"},
    {TokenType::ELSE, "else"},
    {TokenType::FALSE, "false"},
    {TokenType::FUN, "fun"},
    {TokenType::FOR, "for"},
    {TokenType::IF, "if"},
    {TokenType::NIL, "nil"},
    {TokenType::OR, "or"},
    {TokenType::PRINT, "print"},
    {TokenType::RETURN, "return"},
    {TokenType::SUPER, "super"},
    {TokenType::THIS, "this"},
    {TokenType::TRUE, "true"},
    {TokenType::VAR, "var"},
    {TokenType::WHILE, "while"},
    {TokenType::EOF_, "EOF"},
  };
  return tokenNames.at(type_) + " " + lexeme_ + " " + literal_;
}
