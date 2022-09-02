#include "expr.h"

ExprVisitorResT Binary::accept(ExprVisitor &visitor) const {
  return visitor.visitBinaryExpr(*this);
}

ExprVisitorResT Grouping::accept(ExprVisitor &visitor) const {
  return visitor.visitGroupingExpr(*this);
}

ExprVisitorResT Literal::accept(ExprVisitor &visitor) const {
  return visitor.visitLiteralExpr(*this);
}

ExprVisitorResT Unary::accept(ExprVisitor &visitor) const {
  return visitor.visitUnaryExpr(*this);
}

ExprVisitorResT Variable::accept(ExprVisitor &visitor) const {
  return visitor.visitVariableExpr(*this);
}

ExprVisitorResT Assignment::accept(ExprVisitor &visitor) const {
  return visitor.visitAssignmentExpr(*this);
}

ExprVisitorResT Logical::accept(ExprVisitor &visitor) const {
  return visitor.visitLogicalExpr(*this);
}

ExprVisitorResT Call::accept(ExprVisitor &visitor) const {
  return visitor.visitCallExpr(*this);
}

ExprVisitorResT Get::accept(ExprVisitor &visitor) const {
  return visitor.visitGetExpr(*this);
}

ExprVisitorResT Set::accept(ExprVisitor &visitor) const {
  return visitor.visitSetExpr(*this);
}

ExprVisitorResT This::accept(ExprVisitor &visitor) const {
  return visitor.visitThisExpr(*this);
}
