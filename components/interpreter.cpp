#include "interpreter.h"
#include "../utils/any_util.h"
#include "token.h"
#include <iostream>
#include <vector>

namespace {
bool isTruthy(std::any value) {
  // only Nil and false are false; everything else is true.
  if (!value.has_value())
    return false;
  if (value.type() == typeid(bool))
    return std::any_cast<bool>(value);
  return true;
}

void checkNumber(const Token &op, const std::any &operand) {
  if (operand.type() == typeid(double))
    return;
  throw new RuntimeError(op.errorStr() + ": operand must be a number.");
}

void checkNumbers(const Token &op, const std::any &left,
                  const std::any &right) {
  if (left.type() == typeid(double) && right.type() == typeid(double))
    return;
  throw new RuntimeError(op.errorStr() + ": operands must be a number.");
}

} // namespace

Interpreter::Interpreter(ErrorReporter &errorReporter)
    : errorReporter_(errorReporter), env_(std::make_unique<Environment>()) {}

ExprVisitorResT Interpreter::visitBinaryExpr(const Binary &expr) {
  auto left = eval(expr.left);
  auto right = eval(expr.right);
  switch (expr.op.type) {
  case TokenType::MINUS:
    checkNumbers(expr.op, left, right);
    return std::any_cast<double>(left) - std::any_cast<double>(right);
  case TokenType::SLASH:
    checkNumbers(expr.op, left, right);
    return std::any_cast<double>(left) / std::any_cast<double>(right);
  case TokenType::STAR:
    checkNumbers(expr.op, left, right);
    return std::any_cast<double>(left) * std::any_cast<double>(right);
  case TokenType::PLUS:
    if (left.type() == typeid(double) && right.type() == typeid(double)) {
      return std::any_cast<double>(left) + std::any_cast<double>(right);
    }
    if (left.type() == typeid(std::string) &&
        right.type() == typeid(std::string)) {
      return std::any_cast<std::string>(left) +
             std::any_cast<std::string>(right);
    }
    throw new RuntimeError(expr.op.errorStr() +
                           ": operands must both be either doubles or strings");
  case TokenType::GREATER:
    checkNumbers(expr.op, left, right);
    return std::any_cast<double>(left) > std::any_cast<double>(right);
  case TokenType::GREATER_EQUAL:
    checkNumbers(expr.op, left, right);
    return std::any_cast<double>(left) >= std::any_cast<double>(right);
  case TokenType::LESS:
    checkNumbers(expr.op, left, right);
    return std::any_cast<double>(left) < std::any_cast<double>(right);
  case TokenType::LESS_EQUAL:
    checkNumbers(expr.op, left, right);
    return std::any_cast<double>(left) <= std::any_cast<double>(right);
  case TokenType::BANG_EQUAL:
    return !anyEqual(left, right);
  case TokenType::EQUAL_EQUAL:
    return anyEqual(left, right);
  default:
    break;
  }
  throw new RuntimeError(expr.op.errorStr() + ": unsupported binary operator");
}

ExprVisitorResT Interpreter::visitGroupingExpr(const Grouping &expr) {
  return eval(expr.expr);
}

ExprVisitorResT Interpreter::visitLiteralExpr(const Literal &expr) {
  return expr.value;
}

ExprVisitorResT Interpreter::visitUnaryExpr(const Unary &expr) {
  auto right = eval(expr.right);
  switch (expr.op.type) {
  case TokenType::MINUS:
    checkNumber(expr.op, right);
    return -std::any_cast<double>(right);
  case TokenType::BANG:
    return !isTruthy(right);
  default:
    break;
  }
  throw new RuntimeError(expr.op.errorStr() + " unsupported unary operator");
}

ExprVisitorResT Interpreter::visitVariableExpr(const Variable &expr) {
  return env_->get(expr.name);
}

ExprVisitorResT Interpreter::visitAssignmentExpr(const Assignment &expr) {
  auto value = eval(expr.value);
  env_->assign(expr.name, value);
  return value;
}

ExprVisitorResT Interpreter::eval(const ExprPtr &expr) {
  return expr->accept(*this);
}

ExprVisitorResT Interpreter::eval(const Expr &expr) {
  return expr.accept(*this);
}

StmtVisitorResT Interpreter::visitPrintStmt(const PrintStmt &stmt) {
  auto value = eval(stmt.expr);
  std::cout << anyToStr(value) << std::endl;
  return StmtVisitorResT();
}

StmtVisitorResT Interpreter::visitExpressionStmt(const ExpressionStmt &stmt) {
  eval(stmt.expr);
  return StmtVisitorResT();
}

StmtVisitorResT Interpreter::visitVarDecl(const VarDecl &stmt) {
  std::any value;
  if (stmt.initializer != nullptr) {
    value = eval(stmt.initializer);
  }
  env_->define(stmt.name.lexeme, std::move(value));
  return StmtVisitorResT();
}

StmtVisitorResT Interpreter::visitBlock(const Block &block) {
  executeBlock(block, std::make_unique<Environment>(env_.get()));
  return StmtVisitorResT();
}

void Interpreter::executeBlock(const Block &block,
                               std::unique_ptr<Environment> env) {
  std::unique_ptr<Environment> enclosing = std::move(env_);

  env_ = std::move(env);

  // with poor man's scope guard
  try {
    for (const auto &stmt : block.stmts) {
      execute(stmt);
    }
  } catch (...) {
    env_ = std::move(enclosing);
    throw;
  }
  env_ = std::move(enclosing);
}

StmtVisitorResT Interpreter::execute(const StmtPtr &stmt) {
  stmt->accept(*this);
}

void Interpreter::interpret(const std::vector<StmtPtr> &stmts) {
  try {
    for (const auto &stmt : stmts) {
      execute(stmt);
    }
  } catch (RuntimeError *e) {
    errorReporter_.reportRuntimeError(*e);
  }
}
