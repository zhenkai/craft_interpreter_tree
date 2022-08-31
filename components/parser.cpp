#include "parser.h"
#include "expr.h"
#include <iostream>

ExprPtr Parser::expression() { return assignment(); }

ExprPtr Parser::assignment() {
  ExprPtr expr = orExpr();

  if (match({TokenType::EQUAL})) {
    Token equals = previous();
    ExprPtr value = assignment();
    if (Variable *v = dynamic_cast<Variable *>(expr.get())) {
      return std::make_unique<Assignment>(v->name, std::move(value));
    }
    errorReporter_.report(equals.line, " = ", "Invalid assignment target.");
  }

  return expr;
}

ExprPtr Parser::orExpr() {
  ExprPtr expr = andExpr();
  while (match({TokenType::OR})) {
    Token op = previous();
    ExprPtr right = andExpr();
    expr = std::make_unique<Logical>(op, std::move(expr), std::move(right));
  }

  return expr;
}

ExprPtr Parser::andExpr() {
  ExprPtr expr = equality();
  while (match({TokenType::AND})) {
    Token op = previous();
    ExprPtr right = equality();
    expr = std::make_unique<Logical>(op, std::move(expr), std::move(right));
  }
  return expr;
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

bool Parser::match(const std::vector<TokenType> &&types) {
  for (const auto &type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

ExprPtr Parser::comparison() {
  ExprPtr expr = term();
  while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                TokenType::LESS_EQUAL})) {
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

  if (match({TokenType::IDENTIFIER})) {
    return std::make_unique<Variable>(previous());
  }

  if (match({TokenType::LEFT_PAREN})) {
    ExprPtr expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_unique<Grouping>(std::move(expr));
  }

  // throw away un-recognized stuff
  advance();
  throw error(previous(), "Expect expression.");
}

Token Parser::consume(const TokenType type, const std::string &msg) {
  if (check(type)) {
    return advance();
  }

  throw error(peek(), msg);
}

ParseError *Parser::error(const Token &token, const std::string &msg) {
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
    if (previous().type == TokenType::SEMICOLON)
      return;

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

// ExprPtr Parser::parse() {
//   try {
//     return expression();
//   } catch (ParseError* pe) {
//     return nullptr;
//   }
// }
StmtPtr Parser::declaration() {
  try {
    if (match({TokenType::VAR}))
      return varStatement();

    return statement();
  } catch (ParseError *pe) {
    synchronize();
    return nullptr;
  }
}
StmtPtr Parser::statement() {
  if (match({TokenType::IF}))
    return ifStatement();
  if (match({TokenType::PRINT}))
    return printStatement();
  if (match({TokenType::LEFT_BRACE}))
    return block();

  return expressionStatement();
}

StmtPtr Parser::ifStatement() {
  consume(TokenType::LEFT_PAREN, "Expect '(' after if.");
  auto condition = expression();
  consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
  auto thenStmt = statement();
  StmtPtr elseStmt = nullptr;
  if (match({TokenType::ELSE})) {
    elseStmt = statement();
  }

  return std::make_unique<IfStmt>(std::move(condition), std::move(thenStmt),
                                  std::move(elseStmt));
}

StmtPtr Parser::printStatement() {
  ExprPtr value = expression();
  consume(TokenType::SEMICOLON, "Expect ';' after value.");
  return std::make_unique<PrintStmt>(std::move(value));
}

StmtPtr Parser::block() {
  std::vector<StmtPtr> stmts;
  while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
    stmts.push_back(declaration());
  }
  consume(TokenType::RIGHT_BRACE, "Expect '}' at the end of a block.");

  return std::make_unique<Block>(std::move(stmts));
}

StmtPtr Parser::expressionStatement() {
  ExprPtr expr = expression();
  consume(TokenType::SEMICOLON, "Expect ';' after expression.");
  return std::make_unique<ExpressionStmt>(std::move(expr));
}

StmtPtr Parser::varStatement() {
  Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
  ExprPtr initializer = nullptr;
  if (match({TokenType::EQUAL})) {
    initializer = expression();
  }
  consume(TokenType::SEMICOLON, "Expect ';' after expression.");
  return std::make_unique<VarDecl>(name, std::move(initializer));
}

std::vector<StmtPtr> Parser::parse() {
  std::vector<StmtPtr> stmts;
  while (!isAtEnd()) {
    stmts.push_back(declaration());
  }
  return stmts;
}
