#include "interpreter.h"
#include "../utils/any_util.h"
#include "callable.h"
#include "class.h"
#include "function.h"
#include "instance.h"
#include "native.h"
#include "token.h"
#include <iostream>
#include <unordered_map>
#include <vector>

namespace {
std::uintptr_t id(const Expr &expr) {
  return reinterpret_cast<std::uintptr_t>(&expr);
}

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
    : errorReporter_(errorReporter),
      globalEnv_(std::make_shared<Environment>()), env_(globalEnv_),
      locals_(LocalMap()) {
  // add native functions to global env
  globalEnv_->define("clock", std::make_shared<LoxClock>());
}

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
  return lookUpVariable(expr.name, expr);
}

ExprVisitorResT Interpreter::visitThisExpr(const This &expr) {
  return lookUpVariable(expr.keyword, expr);
}

ExprVisitorResT Interpreter::visitSuperExpr(const Super &expr) {
  // should always have 'super' if we're visiting super here
  int dist = locals_.at(id(expr));
  ClassPtr superClass = any_cast<ClassPtr>(env_->getAt(dist, "super"));

  // "this" exists in the environment one hop closer than the one that
  // contains "super"
  InstancePtr object = any_cast<InstancePtr>(env_->getAt(dist - 1, "this"));
  FunPtr method = superClass->findMethod(expr.method.lexeme);
  if (method == nullptr) {
    throw new RuntimeError(expr.method.errorStr() + "Undefined property '" +
                           expr.method.lexeme + "'.");
  }
  return method->bind(object);
}

std::any Interpreter::lookUpVariable(const Token &name, const Expr &expr) {
  if (locals_.find(id(expr)) != locals_.end()) {
    return env_->getAt(locals_.at(id(expr)), name.lexeme);
  }
  return globalEnv_->get(name);
}

ExprVisitorResT Interpreter::visitAssignmentExpr(const Assignment &expr) {
  auto value = eval(expr.value);
  if (locals_.find(id(expr)) != locals_.end()) {
    env_->assignAt(locals_.at(id(expr)), expr.name, value);
  } else {
    globalEnv_->assign(expr.name, value);
  }
  return value;
}

ExprVisitorResT Interpreter::visitLogicalExpr(const Logical &expr) {
  auto left = eval(expr.left);
  if (expr.op.type == TokenType::OR) {
    if (isTruthy(left)) {
      return left;
    }
  } else {
    if (!isTruthy(left)) {
      return left;
    }
  }

  return eval(expr.right);
}

ExprVisitorResT Interpreter::visitCallExpr(const Call &expr) {
  auto callee = eval(expr.callee);

  std::vector<std::any> arguments;
  for (const auto &argument : expr.arguments) {
    arguments.push_back(eval(argument));
  }

  // using any together with shared_ptr makes this tricky
  CallablePtr fun = nullptr;
  if (callee.type() == typeid(FunPtr)) {
    fun = std::any_cast<FunPtr>(callee);
  } else if (callee.type() == typeid(ClassPtr)) {
    fun = std::any_cast<ClassPtr>(callee);
  } else if (callee.type() == typeid(std::shared_ptr<LoxClock>)) {
    fun = std::any_cast<std::shared_ptr<LoxClock>>(callee);
  }
  if (fun == nullptr) {
    throw new RuntimeError(expr.paren.errorStr() +
                           " Can only call functions and classes.");
  }

  if (arguments.size() != fun->arity()) {
    throw new RuntimeError(
        expr.paren.errorStr() + " Expected " + std::to_string(fun->arity()) +
        " arguments but got " + std::to_string(arguments.size()) + ".");
  }
  return fun->call(*this, arguments);
}

ExprVisitorResT Interpreter::visitGetExpr(const Get &expr) {
  auto object = eval(expr.object);
  if (object.type() != typeid(InstancePtr)) {
    throw new RuntimeError(expr.name.errorStr() +
                           " Only instances have properties.");
  }
  auto instance = std::any_cast<InstancePtr>(object);
  return instance->get(expr.name);
}

ExprVisitorResT Interpreter::visitSetExpr(const Set &expr) {
  auto object = eval(expr.object);
  if (object.type() != typeid(InstancePtr)) {
    throw new RuntimeError(expr.name.errorStr() +
                           " Only instances have properties.");
  }
  auto value = eval(expr.value);
  auto instance = std::any_cast<InstancePtr>(object);
  instance->set(expr.name, value);
  return ExprVisitorResT();
}

ExprVisitorResT Interpreter::eval(const ExprPtr &expr) {
  return expr->accept(*this);
}

ExprVisitorResT Interpreter::eval(const Expr &expr) {
  return expr.accept(*this);
}

StmtVisitorResT Interpreter::visitPrintStmt(const PrintStmt &stmt) {
  auto value = eval(stmt.expr);
  try {
    std::cout << anyToStr(value) << std::endl;
  } catch (std::exception *e) {
    std::cout << "Cannot print: unsupported type " << e->what() << std::endl;
  }
  return StmtVisitorResT();
}

StmtVisitorResT Interpreter::visitReturnStmt(const ReturnStmt &stmt) {
  std::any value;
  if (stmt.value != nullptr) {
    value = eval(stmt.value);
  }
  throw new Return(std::move(value));
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
  executeBlock(block.stmts, std::make_shared<Environment>(env_));
  return StmtVisitorResT();
}

StmtVisitorResT Interpreter::visitIfStmt(const IfStmt &stmt) {
  if (isTruthy(eval(stmt.condition))) {
    return execute(stmt.thenStmt);
  } else if (stmt.elseStmt != nullptr) {
    return execute(stmt.elseStmt);
  }
}

StmtVisitorResT Interpreter::visitWhileStmt(const WhileStmt &stmt) {
  while (isTruthy(eval(stmt.condition))) {
    execute(stmt.stmt);
  }
  return StmtVisitorResT();
}

StmtVisitorResT Interpreter::visitFunStmt(const FunStmt &stmt) {
  auto fun = std::make_shared<LoxFunction>(stmt, false, env_);
  env_->define(stmt.name.lexeme, fun);
  return StmtVisitorResT();
}

StmtVisitorResT Interpreter::visitClassStmt(const ClassStmt &stmt) {
  ClassPtr superPtr = nullptr;
  if (stmt.super != nullptr) {
    auto superClass = eval(*stmt.super);
    if (superClass.type() != typeid(ClassPtr)) {
      throw new RuntimeError(stmt.super->name.errorStr() +
                             " Superclass must be a class.");
    }
    superPtr = any_cast<ClassPtr>(superClass);
  }
  // Two-stage variable binding process allows references to the class
  //  inside its own methods.
  env_->define(stmt.name.lexeme, nullptr);
  if (superPtr != nullptr) {
    env_ = std::make_shared<Environment>(env_);
    env_->define("super", superPtr);
  }
  std::unordered_map<std::string, FunPtr> methods;
  for (const auto &method : stmt.methods) {
    FunPtr fun = std::make_shared<LoxFunction>(
        *method, method->name.lexeme == "init", env_);
    methods[method->name.lexeme] = fun;
  }

  auto klass = std::make_shared<LoxClass>(stmt.name.lexeme, superPtr,
                                          std::move(methods));
  if (superPtr != nullptr) {
    env_ = env_->enclosing();
  }
  env_->assign(stmt.name, klass);
  return StmtVisitorResT();
}

void Interpreter::executeBlock(const std::vector<StmtPtr> &block, EnvPtr env) {
  EnvPtr enclosing = env_;

  env_ = env;

  // with poor man's scope guard
  try {
    for (const auto &stmt : block) {
      execute(stmt);
    }
  } catch (...) {
    env_ = enclosing;
    throw;
  }
  env_ = enclosing;
}

StmtVisitorResT Interpreter::execute(const StmtPtr &stmt) {
  return stmt->accept(*this);
}

void Interpreter::resolve(const Expr &expr, int depth) {
  locals_[id(expr)] = depth;
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
