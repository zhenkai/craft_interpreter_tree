#pragma once

#include "env.h"
#include "error.h"
#include "expr.h"
#include "stmt.h"
#include <memory>
#include <unordered_map>

using LocalMap = std::unordered_map<uintptr_t, int>;

class Interpreter : public ExprVisitor, public StmtVisitor {
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
  ExprVisitorResT visitGetExpr(const Get &expr) override;
  StmtVisitorResT visitPrintStmt(const PrintStmt &stmt) override;
  StmtVisitorResT visitExpressionStmt(const ExpressionStmt &stmt) override;
  StmtVisitorResT visitVarDecl(const VarDecl &stmt) override;
  StmtVisitorResT visitBlock(const Block &block) override;
  StmtVisitorResT visitIfStmt(const IfStmt &stmt) override;
  StmtVisitorResT visitWhileStmt(const WhileStmt &stmt) override;
  StmtVisitorResT visitFunStmt(const FunStmt &stmt) override;
  StmtVisitorResT visitReturnStmt(const ReturnStmt &stmt) override;
  StmtVisitorResT visitClassStmt(const ClassStmt &stmt) override;
  void interpret(const std::vector<StmtPtr> &stmts);
  void executeBlock(const std::vector<StmtPtr> &block, EnvPtr env);
  EnvPtr globalEnv() { return globalEnv_; }
  void resolve(const Expr &expr, int depth);

private:
  ExprVisitorResT eval(const ExprPtr &expr);
  ExprVisitorResT eval(const Expr &expr);
  StmtVisitorResT execute(const StmtPtr &stmt);
  std::any lookUpVariable(const Token &name, const Expr &expr);
  ErrorReporter &errorReporter_;
  EnvPtr globalEnv_;
  EnvPtr env_;
  LocalMap locals_;
};

class Return : public std::runtime_error {
public:
  Return(std::any value)
      : std::runtime_error("hack"), value(std::move(value)) {}

  const std::any value;
};
