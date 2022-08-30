#include "expr.h"

VisitorResT Binary::accept(Visitor& visitor) const {
  return visitor.visitBinaryExpr(*this);
}

VisitorResT Grouping::accept(Visitor& visitor) const {
  return visitor.visitGroupingExpr(*this);
}

VisitorResT Literal::accept(Visitor& visitor) const {
  return visitor.visitLiteralExpr(*this);
}

VisitorResT Unary::accept(Visitor& visitor) const {
  return visitor.visitUnaryExpr(*this);
}
