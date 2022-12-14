#pragma once

#include "expr.h"
#include <sstream>

class AstPrinter : public ExprVisitor {
public:
  ExprVisitorResT visitBinaryExpr(const Binary &expr) override;
  ExprVisitorResT visitGroupingExpr(const Grouping &expr) override;
  ExprVisitorResT visitLiteralExpr(const Literal &expr) override;
  ExprVisitorResT visitUnaryExpr(const Unary &expr) override;
  ExprVisitorResT visitVariableExpr(const Variable &expr) override;
  ExprVisitorResT visitAssignmentExpr(const Assignment &expr) override;
  ExprVisitorResT visitLogicalExpr(const Logical &expr) override;
  std::string print(const Expr &expr);

private:
  void parenthesize(const std::string &name,
                    const std::vector<const Expr *> &exprs);
  std::stringstream s_;
};
