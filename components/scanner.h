#pragma once

#include "error.h"
#include "token.h"
#include <any>
#include <string>
#include <vector>

class Scanner {
public:
  Scanner(const std::string &source, const ErrorReporter &errorReporter)
      : source_(source), errorReporter_(errorReporter), tokens_({}), start_(0),
        current_(0), line_(1) {}
  const std::vector<const Token> &scanTokens();

private:
  bool isAtEnd() { return current_ >= source_.length(); }
  char advance() { return source_[current_++]; }
  void addToken(TokenType type) {
    static std::any EMPTY_ANY;
    addToken(type, EMPTY_ANY);
  }

  void addToken(TokenType type, const std::any &literal);
  void scanToken();
  bool match(char expected);
  char peek();
  char peekNext();
  void string();
  void number();
  void identifier();

  const std::string source_;
  const ErrorReporter &errorReporter_;
  std::vector<const Token> tokens_;
  int start_, current_, line_;
};
