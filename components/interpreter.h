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
  StmtVisitorResT visitPrintStmt(const PrintStmt &stmt) override;
  StmtVisitorResT visitExpressionStmt(const ExpressionStmt &stmt) override;
  StmtVisitorResT visitVarDecl(const VarDecl &stmt) override;
  StmtVisitorResT visitBlock(const Block &block) override;
  void interpret(const std::vector<StmtPtr> &stmts);

private:
  ExprVisitorResT eval(const ExprPtr &expr);
  ExprVisitorResT eval(const Expr &expr);
  StmtVisitorResT execute(const StmtPtr &stmt);
  void executeBlock(const Block &block, std::unique_ptr<Environment> env);
  ErrorReporter &errorReporter_;
  std::unique_ptr<Environment> env_;
};
