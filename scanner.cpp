#include "scanner.h"

const std::vector<const Token>& Scanner::scanTokens() {
  while (!isAtEnd()) {
    // we're at the beginning of the next lexme
    start_ = current_;
    scanToken();
  }

  tokens_.emplace_back(TokenType::EOF_, "", std::make_any<int>(0), line_);

  return tokens_;
}

void Scanner::addToken(TokenType type, const std::any& literal) {
  std::string text = source_.substr(start_, current_-start_);
  tokens_.emplace_back(type, text, literal, line_);
}

bool Scanner::match(char expected) {
  if (isAtEnd()) return false;
  if (source_[current_] != expected) return false;

  current_++;
  return true;
}

char Scanner::peek() {
  if (isAtEnd()) return '\0';
  return source_[current_];
}

void Scanner::scanToken() {
  char c = advance();
  switch (c) {
    case '(': addToken(TokenType::LEFT_PAREN); break;
    case ')': addToken(TokenType::RIGHT_PAREN); break;
    case '{': addToken(TokenType::LEFT_BRACE); break;
    case '}': addToken(TokenType::RIGHT_BRACE); break;
    case ',': addToken(TokenType::COMMA); break;
    case '.': addToken(TokenType::DOT); break;
    case '-': addToken(TokenType::MINUS); break;
    case '+': addToken(TokenType::PLUS); break;
    case ';': addToken(TokenType::SEMICOLON); break;
    case '*': addToken(TokenType::STAR); break; 
    default:
      errorReporter_.report(line_, "Unexpected character");
      break;
  }
}
