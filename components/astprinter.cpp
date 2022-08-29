#include "astprinter.h"
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

void AstPrinter::visitBinaryExpr(const Binary& expr) {
  std::vector<const Expr*> exprs = {expr.left.get(), expr.right.get()};
  parenthesize(expr.op.lexeme(), exprs);
}

void AstPrinter::visitGroupingExpr(const Grouping& expr) {
  std::vector<const Expr*> exprs = {expr.expr.get()};
  parenthesize("group", exprs);
}

void AstPrinter::visitLiteralExpr(const Literal& expr) {
  auto& value = expr.value;
  if (!value.has_value()) { s_ << "nil"; }
  std::stringstream s;
  if (value.type() == typeid(std::string)) {
    s << std::any_cast<std::string>(value);
  } else if (value.type() == typeid(double)) {
    s << std::any_cast<double>(value);
  } else if (value.type() == typeid(int)) {
    s << std::any_cast<int>(value);
  } else if (value.type() == typeid(float)) {
    s << std::any_cast<float>(value);
  } else {
    s << "LITERAL: unknown";
  }

  s_ << s.str();
}

void AstPrinter::visitUnaryExpr(const Unary& expr) {
  std::vector<const Expr*> exprs = {expr.right.get()};
  parenthesize(expr.op.lexeme(), exprs);
}
