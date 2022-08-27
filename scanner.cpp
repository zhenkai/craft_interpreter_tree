#include "scanner.h"
#include <unordered_map>

namespace {
bool isDigit(char c) {
  return c >= '0' && c <= '9';
} 

bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isAlphaNumeric(char c) {
  return isAlpha(c) || isDigit(c);
}
}


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

char Scanner::peekNext() {
  if (current_ + 1 >= source_.length()) return '\0';
  return source_[current_ + 1];
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
    case '!':
      addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
      break;
    case '<':
      addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
      break;
    case '>':
      addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
      break;
    case '=':
      addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
      break;
    case '/':
      if (match('/')) {
        while (peek() != '\n' && !isAtEnd()) advance();
      } else {
        addToken(TokenType::SLASH);
      }
      break;
    case ' ':
    case '\t':
    case '\r':
      break;
    case '\n':
      line_++;
      break;
    case '"':
      string();
      break;
    default:
      if (isDigit(c)) {
        number();
      } else if (isAlpha(c)) {
        identifier();
      } else {
        errorReporter_.report(line_, "Unexpected character");
      }
      break;
  }
}

void Scanner::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') line_++;
    advance();
  }

  if (isAtEnd()) {
    errorReporter_.report(line_, "", "Unterminated string.");
    return;
  }

  // The closing ".
  advance();

  // Trim the surrounding quotes.
  std::string value = source_.substr(start_ + 1, current_ - start_ -2);
  addToken(TokenType::STRING, value);
}

void Scanner::number() {
  while (isDigit(peek())) advance();

  // Look for a fractional part.
  if (peek() == '.' && isDigit(peekNext())) {
    // Consume the "."
    advance();

    while (isDigit(peek())) advance();
  }

  addToken(TokenType::NUMBER, std::stod(source_.substr(start_, current_-start_-1)));
}

void Scanner::identifier() {
  while (isAlphaNumeric(peek())) advance();
  static const std::unordered_map<std::string, TokenType> keywords {
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"if", TokenType::IF},
    {"fun", TokenType::FUN},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE},
  };

  std::string value = source_.substr(start_, current_-start_-1);
  auto type = keywords.find(value) != keywords.end() ? keywords.at(value) : TokenType::IDENTIFIER;
  addToken(type);
}
