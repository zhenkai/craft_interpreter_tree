#pragma once

#include "expr.h"
#include "error.h"

class Interpreter: public Visitor {
public:
  Interpreter(ErrorReporter& errorReporter): errorReporter_(errorReporter) {}
  VisitorResT visitBinaryExpr(const Binary& expr) override;
  VisitorResT visitGroupingExpr(const Grouping& expr) override;
  VisitorResT visitLiteralExpr(const Literal& expr) override;
  VisitorResT visitUnaryExpr(const Unary& expr) override;
  void interpret(const Expr& expr);
private:
  VisitorResT eval(const Expr& expr);
  ErrorReporter& errorReporter_;
}
