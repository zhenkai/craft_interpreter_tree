#pragma once

#include "expr.h"
#include "error.h"

class Interpreter: public ExprVisitor {
public:
  Interpreter(ErrorReporter& errorReporter): errorReporter_(errorReporter) {}
  ExprVisitorResT visitBinaryExpr(const Binary& expr) override;
  ExprVisitorResT visitGroupingExpr(const Grouping& expr) override;
  ExprVisitorResT visitLiteralExpr(const Literal& expr) override;
  ExprVisitorResT visitUnaryExpr(const Unary& expr) override;
  void interpret(const Expr& expr);
private:
  ExprVisitorResT eval(const ExprPtr& expr);
  ExprVisitorResT eval(const Expr& expr);
  ErrorReporter& errorReporter_;
};
