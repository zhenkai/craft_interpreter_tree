#include "expr.h"

void Binary::accept(Visitor& visitor) const {
  return visitor.visitBinaryExpr(*this);
}

void Grouping::accept(Visitor& visitor) const {
  return visitor.visitGroupingExpr(*this);
}

void Literal::accept(Visitor& visitor) const {
  return visitor.visitLiteralExpr(*this);
}

void Unary::accept(Visitor& visitor) const {
  return visitor.visitUnaryExpr(*this);
}
