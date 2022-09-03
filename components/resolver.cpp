#include "resolver.h"
#include "interpreter.h"

Resolver::Resolver(Interpreter &ip, ErrorReporter &errorReporter)
    : ip_(ip), errorReporter_(errorReporter), scopes_(std::vector<SymbolMap>()),
      currentFunction_(FunctionType::NONE), currentClass_(ClassType::NONE) {}

StmtVisitorResT Resolver::visitBlock(const Block &block) {
  beginScope();
  resolve(block.stmts);
  endScope();
  return StmtVisitorResT();
}

StmtVisitorResT Resolver::visitVarDecl(const VarDecl &stmt) {
  declare(stmt.name);
  if (stmt.initializer != nullptr) {
    resolve(stmt.initializer);
  }
  define(stmt.name);
  return StmtVisitorResT();
}

ExprVisitorResT Resolver::visitVariableExpr(const Variable &expr) {
  if (scopes_.size() > 0) {
    const auto &top = scopes_.back();
    if (top.find(expr.name.lexeme) != top.end() && !top.at(expr.name.lexeme)) {
      errorReporter_.report(
          expr.name.line, expr.name.lexeme,
          "Cannot read local variable in its own initializer.");
    }
  }
  resolveLocal(expr, expr.name);
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitAssignmentExpr(const Assignment &expr) {
  resolve(expr.value);
  resolveLocal(expr, expr.name);
  return ExprVisitorResT();
}

StmtVisitorResT Resolver::visitFunStmt(const FunStmt &fun) {
  declare(fun.name);
  define(fun.name);
  resolveFun(fun, FunctionType::FUNCTION);
  return StmtVisitorResT();
}

StmtVisitorResT Resolver::visitClassStmt(const ClassStmt &c) {
  ClassType enclosingClass = currentClass_;
  currentClass_ = ClassType::CLASS;
  declare(c.name);
  define(c.name);
  if (c.super != nullptr && c.name.lexeme == c.super->name.lexeme) {
    errorReporter_.report(c.super->name.line,
                          " A class can't inherit from itself.");
  }
  if (c.super != nullptr) {
    currentClass_ = ClassType::SUBCLASS;
    resolve(*c.super);
  }
  if (c.super != nullptr) {
    beginScope();
    scopes_.back()["super"] = true;
  }

  beginScope();
  scopes_.back()["this"] = true;
  for (const auto &method : c.methods) {
    FunctionType decl = method->name.lexeme == "init" ? FunctionType::INIT
                                                      : FunctionType::METHOD;
    resolveFun(*method, decl);
  }
  endScope();
  if (c.super != nullptr) {
    endScope();
  }
  currentClass_ = enclosingClass;
  return StmtVisitorResT();
}

StmtVisitorResT Resolver::visitExpressionStmt(const ExpressionStmt &stmt) {
  resolve(stmt.expr);
  return StmtVisitorResT();
}

StmtVisitorResT Resolver::visitIfStmt(const IfStmt &stmt) {
  resolve(stmt.condition);
  resolve(stmt.thenStmt);
  if (stmt.elseStmt != nullptr) {
    resolve(stmt.elseStmt);
  }
}

StmtVisitorResT Resolver::visitPrintStmt(const PrintStmt &stmt) {
  resolve(stmt.expr);
  return StmtVisitorResT();
}

StmtVisitorResT Resolver::visitWhileStmt(const WhileStmt &stmt) {
  resolve(stmt.condition);
  resolve(stmt.stmt);
  return StmtVisitorResT();
}

StmtVisitorResT Resolver::visitReturnStmt(const ReturnStmt &stmt) {
  if (currentFunction_ == FunctionType::NONE) {
    errorReporter_.report(stmt.keyword.line,
                          "Can't return from top-level code.");
  }
  if (stmt.value != nullptr) {
    if (currentFunction_ == FunctionType::INIT) {
      errorReporter_.report(stmt.keyword.line,
                            "Can't return a value from an initializer.");
    }
    resolve(stmt.value);
  }
  return StmtVisitorResT();
}

ExprVisitorResT Resolver::visitBinaryExpr(const Binary &expr) {
  resolve(expr.left);
  resolve(expr.right);
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitLogicalExpr(const Logical &expr) {
  resolve(expr.left);
  resolve(expr.right);
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitCallExpr(const Call &expr) {
  resolve(expr.callee);
  for (const auto &arg : expr.arguments) {
    resolve(arg);
  }
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitGroupingExpr(const Grouping &expr) {
  resolve(expr.expr);
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitGetExpr(const Get &expr) {
  resolve(expr.object);
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitSetExpr(const Set &expr) {
  resolve(expr.object);
  resolve(expr.value);
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitThisExpr(const This &expr) {
  if (currentClass_ == ClassType::NONE) {
    errorReporter_.report(expr.keyword.line,
                          " Can't use 'this' outside of a class.");
    return ExprVisitorResT();
  }
  resolveLocal(expr, expr.keyword);
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitSuperExpr(const Super &expr) {
  if (currentClass_ == ClassType::NONE) {
    errorReporter_.report(expr.keyword.line,
                          "Cannot use 'super' outside of a class.");
  } else if (currentClass_ != ClassType::SUBCLASS) {
    errorReporter_.report(expr.keyword.line,
                          "Cannot use 'super' in a class with no super class.");
  }
  resolveLocal(expr, expr.keyword);
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitLiteralExpr(const Literal &expr) {
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitUnaryExpr(const Unary &expr) {
  resolve(expr.right);
  return ExprVisitorResT();
}

void Resolver::resolveFun(const FunStmt &fun, FunctionType type) {
  FunctionType enclosingFunction = currentFunction_;
  currentFunction_ = type;
  beginScope();
  for (const auto &param : fun.params) {
    declare(param);
    define(param);
  }
  resolve(fun.body);
  endScope();
  currentFunction_ = enclosingFunction;
}

void Resolver::beginScope() { scopes_.push_back(SymbolMap()); }

void Resolver::endScope() { scopes_.pop_back(); }

void Resolver::resolve(const std::vector<StmtPtr> &stmts) {
  for (const auto &stmt : stmts) {
    resolve(stmt);
  }
}

void Resolver::resolve(const StmtPtr &stmt) { stmt->accept(*this); }
void Resolver::resolve(const Stmt &stmt) { stmt.accept(*this); }

void Resolver::resolve(const ExprPtr &expr) { expr->accept(*this); }
void Resolver::resolve(const Expr &expr) { expr.accept(*this); }

void Resolver::resolveLocal(const Expr &expr, const Token &name) {
  for (int i = scopes_.size() - 1; i >= 0; i--) {
    if (scopes_[i].find(name.lexeme) != scopes_[i].end()) {
      ip_.resolve(expr, scopes_.size() - 1 - i);
      return;
    }
  }
}

void Resolver::declare(const Token &name) {
  if (scopes_.size() > 0) {
    auto &top = scopes_.back();
    if (top.find(name.lexeme) != top.end()) {
      errorReporter_.report(name.line, name.lexeme,
                            "Already a variable with this name in this scope.");
    }
    top[name.lexeme] = false;
  }
}

void Resolver::define(const Token &name) {
  if (scopes_.size() > 0) {
    scopes_.back()[name.lexeme] = true;
  }
}
