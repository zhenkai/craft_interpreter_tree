#include "stmt.h"

StmtVisitorResT ExpressionStmt::accept(StmtVisitor& visitor) const {
  return visitor.visitExpressionStmt(*this);
}

StmtVisitorResT PrintStmt::accept(StmtVisitor& visitor) const {
  return visitor.visitPrintStmt(*this);
}
