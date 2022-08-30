#pragma once

#include <vector>
#include <exception>
#include "token.h"
#include "error.h"
#include "expr.h"

class ParseError : public std::exception {};

/***
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
***/
class Parser {
public:
  Parser(const std::vector<const Token>& tokens, ErrorReporter& errorReporter ): tokens_(tokens), current_(0), errorReporter_(errorReporter) {}
  ExprPtr parse();
private:
  ExprPtr expression();
  ExprPtr equality();
  bool match(const std::vector<TokenType>&& types); 

  Token peek() { return tokens_[current_];}

  Token previous() { return tokens_[current_-1]; }

  bool isAtEnd() {
    return peek().type == TokenType::EOF_;
  }

  Token advance() {
    if (!isAtEnd()) {
      current_++;
    }
    return previous();
  }

  bool check(TokenType type) {
    if (isAtEnd()) {
      return false;
    }

    return peek().type == type;
  }

  ExprPtr comparison();
  ExprPtr term();
  ExprPtr factor();
  ExprPtr unary();
  ExprPtr primary();
  Token consume(const TokenType type, const std::string& msg);
  ParseError* error(const Token& token, const std::string& msg);
  void synchronize();
  // ----------------------------
  const std::vector<const Token>& tokens_;
  ErrorReporter& errorReporter_;
  int current_;
};
