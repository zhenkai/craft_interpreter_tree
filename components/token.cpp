#include "token.h"
#include <unordered_map>
#include <sstream>

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
  std::stringstream s;
  s << "Token type: " << tokenNames.at(type_) << ", lexeme: " << lexeme_ << ", literal: ";
  if (literal_.has_value()) {
    if (literal_.type() == typeid(std::string)) {
      s << std::any_cast<std::string>(literal_);
    } else if (literal_.type() == typeid(double)) {
      s << std::any_cast<double>(literal_);
    } else if (literal_.type() == typeid(int)) {
      s << std::any_cast<int>(literal_);
    } else if (literal_.type() == typeid(float)) {
      s << std::any_cast<float>(literal_);
    } else {
      s << "LITERAL: unknown";
    }
  }
  return s.str();
  // return tokenNames.at(type_) + " " + lexeme_ + " " + literal_;
}
