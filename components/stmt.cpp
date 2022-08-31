#include "stmt.h"

StmtVisitorResT ExpressionStmt::accept(StmtVisitor &visitor) const {
  return visitor.visitExpressionStmt(*this);
}

StmtVisitorResT PrintStmt::accept(StmtVisitor &visitor) const {
  return visitor.visitPrintStmt(*this);
}

StmtVisitorResT VarDecl::accept(StmtVisitor &visitor) const {
  return visitor.visitVarDecl(*this);
}

StmtVisitorResT Block::accept(StmtVisitor &visitor) const {
  return visitor.visitBlock(*this);
}
