#include "astprinter.h"
#include <sstream>

std::string AstPrinter::parenthesize(const std::string& name, const std::vector<Expr>& exprs) {
  std::stringstream s;
  s << "(" << name;
  for (Expr expr : exprs) {
    s << " ";
    s << expr.accept(this);
  }
  s << ")";
  return s.str();
}

std::string AstPrinter::visitBinaryExpr(const Binary& expr) {
  std::vector<Expr> exprs = {expr.left, expr.right};
  return parenthesize(expr.op.lexeme(), exprs);
}

std::string AstPrinter::visitGroupExpr(const Grouping& expr) {
  std::vector<Expr> exprs = {expr.expr};
  return parenthesize("group", exprs);
}

std::string AstPrinter::visitLiteralExpr(const Literal& expr) {
  if (!expr.value.has_value()) { return "nil"; }
  std::stringstream s;
  if (literal_.type() == typeid(std::string)) {
    s << std::any_cast<std::string>(literal_);
  } else if (literal_.type() == typeid(double)) {
    s << std::any_cast<double>(literal_);
  } else if (literal_.type() == typeid(int)) {
    s << std::any_cast<int>(literal_);
  } else if (literal_.type() == typeid(float)) {
    s << std::any_cast<float>(literal_);
  } else {
    s << "LITERAL: unknown";
  }

  return s.str();
}

std::string AstPrinter::visitUnaryExpr(const Unary& expr) {
  std::vector<Expr> exprs = {expr.right};
  return parenthesize(expr.op.lexeme(), exprs);
}
