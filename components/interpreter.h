#pragma once

#include "env.h"
#include "error.h"
#include "expr.h"
#include "stmt.h"
#include <memory>

class Interpreter : public ExprVisitor, StmtVisitor {
public:
  explicit Interpreter(ErrorReporter &errorReporter);
  ExprVisitorResT visitBinaryExpr(const Binary &expr) override;
  ExprVisitorResT visitGroupingExpr(const Grouping &expr) override;
  ExprVisitorResT visitLiteralExpr(const Literal &expr) override;
  ExprVisitorResT visitUnaryExpr(const Unary &expr) override;
  ExprVisitorResT visitVariableExpr(const Variable &expr) override;
  ExprVisitorResT visitAssignmentExpr(const Assignment &expr) override;
  ExprVisitorResT visitLogicalExpr(const Logical &expr) override;
  ExprVisitorResT visitCallExpr(const Call &expr) override;
  StmtVisitorResT visitPrintStmt(const PrintStmt &stmt) override;
  StmtVisitorResT visitExpressionStmt(const ExpressionStmt &stmt) override;
  StmtVisitorResT visitVarDecl(const VarDecl &stmt) override;
  StmtVisitorResT visitBlock(const Block &block) override;
  StmtVisitorResT visitIfStmt(const IfStmt &stmt) override;
  StmtVisitorResT visitWhileStmt(const WhileStmt &stmt) override;
  StmtVisitorResT visitFunStmt(const FunStmt &stmt) override;
  StmtVisitorResT visitReturnStmt(const ReturnStmt &stmt) override;
  void interpret(const std::vector<StmtPtr> &stmts);
  void executeBlock(const std::vector<StmtPtr> &block, EnvPtr env);
  EnvPtr globalEnv() { return globalEnv_; }

private:
  ExprVisitorResT eval(const ExprPtr &expr);
  ExprVisitorResT eval(const Expr &expr);
  StmtVisitorResT execute(const StmtPtr &stmt);
  ErrorReporter &errorReporter_;
  EnvPtr globalEnv_;
  EnvPtr env_;
};

class Return : public std::runtime_error {
public:
  Return(std::any value)
      : std::runtime_error("hack"), value(std::move(value)) {}

  const std::any value;
};
