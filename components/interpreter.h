#pragma once

#include "expr.h"
#include "error.h"
#include "stmt.h"

class Interpreter: public ExprVisitor, StmtVisitor {
public:
  Interpreter(ErrorReporter& errorReporter): errorReporter_(errorReporter) {}
  ExprVisitorResT visitBinaryExpr(const Binary& expr) override;
  ExprVisitorResT visitGroupingExpr(const Grouping& expr) override;
  ExprVisitorResT visitLiteralExpr(const Literal& expr) override;
  ExprVisitorResT visitUnaryExpr(const Unary& expr) override;
  ExprVisitorResT visitVariableExpr(const Variable& expr) override;
  StmtVisitorResT visitPrintStmt(const PrintStmt& stmt) override;
  StmtVisitorResT visitExpressionStmt(const ExpressionStmt& stmt) override;
  StmtVisitorResT visitVarStmt(const VarStmt& stmt) override;
  void interpret(const std::vector<StmtPtr>& stmts);
private:
  ExprVisitorResT eval(const ExprPtr& expr);
  ExprVisitorResT eval(const Expr& expr);
  StmtVisitorResT execute(const StmtPtr& stmt);
  ErrorReporter& errorReporter_;
};
