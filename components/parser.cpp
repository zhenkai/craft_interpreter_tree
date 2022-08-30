#include "parser.h"
#include "expr.h"

ExprPtr Parser::expression() {
  return equality();
}

ExprPtr Parser::equality() {
  ExprPtr expr = comparison();

  while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
    Token op = previous();
    ExprPtr right = comparison();
    expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }

  return expr;
}

bool Parser::match(const std::vector<TokenType>&& types) {
  for (const auto& type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }
  
  return false;
}

ExprPtr Parser::comparison() {
  ExprPtr expr = term();
  while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
    Token op = previous();
    ExprPtr right = term();
    expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

ExprPtr Parser::term() {
  ExprPtr expr = factor();
  while (match({TokenType::MINUS, TokenType::PLUS})) {
    Token op = previous();
    ExprPtr right = factor();
    expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

ExprPtr Parser::factor() {
  ExprPtr expr = unary();

  while (match({TokenType::SLASH, TokenType::STAR})) {
    Token op = previous();
    ExprPtr right = unary();
    expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

ExprPtr Parser::unary() {
  if (match({TokenType::BANG, TokenType::MINUS})) {
    Token op = previous();
    ExprPtr right = unary();
    return std::make_unique<Unary>(op, std::move(right));
  }

  return primary();
}

ExprPtr Parser::primary() {
  if (match({TokenType::FALSE})) {
    return std::make_unique<Literal>(false);
  }
  if (match({TokenType::TRUE})) {
    return std::make_unique<Literal>(true);
  }
  if (match({TokenType::NIL})) {
    return std::make_unique<Literal>(std::any());
  }

  if (match({TokenType::NUMBER, TokenType::STRING})) {
    return std::make_unique<Literal>(previous().literal);
  }

  if (match({TokenType::LEFT_PAREN})) {
    ExprPtr expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_unique<Grouping>(std::move(expr));
  }

  throw error(peek(), "Expect expression.");
}

Token Parser::consume(const TokenType type, const std::string& msg) {
  if (check(type)) {
    return advance();
  }

  throw error(peek(), msg);
}

ParseError* Parser::error(const Token& token, const std::string& msg) {
  if (token.type == TokenType::EOF_) {
    errorReporter_.report(token.line, " at end", msg);
  } else {
    errorReporter_.report(token.line, " at '" + token.lexeme + "'", msg);
  }

  return new ParseError();
}

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous().type == TokenType::SEMICOLON) return;

    switch (peek().type) {
      case TokenType::CLASS:
      case TokenType::FUN:
      case TokenType::VAR:
      case TokenType::FOR:
      case TokenType::IF:
      case TokenType::WHILE:
      case TokenType::PRINT:
      case TokenType::RETURN:
        return;
      default:
        break;
    }

    advance();
  }
}

ExprPtr Parser::parse() {
  try {
    return expression();
  } catch (ParseError* pe) {
    return nullptr;
  }
}