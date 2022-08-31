#include "astprinter.h"
#include "../utils/any_util.h"
#include <vector>

std::string AstPrinter::print(const Expr& expr) {
  s_ = std::stringstream();
  expr.accept(*this);
  return s_.str();
}

void AstPrinter::parenthesize(const std::string& name, const std::vector<const Expr*>& exprs) {
  s_ << "(" << name;
  for (const auto& expr : exprs) {
    s_ << " ";
    expr->accept(*this);
  }
  s_ << ")";
}

ExprVisitorResT AstPrinter::visitBinaryExpr(const Binary& expr) {
  std::vector<const Expr*> exprs = {expr.left.get(), expr.right.get()};
  parenthesize(expr.op.lexeme, exprs);
  return ExprVisitorResT();
}

ExprVisitorResT AstPrinter::visitGroupingExpr(const Grouping& expr) {
  std::vector<const Expr*> exprs = {expr.expr.get()};
  parenthesize("group", exprs);
  return ExprVisitorResT();
}

ExprVisitorResT AstPrinter::visitLiteralExpr(const Literal& expr) {
  auto& value = expr.value;
  s_ << anyToStr(value);
  return ExprVisitorResT();
}

ExprVisitorResT AstPrinter::visitUnaryExpr(const Unary& expr) {
  std::vector<const Expr*> exprs = {expr.right.get()};
  parenthesize(expr.op.lexeme, exprs);
  return ExprVisitorResT();
}

ExprVisitorResT AstPrinter::visitVariableExpr(const Variable& expr) {
  s_ << "var " << expr.name.lexeme;
  return ExprVisitorResT();
}

ExprVisitorResT AstPrinter::visitAssignmentExpr(const Assignment& expr) {
  std::vector<const Expr*> exprs = {expr.value.get()};
  parenthesize(expr.name.lexeme, exprs);
  return ExprVisitorResT();
}
