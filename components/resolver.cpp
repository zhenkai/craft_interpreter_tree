#include "resolver.h"
#include "interpreter.h"

Resolver::Resolver(Interpreter &ip, ErrorReporter &errorReporter)
    : ip_(ip), errorReporter_(errorReporter),
      scopes_(std::vector<SymbolMap>()) {}

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
  resolveFun(fun);
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
  if (stmt.value != nullptr) {
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

ExprVisitorResT Resolver::visitLiteralExpr(const Literal &expr) {
  return ExprVisitorResT();
}

ExprVisitorResT Resolver::visitUnaryExpr(const Unary &expr) {
  resolve(expr.right);
  return ExprVisitorResT();
}

void Resolver::resolveFun(const FunStmt &fun) {
  beginScope();
  for (const auto &param : fun.params) {
    declare(param);
    define(param);
  }
  resolve(fun.body);
  endScope();
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
    scopes_.back()[name.lexeme] = false;
  }
}

void Resolver::define(const Token &name) {
  if (scopes_.size() > 0) {
    scopes_.back()[name.lexeme] = true;
  }
}
