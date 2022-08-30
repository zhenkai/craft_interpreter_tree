#include "interpreter.h"
#include "token.h"
#include "../utils/any_util.h"
#include <iostream>

namespace {
bool isTruthy(std::any value) {
  // only Nil and false are false; everything else is true.
  if (!value.has_value()) return false;
  if (value.type() == typeid(bool)) return std::any_cast<bool>(value);
  return true;
}

void checkNumber(const Token& op, const std::any& operand) {
  if (operand.type() == typeid(double)) return;
  throw new RuntimeError(op.errorStr() + ": operand must be a number.");
}

void checkNumbers(const Token& op, const std::any& left, const std::any& right) {
  if (left.type() == typeid(double) && right.type() == typeid(double)) return;
  throw new RuntimeError(op.errorStr() + ": operands must be a number.");
}

}
ExprVisitorResT Interpreter::visitBinaryExpr(const Binary& expr) {
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
      if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
        return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
      }
      throw new RuntimeError(expr.op.errorStr() + ": operands must both be either doubles or strings");
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

ExprVisitorResT Interpreter::visitGroupingExpr(const Grouping& expr) {
  return eval(expr.expr);
}

ExprVisitorResT Interpreter::visitLiteralExpr(const Literal& expr) {
  return expr.value;
}

ExprVisitorResT Interpreter::visitUnaryExpr(const Unary& expr) {
  auto right = eval(expr.right);
  switch(expr.op.type) {
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

ExprVisitorResT Interpreter::eval(const ExprPtr& expr) {
  return expr->accept(*this);
}

ExprVisitorResT Interpreter::eval(const Expr& expr) {
  return expr.accept(*this);
}

void Interpreter::interpret(const Expr& expr) {
  try {
    auto res = eval(expr);
    std::cout << anyToStr(res) << std::endl;
  } catch (RuntimeError* e) {
    errorReporter_.reportRuntimeError(*e);
  }
}
